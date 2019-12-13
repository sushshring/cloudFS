//
// Created by Sushrut Shringarputale on 2019-08-03.
//

#ifndef CLOUDFS_CLOUD_INTERFACE_H
#define CLOUDFS_CLOUD_INTERFACE_H

#include <fuse.h>
#include <yaml-cpp/yaml.h>
#include "cache/stats_cache.h"
#include "cloud_metadata_accessor.h"
#include "cloudfs_types/fd/fd_entity.h"
#include "cloudfs_types/fd/fd_manager.h"
#include "security/permission_checker.h"

namespace cloudfs {
class CloudInterface {
 public:
  explicit CloudInterface(YAML::Node& config);

  virtual ~CloudInterface() = default;

  virtual std::shared_ptr<cloudfs::FdEntity> open(std::string path,
                                                  bool needsFlush, int flags);

 private:
  std::shared_ptr<StatsCache> statsCache;

  std::shared_ptr<FdManager> fdManager;

  std::shared_ptr<PermissionChecker> permissionChecker;

  std::shared_ptr<CloudMetadataAccessor> metadataAccessor;
};
}  // namespace cloudfs
#endif  // CLOUDFS_CLOUD_INTERFACE_H
