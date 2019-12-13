//
// Created by Sushrut Shringarputale on 2019-08-03.
//

#include "cloud_interface.h"
#include "cloudfs.h"
#include "security/permission_checker.h"
using namespace cloudfs;

CloudInterface::CloudInterface(YAML::Node& config)
    : permissionChecker(GET_SHARED(PermissionChecker, config["fdmanager"])),
      metadataAccessor(
          GET_SHARED(CloudMetadataAccessor, config["metadataAccessor"])),
      statsCache(GET_SHARED(StatsCache, config["statsCache"])),
      fdManager(GET_SHARED(FdManager, config["fdManager"])) {}

std::shared_ptr<cloudfs::FdEntity> cloudfs::CloudInterface::open(
    std::string path, bool needsFlush, int flags) {
  int result;
  result = permissionChecker->checkObjectPermissionRecursive(path, flags);
  if (result != 0) {
    errno = result;
    return nullptr;
  }
  auto stat = metadataAccessor->getMetadata(path);
  if ((unsigned int)flags & O_TRUNC && 0 != stat->st_size) {
    stat->st_size = 0;
    needsFlush = true;
  }
  // If not regular file, don't set access time
  if (!S_ISREG(stat->st_mode) || S_ISLNK(stat->st_mode)) {
    stat->st_mtime = -1;
  }
  std::shared_ptr<FdEntity> entity;

  // get_object_attribute(path, NULL, &meta, true, NULL, true);    // no
  // truncate cache
  if (nullptr == (entity = fdManager->open(path, stat->st_size, stat->st_mtime,
                                           false, true, false))) {
    statsCache->remove(path);
    errno = -EIO;
    return nullptr;
  }

  if (needsFlush) {
    if (0 != entity->rowFlush(path, true)) {
      GET_LOGGER(this)->error("could not upload file(%s): result=%d", path,
                              result);
      fdManager->close(entity);
      statsCache->remove(path);
      return nullptr;
    }
  }
  return entity;
}
