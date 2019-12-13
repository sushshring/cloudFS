//
// Created by Sushrut Shringarputale on 2019-08-02.
//

#include "cloudfs.h"
#include "cloudfs_types/cloud_interface.h"
#include "security/permission_checker.h"
#include <iostream>
using namespace cloudfs;

int Cloudfs::open(const std::string& path,
                  std::shared_ptr<struct fuse_file_info> file_info) {
  // int result;
  // // std::shared_ptr<struct stat> st = std::make_shared<struct stat>();
  // bool needsFlush = false;

  // GET_LOGGER(this)->debug("[path=%s][flags=0x%x]", path, file_info->flags);
  // // if (statsCache->exists(path)) {
  // //   if ((result = flush(path, file_info))) {
  // //     GET_LOGGER(this)->error("Could not flush file: %s, result: %d", path,
  // //                             result);
  // //   }
  // //   statsCache->remove(path);
  // // }
  // auto fd = cloudInterface->open(path, needsFlush, 0);
  // file_info->fh = fd->fd();
  std::basic_fstream::open(path, file_info->flags);
  return 0;
}

std::unique_ptr<Cloudfs>& cloudfs::Cloudfs::instance(
    std::optional<YAML::Node> config) {
  if (!theCloudFs) {
    if (!config) {
      throw std::runtime_error("CMD args must be provided for instantiation");
    }
    theCloudFs = GET_UNIQUE(Cloudfs, *config);
    //    theCloudFs =
    //    std::move(std::get<std::unique_ptr<Cloudfs>>(Factory<Cloudfs>::create(*config)));
  }
  return theCloudFs;
}

/**
 * -d   -o debug          enable debug output (implies -f)\n"
   -f                     foreground operation\n"
   -s                     disable multi-threaded operation\n"
   -o clone_fd            use separate fuse device fd for each thread\n"
                           (may improve performance)\n"
   -o max_idle_threads    the maximum number of idle worker threads\n"
                           allowed (default: 10)\n");
 * @param node
 * @return
 */
cloudfs_internal::CloudfsArgs Cloudfs::getCloudFsArgs(YAML::Node& node) {
  auto args = std::vector<std::string>();
  if (node["-d"].IsDefined()) {
    args.push_back("-d");
    args.push_back(node["-d"].as<std::string>());
  }
  if (node["-f"].IsDefined()) {
    args.push_back("-f");
    args.push_back(node["-f"].as<std::string>());
  }
  if (node["-s"].IsDefined()) {
    args.push_back("-s");
    args.push_back(node["-s"].as<std::string>());
  }
  if (node["-o"].IsDefined()) {
    args.push_back("-o");
    args.push_back(node["-o"].as<std::string>());
  }
  return cloudfs_internal::CloudfsArgs(args);
}

Cloudfs::Cloudfs(YAML::Node& config)
    : cloudInterface(GET_SHARED(CloudInterface, config["interface"])),
      statsCache(GET_SHARED(StatsCache, config["statsCache"])),
      //    permissionChecker(
      //      Factory<PermissionChecker>::create(config["permissionChecker"])),
      internalFsLayer(cloudfs_internal::CloudfsInternal::initialize(
          std::move(getCloudFsArgs(config)))) {}
