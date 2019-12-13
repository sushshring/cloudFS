//
// Created by Sushrut Shringarputale on 2019-08-03.
//

#include <grp.h>
#include <libgen.h>
#include <pwd.h>
#include "cloudfs.h"
#include "cloudfs_types/cloud_interface.h"
#include "common/str_utils.h"
#include "common/sys_utils.h"
#include "factory.h"
#include "permission_checker.h"

using namespace cloudfs;
REGISTER_CLASS(PermissionChecker, PermissionChecker)

PermissionChecker::PermissionChecker(YAML::Node& config)
    : mtdAccessor(
          GET_SHARED(CloudMetadataAccessor, config["metadataAccessor"])) {}

int cloudfs::PermissionChecker::checkObjectAccess(const std::string& path,
                                                  int mask) const {
  // std::shared_ptr<struct stat> pst =
  //   ( pstbuf ? pstbuf : std::make_shared<struct stat>());

  GET_LOGGER(this)->debug("path=%s", path);
  std::shared_ptr<fuse_context> fuseContext =
      std::shared_ptr<fuse_context>(fuse_get_context());
  if (fuseContext == nullptr) {
    return -EIO;
  }
  auto stat = mtdAccessor->getMetadata(path);
  if (!stat) {
    return -ENOENT;
  }
  if (0 == fuseContext->uid) {
    // root is allowed all accessing.
    return 0;
  }
  if (isAccessor && accessorUid == fuseContext->uid) {
    // "uid" user is allowed all accessing.
    return 0;
  }
  if (F_OK == mask) {
    // if there is a file, always return allowed.
    return 0;
  }

  // for "uid", "gid" option
  uid_t obj_uid = (isAccessor ? accessorUid : stat->st_uid);
  gid_t obj_gid = (isAccessorGroup ? accessorGid : stat->st_gid);

  // compare file mode and uid/gid + mask.
  mode_t mode;
  mode_t base_mask = S_IRWXO;
  //  if (is_s3fs_umask) {
  //    // If umask is set, all object attributes set ~umask.
  //    mode = ((S_IRWXU | S_IRWXG | S_IRWXO) & ~s3fs_umask);
  //  } else {
  mode = stat->st_mode;
  //  }
  if (fuseContext->uid == obj_uid) {
    base_mask |= S_IRWXU;
  }
  if (fuseContext->gid == obj_gid) {
    base_mask |= S_IRWXG;
  }
  if (1 == checkUidInGroup(fuseContext->uid, obj_gid)) {
    base_mask |= S_IRWXG;
  }
  mode &= base_mask;

  if (X_OK == (mask & X_OK) && 0 == (mode & (S_IXUSR | S_IXGRP | S_IXOTH))) {
    return -EPERM;
  }
  if (W_OK == (mask & W_OK) && 0 == (mode & (S_IWUSR | S_IWGRP | S_IWOTH))) {
    return -EACCES;
  }
  if (R_OK == (mask & R_OK) && 0 == (mode & (S_IRUSR | S_IRGRP | S_IROTH))) {
    return -EACCES;
  }
  if (0 == mode) {
    return -EACCES;
  }
  return 0;
}

int cloudfs::PermissionChecker::checkParentObjectAccess(const std::string& path,
                                                        int mask) const {
  std::string parent;
  std::shared_ptr<char> buf;
  int result;

  GET_LOGGER(this)->debug("path=%s", path);

  if (path == "/" || path == ".") {
    // path is mount point.
    return 0;
  }
  if (X_OK == (mask & X_OK)) {
    for (parent = dirname(copyStrToCPtr(path).get()); !parent.empty();
         parent = dirname(copyStrToCPtr(parent).get())) {
      if (parent == ".") {
        parent = "/";
      }
      if (0 != (result = checkObjectAccess(parent, X_OK))) {
        return result;
      }
      if (parent == "/" || parent == ".") {
        break;
      }
    }
  }
  mask = (mask & ~X_OK);
  if (0 != mask) {
    parent = dirname(copyStrToCPtr(path).get());
    if (parent == ".") {
      parent = "/";
    }
    if (0 != (result = checkObjectAccess(parent, mask))) {
      return result;
    }
  }
  return 0;
}

std::string cloudfs::PermissionChecker::getUsername(uid_t uid) const {
  size_t maxlen =
      SysUtils::getSystemVariable(SystemVariable::MAX_PASSWORD_SIZE);
  int result;
  std::shared_ptr<char> pbuf;
  passwd pwinfo;
  passwd* ppwinfo = NULL;

  // make buffer
  pbuf = std::make_shared<char>(maxlen);
  // get pw information
  while (ERANGE ==
         (result = getpwuid_r(uid, &pwinfo, pbuf.get(), maxlen, &ppwinfo))) {
    maxlen *= 2;
    pbuf = std::make_shared<char>(maxlen);
  }

  if (0 != result) {
    GET_LOGGER(this)->error("Could not get pw information(%d).", result);
    return std::string("");
  }

  // check pw
  if (nullptr == ppwinfo) {
    return std::string("");
  }
  std::string name = std::string(ppwinfo->pw_name);
  return name;
}

int cloudfs::PermissionChecker::checkUidInGroup(uid_t uid, gid_t gid) const {
  size_t maxlen =
      SysUtils::getSystemVariable(SystemVariable::MAX_GROUPNAME_SIZE);
  int result;
  std::shared_ptr<char> pbuf;
  group ginfo{};
  group* pginfo = nullptr;

  // make buffer
  pbuf = std::make_shared<char>(maxlen);
  // get group information
  while (ERANGE ==
         (result = getgrgid_r(gid, &ginfo, pbuf.get(), maxlen, &pginfo))) {
    maxlen *= 2;
    pbuf = std::make_shared<char>(maxlen);
  }

  if (0 != result) {
    GET_LOGGER(this)->error("Could not get group information(%d).", result);
    return -result;
  }

  // check group
  if (nullptr == pginfo) {
    // there is not gid in group.
    return -EINVAL;
  }

  std::string username = getUsername(uid);

  char** ppgr_mem;
  for (ppgr_mem = pginfo->gr_mem; ppgr_mem && *ppgr_mem; ppgr_mem++) {
    if (username == *ppgr_mem) {
      // Found username in group.
      return 1;
    }
  }
  return 0;
}

int cloudfs::PermissionChecker::checkObjectPermissionRecursive(
    const std::string& path, int flags) const {
  int result;
  int mask = (O_RDONLY != (flags & O_ACCMODE) ? W_OK : R_OK);
  if ((result = this->checkParentObjectAccess(path, X_OK)) != 0) {
    return result;
  }
  result = this->checkObjectAccess(path, mask);
  if (0 != (-ENOENT == result
                ? (result = this->checkParentObjectAccess(path, W_OK))
                : result)) {
    return result;
  }
  return 0;
}