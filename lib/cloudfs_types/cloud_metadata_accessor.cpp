//
// Created by Sushrut Shringarputale on 2019-08-03.
//

#include <memory>
#include "cloud_metadata_accessor.h"
#include "factory.h"
using namespace cloudfs;

CloudMetadataAccessor::CloudMetadataAccessor(const YAML::Node& config)
    : statsCache(GET_SHARED(StatsCache, config["statsCache"])) {}

std::shared_ptr<struct stat> cloudfs::CloudMetadataAccessor::getMetadata(
    const std::string& path) {
  return nullptr;
}
