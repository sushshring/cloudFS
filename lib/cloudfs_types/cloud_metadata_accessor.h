//
// Created by Sushrut Shringarputale on 2019-08-03.
//

#ifndef CLOUDFS_CLOUD_METADATA_ACCESSOR_H
#define CLOUDFS_CLOUD_METADATA_ACCESSOR_H

#include <yaml-cpp/yaml.h>
#include <memory>
#include "cache/stats_cache.h"

namespace cloudfs {
class CloudMetadataAccessor {
 public:
  explicit CloudMetadataAccessor(const YAML::Node& config);

  virtual ~CloudMetadataAccessor() = default;

  std::shared_ptr<struct stat> getMetadata(const std::string& path);

 protected:
  virtual std::shared_ptr<struct stat> getMetadatafromCloud(
      const std::string& path) = 0;

 private:
  std::shared_ptr<StatsCache> statsCache;
};
}  // namespace cloudfs
#endif  // CLOUDFS_CLOUD_METADATA_ACCESSOR_H
