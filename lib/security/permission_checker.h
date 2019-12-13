//
// Created by Sushrut Shringarputale on 2019-08-03.
//

#ifndef CLOUDFS_PERMISSION_CHECKER_H
#define CLOUDFS_PERMISSION_CHECKER_H

#include <fuse/fuse.h>
#include <yaml-cpp/yaml.h>
#include "factory.h"
#include "cloudfs_types/cloud_metadata_accessor.h"

namespace cloudfs {
  class PermissionChecker {
  public:
    explicit PermissionChecker(YAML::Node &config);

    /**
     * Recursively checks the directory structure for permission with the provided flags.
     * @param path: path of file to check permissions for
     * @param flags: bitmask of flags to request file permissions for
     * @return 0 is success, -errno is failed
     */
    int checkObjectPermissionRecursive(const std::string &path, int flags) const;

  protected:
    std::string getUsername(uid_t uid) const;

    int checkUidInGroup(uid_t uid, gid_t gid) const;

    int checkParentObjectAccess(const std::string &path, int mask) const;

    int checkObjectAccess(const std::string &path, int mask) const;

  private:
    std::shared_ptr<CloudMetadataAccessor> mtdAccessor;

    static int accessorUid;
    static int accessorGid;
    static bool isAccessor;
    static bool isAccessorGroup;
  };
}  // namespace cloudfs
#endif  // CLOUDFS_PERMISSION_CHECKER_H
