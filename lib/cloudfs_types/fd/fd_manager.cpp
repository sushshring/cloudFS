//
// Created by Sushrut Shringarputale on 2019-08-05.
//

#include <cstdio>
#include <filesystem>
#include <random>
#include "fd_manager.h"

namespace fs = std::filesystem;
using namespace cloudfs;

REGISTER_CLASS(FdManager, FdManagerImpl)

std::shared_ptr<FdEntity> FdManagerImpl::open(const std::string& path,
                                              off_t size, time_t time,
                                              bool forceTemporaryFile,
                                              bool create,
                                              bool dontWaitOnFdLock) {
  bool close = false;
  std::shared_ptr<FdEntity> ent;
  if (path.empty()) {
    errno = -EINVAL;
    return nullptr;
  }
  std::lock_guard<std::mutex> guard(fdManagerLock);

  // Search cache for path
  auto itr = entryTable.find(path);
  auto internalItr = openTable.end();
  if (itr != entryTable.end() && !forceTemporaryFile && !cacheDirExists()) {
    auto internalItrRange = openTable.equal_range(itr->second);
    internalItr = std::find_if(internalItrRange.first, internalItrRange.second,
                               [](auto elem) { return elem->isOpen(); });
  }
  if (itr != entryTable.end() && internalItr != openTable.end()) {
    ent = *internalItr;
    ent->duplicate();
    close = true;
  } else if (create) {
    std::string cachePath;
    if (!forceTemporaryFile && !makeCachePath(path, cachePath, true, false)) {
      GET_LOGGER(this)->error("failed to make cache path for object(%s).",
                              path);
      errno = -EIO;
      return nullptr;
    }
    ent = std::make_shared<FdEntity>(path, cachePath);
    if (!cachePath.empty()) {
      // using cache
      entryTable[path] = ent;
      openTable.insert(ent);
    } else {
      auto tmpPath = FdManagerImpl::makeRandomTempPath(path);
      entryTable[tmpPath] = ent;
      openTable.insert(ent);
    }
  } else {
    errno = -EINVAL;
    return nullptr;
  }
  // open
  if (0 != ent->open(size, time, dontWaitOnFdLock)) {
    if (close) {
      ent->close();
    }
    errno = -EIO;
    return nullptr;
  }
  if (close) {
    ent->close();
  }
  return ent;
}

std::string FdManagerImpl::makeRandomTempPath(const std::string& path) {
  std::string
      in = "qwertyuiopasdfghjklzxcvbnm1234567890QWERTYUIOPASDFGHJKLZXCVBNM",
      out;
  std::sample(in.begin(), in.end(), std::back_inserter(out), 64 /*length*/,
              std::mt19937{std::random_device{}()});
  std::stringstream sstream;
  sstream << NOCACHE_PATH_PREFIX_FORM << out << "__ /";
  auto tmpPath = sstream.str();
  tmpPath += path.empty() ? path : "";
  return tmpPath;
}

bool FdManagerImpl::makeCachePath(const std::string& path,
                                  std::string& cachePath, bool shouldMakeDir,
                                  bool isMirrorPath) {
  //  if (!cacheDirExists()) {
  //    cachePath = "";
  //    return true;
  //  }
  //  auto resolved_path = path;
  //  if (!isMirrorPath) {
  //    resolved_path += "/";
  //    resolved_path += bucket;
  //  } else {
  //    resolved_path += "/.";
  //    resolved_path += bucket;
  //    resolved_path += ".mirror";
  //  }
  auto resolvedPath = getCachePath(path, isMirrorPath);
  if (shouldMakeDir) {
    std::error_code ec;
    fs::create_directories(resolvedPath, ec);
    if (ec) {
      GET_LOGGER(this)->error("Error creating cache directory. Error code: %s",
                              ec.message());
      return false;
    }
    fs::permissions(resolvedPath, std::filesystem::perms::all,
                    std::filesystem::perm_options::add, ec);
    if (ec) {
      GET_LOGGER(this)->error(
          "Error setting permissions on cache directory. Error code: %s",
          ec.message());
      return false;
    }
  }
  cachePath = resolvedPath + path;
  return true;
}
