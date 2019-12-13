//
// Created by Sushrut Shringarputale on 2019-08-02.
//

#include <spdlog/sinks/rotating_file_sink.h>
#include <utility>
#include "cloudfs.h"
#include "cloudfs_internal.h"

using namespace cloudfs::cloudfs_internal;
std::unique_ptr<CloudfsInternal> CloudfsInternal::initialize(
    CloudfsArgs&& args) {
  struct unique_enabler : public CloudfsInternal {
    explicit unique_enabler(CloudfsArgs args)
        : CloudfsInternal(std::move(args)) {}
  };
  return std::make_unique<unique_enabler>(args);
}

std::unique_ptr<cloudfs::Cloudfs>& CloudfsInternal::getCloudFs(
    std::optional<YAML::Node> args) {
  return Cloudfs::instance(std::move(args));
}

std::shared_ptr<spdlog::logger> CloudfsInternal::internalLog =
    spdlog::rotating_logger_mt("internal_logger", "logs/rotating_internal.txt",
                               1048576 * 5, 3);

CloudfsInternal::CloudfsInternal(CloudfsArgs args) : fop(fuse_operations()) {
  fop.access = access;
  fop.chmod = chmod;
  fop.chown = chown;
  fop.create = create;
  fop.destroy = destroy;
  fop.fallocate = fallocate;
  fop.fgetattr = fgetattr;
  fop.flush = flush;
  fop.flock = flock;
  fop.fsync = fsync;
  fop.fsyncdir = fsyncdir;
  fop.ftruncate = ftruncate;
  fop.getattr = getattr;
  fop.getxattr = getxattr;
  fop.init = init;
  fop.ioctl = ioctl;
  fop.link = link;
  fop.listxattr = listxattr;
  fop.lock = lock;
  fop.mkdir = mkdir;
  fop.mknod = mknod;
  fop.open = open;
  fop.opendir = opendir;
  fop.poll = poll;
  fop.read = read;
  fop.readdir = readdir;
  fop.readlink = readlink;
  fop.release = release;
  fop.releasedir = releasedir;
  fop.removexattr = removexattr;
  fop.rmdir = rmdir;
  fop.read_buf = read_buf;
  fop.rename = rename;
  fop.setxattr = setxattr;
  fop.statfs = statfs;
  fop.symlink = symlink;
  fop.truncate = truncate;
  fop.unlink = unlink;
  fop.utimens = utimens;
  fop.write = write;
  fop.write_buf = write_buf;
  fuse_main(args.get_argc(), args.get_argv(), &fop, nullptr);
}

int cloudfs::cloudfs_internal::CloudfsInternal::getattr(const char* path,
                                                        struct stat* statbuf) {
  try {
    getCloudFs()->getattr(std::string(path),
                          std::shared_ptr<struct stat>(statbuf));
  } catch (std::exception& e) {
    internalLog->error("INTERNAL FS ERROR: getattr: %s", e.what());
    return -1;
  }
  return 0;
}

int CloudfsInternal::readlink(const char* path, char* target, size_t size) {
  try {
    auto readResult = getCloudFs()->readlink(std::string(path), size);
    std::copy_n(readResult.begin(), size, target);
  } catch (std::exception& e) {
    internalLog->error("INTERNAL FS ERROR: readlink: %s", e.what());
    return -1;
  }
  return 0;
}

int CloudfsInternal::mknod(const char* path, mode_t mode, dev_t dev) {
  try {
    return getCloudFs()->mknod(std::string(path), mode, dev);
  } catch (std::exception& e) {
    internalLog->error("INTERNAL FS ERROR: mknod: %s", e.what());
    return -1;
  }
}

int CloudfsInternal::mkdir(const char* path, mode_t mode) {
  try {
    return getCloudFs()->mkdir(std::string(path), mode);
  } catch (std::exception& e) {
    internalLog->error("INTERNAL FS ERROR: mkdir: %s", e.what());
    return -1;
  }
  return 0;
}

int CloudfsInternal::unlink(const char* path) {
  try {
    return getCloudFs()->unlink(std::string(path));
  } catch (std::exception& e) {
    internalLog->error("INTERNAL FS ERROR: unlink: %s", e.what());
    return -1;
  }
  return 0;
}

int CloudfsInternal::rmdir(const char* path) {
  try {
    return getCloudFs()->rmdir(std::string(path));
  } catch (std::exception& e) {
    internalLog->error("INTERNAL FS ERROR: rmdir: %s", e.what());
    return -1;
  }
  return 0;
}

int CloudfsInternal::symlink(const char* path, const char* target) {
  try {
    return getCloudFs()->symlink(std::string(path), std::string(target));
  } catch (std::exception& e) {
    internalLog->error("INTERNAL FS ERROR: symlink %s", e.what());
    return -1;
  }
  return 0;
}

int CloudfsInternal::rename(const char* path, const char* target) {
  try {
    return getCloudFs()->rename(std::string(path), std::string(target));
  } catch (std::exception& e) {
    internalLog->error("INTERNAL FS ERROR: rename %s", e.what());
    return -1;
  }
  return 0;
}
int CloudfsInternal::link(const char* path, const char* target) {
  try {
    return getCloudFs()->link(std::string(path), std::string(target));
  } catch (std::exception& e) {
    internalLog->error("INTERNAL FS ERROR: link %s", e.what());
    return -1;
  }
  return 0;
}
int CloudfsInternal::chmod(const char* path, mode_t mode) {
  try {
    return getCloudFs()->chmod(std::string(path), mode);
  } catch (std::exception& e) {
    internalLog->error("INTERNAL FS ERROR: chmod %s", e.what());
    return -1;
  }
  return 0;
}
int CloudfsInternal::chown(const char* path, uid_t userId, gid_t groupId) {
  try {
    return getCloudFs()->chown(std::string(path), userId, groupId);
  } catch (std::exception& e) {
    internalLog->error("INTERNAL FS ERROR: chown %s", e.what());
    return -1;
  }
  return 0;
}

int CloudfsInternal::truncate(const char* path, off_t size) {
  try {
    return getCloudFs()->truncate(std::string(path), size);
  } catch (std::exception& e) {
    internalLog->error("INTERNAL FS ERROR: truncate %s", e.what());
    return -1;
  }
  return 0;
}
int CloudfsInternal::open(const char* path, struct fuse_file_info* fileInfo) {
  try {
    return getCloudFs()->open(std::string(path),
                              std::shared_ptr<fuse_file_info>(fileInfo));
  } catch (std::exception& e) {
    internalLog->error("INTERNAL FS ERROR: open %s", e.what());
    return -1;
  }
  return 0;
}

int CloudfsInternal::read(const char* path, char* buf, size_t size,
                          off_t offset, struct fuse_file_info* fileInfo) {
  try {
    auto dest = std::vector<char>();
    dest.assign(buf, buf + size);
    return getCloudFs()->read(std::string(path), dest, offset,
                              std::shared_ptr<fuse_file_info>(fileInfo));
  } catch (std::exception& e) {
    internalLog->error("INTERNAL FS ERROR: read %s", e.what());
    return -1;
  }
  return 0;
}

int CloudfsInternal::write(const char* path, const char* source, size_t size,
                           off_t offset, struct fuse_file_info* fileInfo) {
  try {
    return getCloudFs()->write(std::string(path),
                               std::vector<char>(source, source + size), offset,
                               std::shared_ptr<fuse_file_info>(fileInfo));
  } catch (std::exception& e) {
    internalLog->error("INTERNAL FS ERROR: write %s", e.what());
    return -1;
  }
  return 0;
}
int CloudfsInternal::statfs(const char* path, struct statvfs* statbuf) {
  try {
    return getCloudFs()->statfs(std::string(path),
                                std::shared_ptr<struct statvfs>(statbuf));
  } catch (std::exception& e) {
    internalLog->error("INTERNAL FS ERROR: statfs %s", e.what());
    return -1;
  }
  return 0;
}

int CloudfsInternal::flush(const char* path, struct fuse_file_info* fileInfo) {
  try {
    return getCloudFs()->flush(std::string(path),
                               std::shared_ptr<fuse_file_info>(fileInfo));
  } catch (std::exception& e) {
    internalLog->error("INTERNAL FS ERROR: flush %s", e.what());
    return -1;
  }
  return 0;
}

int CloudfsInternal::release(const char* path,
                             struct fuse_file_info* fileInfo) {
  try {
    return getCloudFs()->release(std::string(path),
                                 std::shared_ptr<fuse_file_info>(fileInfo));
  } catch (std::exception& e) {
    internalLog->error("INTERNAL FS ERROR: release %s", e.what());
    return -1;
  }
  return 0;
}

int CloudfsInternal::fsync(const char* path, int datasync,
                           struct fuse_file_info* fileInfo) {
  try {
    return getCloudFs()->fsync(std::string(path), datasync,
                               std::shared_ptr<fuse_file_info>(fileInfo));
  } catch (std::exception& e) {
    internalLog->error("INTERNAL FS ERROR: fsync %s", e.what());
    return -1;
  }
  return 0;
}

#ifdef __APPLE__
int CloudfsInternal::setxattr(const char* path, const char* name,
                              const char* value, size_t size, int flags,
                              uint32_t position) {
  try {
    return getCloudFs()->setxattr(std::string(path), std::string(name),
                                  std::string(value), size, flags, position);
  } catch (std::exception& e) {
    internalLog->error("INTERNAL FS ERROR: setxattr %s", e.what());
    return -1;
  }
  return 0;
}
int CloudfsInternal::getxattr(const char* path, const char* name, char* value,
                              size_t size, uint32_t position) {
  try {
    return getCloudFs()->getxattr(std::string(path), std::string(name),
                                  std::vector<char>(value, value + size),
                                  position);
  } catch (std::exception& e) {
    internalLog->error("INTERNAL FS ERROR: flush %s", e.what());
    return -1;
  }
  return 0;
}
#else
int CloudfsInternal::setxattr(const char* path, const char* name,
                              const char* value, size_t size, int flags) {
  try {
    return getCloudFs()->setxattr(std::string(path), std::string(name),
                                  std::string(value), size, flags);
  } catch (std::exception& e) {
    internalLog->error("INTERNAL FS ERROR: setxattr %s", e.what());
    return -1;
  }
  return 0;
}
int CloudfsInternal::getxattr(const char* path, const char* name, char* value,
                              size_t size) {
  try {
    return getCloudFs()->getxattr(std::string(path), std::string(name),
                                  std::vector<char>(value, value + size));
  } catch (std::exception& e) {
    internalLog->error("INTERNAL FS ERROR: setxattr %s", e.what());
    return -1;
  }
  return 0;
}
#endif

int CloudfsInternal::listxattr(const char* path, char* list, size_t size) {
  try {
    return getCloudFs()->listxattr(std::string(path),
                                   std::vector<char>(list, list + size));
  } catch (std::exception& e) {
    internalLog->error("INTERNAL FS ERROR: listxattr %s", e.what());
    return -1;
  }
  return 0;
}
int CloudfsInternal::removexattr(const char* path, const char* list) {
  try {
    return getCloudFs()->removexattr(std::string(path), std::string(list));
  } catch (std::exception& e) {
    internalLog->error("INTERNAL FS ERROR: removexattr %s", e.what());
    return -1;
  }
  return 0;
}
int CloudfsInternal::opendir(const char* path,
                             struct fuse_file_info* fileInfo) {
  try {
    return getCloudFs()->opendir(std::string(path),
                                 std::shared_ptr<fuse_file_info>(fileInfo));
  } catch (std::exception& e) {
    internalLog->error("INTERNAL FS ERROR: opendir %s", e.what());
    return -1;
  }
  return 0;
}
int CloudfsInternal::readdir(const char* path, void* buf,
                             fuse_fill_dir_t filler, off_t offset,
                             struct fuse_file_info* fileInfo) {
  try {
    return getCloudFs()->readdir(
        std::string(path), std::shared_ptr<char>((char*)buf), filler, offset,
        std::shared_ptr<fuse_file_info>(fileInfo));
  } catch (std::exception& e) {
    internalLog->error("INTERNAL FS ERROR: readdir %s", e.what());
    return -1;
  }
  return 0;
}
int CloudfsInternal::releasedir(const char* path,
                                struct fuse_file_info* fileInfo) {
  try {
    return getCloudFs()->releasedir(std::string(path),
                                    std::shared_ptr<fuse_file_info>(fileInfo));
  } catch (std::exception& e) {
    internalLog->error("INTERNAL FS ERROR: releasedir %s", e.what());
    return -1;
  }
  return 0;
}
int CloudfsInternal::fsyncdir(const char* path, int datasync,
                              struct fuse_file_info* fileInfo) {
  try {
    return getCloudFs()->fsyncdir(std::string(path), datasync,
                                  std::shared_ptr<fuse_file_info>(fileInfo));
  } catch (std::exception& e) {
    internalLog->error("INTERNAL FS ERROR: fsyncdir %s", e.what());
    return -1;
  }
  return 0;
}
void* CloudfsInternal::init(struct fuse_conn_info* conn) {
  try {
    return getCloudFs()->init(std::shared_ptr<fuse_conn_info>(conn)).get();
  } catch (std::exception& e) {
    internalLog->error("INTERNAL FS ERROR: init %s", e.what());
    return nullptr;
  }
  return nullptr;
}
void CloudfsInternal::destroy(void*) {
  try {
    return getCloudFs()->destroy();
  } catch (std::exception& e) {
    internalLog->error("INTERNAL FS ERROR: destroy %s", e.what());
  }
}
int CloudfsInternal::access(const char* path, int mask) {
  try {
    return getCloudFs()->access(std::string(path), mask);
  } catch (std::exception& e) {
    internalLog->error("INTERNAL FS ERROR: access %s", e.what());
    return -1;
  }
  return 0;
}
int CloudfsInternal::create(const char* path, mode_t mode,
                            struct fuse_file_info* fileInfo) {
  try {
    return getCloudFs()->create(std::string(path), mode,
                                std::shared_ptr<fuse_file_info>(fileInfo));
  } catch (std::exception& e) {
    internalLog->error("INTERNAL FS ERROR: create %s", e.what());
    return -1;
  }
  return 0;
}
int CloudfsInternal::ftruncate(const char* path, off_t offset,
                               struct fuse_file_info* fileInfo) {
  try {
    return getCloudFs()->ftruncate(std::string(path), offset,
                                   std::shared_ptr<fuse_file_info>(fileInfo));
  } catch (std::exception& e) {
    internalLog->error("INTERNAL FS ERROR: ftruncate %s", e.what());
    return -1;
  }
  return 0;
}
int CloudfsInternal::fgetattr(const char* path, struct stat* stat,
                              struct fuse_file_info* fileInfo) {
  try {
    return getCloudFs()->fgetattr(std::string(path),
                                  std::shared_ptr<struct stat>(stat),
                                  std::shared_ptr<fuse_file_info>(fileInfo));
  } catch (std::exception& e) {
    internalLog->error("INTERNAL FS ERROR: fgetattr %s", e.what());
    return -1;
  }
  return 0;
}
int CloudfsInternal::lock(const char* path, struct fuse_file_info* fileInfo,
                          int cmd, struct flock* lock) {
  try {
    return getCloudFs()->lock(std::string(path),
                              std::shared_ptr<fuse_file_info>(fileInfo), cmd,
                              std::shared_ptr<struct flock>(lock));
  } catch (std::exception& e) {
    internalLog->error("INTERNAL FS ERROR: flock %s", e.what());
    return -1;
  }
  return 0;
}
int CloudfsInternal::utimens(const char* path, const struct timespec* tv) {
  try {
    return getCloudFs()->utimens(std::string(path),
                                 std::make_pair(tv[0], tv[1]));
  } catch (std::exception& e) {
    internalLog->error("INTERNAL FS ERROR: utimens %s", e.what());
    return -1;
  }
  return 0;
}
int CloudfsInternal::ioctl(const char* path, int cmd, void* arg,
                           struct fuse_file_info* fileInfo, unsigned int flags,
                           void* data) {
  try {
    return getCloudFs()->ioctl(std::string(path), cmd,
                               std::shared_ptr<char>((char*)arg),
                               std::shared_ptr<fuse_file_info>(fileInfo), flags,
                               std::shared_ptr<char>((char*)data));
  } catch (std::exception& e) {
    internalLog->error("INTERNAL FS ERROR: ioctl %s", e.what());
    return -1;
  }
  return 0;
}
int CloudfsInternal::poll(const char* path, struct fuse_file_info* fileInfo,
                          fuse_pollhandle* ph, unsigned int* reventsp) {
  try {
    return getCloudFs()->poll(
        std::string(path), std::shared_ptr<fuse_file_info>(fileInfo),
        std::shared_ptr<fuse_pollhandle>(ph, [](fuse_pollhandle *ph) {
          if (ph != nullptr) {
            delete ph;
          }
        }), reventsp);
  } catch (std::exception& e) {
    internalLog->error("INTERNAL FS ERROR: poll %s", e.what());
    return -1;
  }
  return 0;
}
int CloudfsInternal::write_buf(const char* path, fuse_bufvec* buf, off_t off,
                               fuse_file_info* fileInfo) {
  try {
    return getCloudFs()->write_buf(std::string(path),
                                   std::shared_ptr<fuse_bufvec>(buf), off,
                                   std::shared_ptr<fuse_file_info>(fileInfo));
  } catch (std::exception& e) {
    internalLog->error("INTERNAL FS ERROR: write_buf %s", e.what());
    return -1;
  }
  return 0;
}
int CloudfsInternal::read_buf(const char* path, fuse_bufvec** bufp, size_t size,
                              off_t off, fuse_file_info* fileInfo) {
  try {
    return getCloudFs()->read_buf(
        std::string(path),
        *(std::shared_ptr<std::shared_ptr<fuse_bufvec>>(
              new std::shared_ptr<fuse_bufvec>(*bufp))
              .get()),
        size, off, std::shared_ptr<fuse_file_info>(fileInfo));
  } catch (std::exception& e) {
    internalLog->error("INTERNAL FS ERROR: read_buf %s", e.what());
    return -1;
  }
  return 0;
}
int CloudfsInternal::flock(const char* path, fuse_file_info* fileInfo, int op) {
  try {
    return getCloudFs()->flock(std::string(path),
                               std::shared_ptr<fuse_file_info>(fileInfo), op);
  } catch (std::exception& e) {
    internalLog->error("INTERNAL FS ERROR: flock %s", e.what());
    return -1;
  }
  return 0;
}
int CloudfsInternal::fallocate(const char* path, int mode, off_t offset,
                               off_t length, fuse_file_info* fileInfo) {
  try {
    return getCloudFs()->fallocate(std::string(path), mode, offset, length,
                                   std::shared_ptr<fuse_file_info>(fileInfo));
  } catch (std::exception& e) {
    internalLog->error("INTERNAL FS ERROR: fallocate %s", e.what());
    return -1;
  }
  return 0;
}
