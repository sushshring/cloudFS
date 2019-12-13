//
// Created by Sushrut Shringarputale on 2019-08-03.
//

#ifndef CLOUDFS_GOOGLE_CLOUD_FS_H
#define CLOUDFS_GOOGLE_CLOUD_FS_H
#include <google/cloud/storage/client.h>
#include <cerrno>
#include "cloudfs_types/cloud_interface.h"
#include "internal/cloudfs_exception.h"

namespace cloudfs {
namespace gcs = google::cloud::storage;
class GoogleCloudFs : public CloudInterface {
 public:
  explicit GoogleCloudFs(YAML::Node& config)
      : authCredPath(config["cred"].as<std::string>()),
        storageClient(nullptr),
        CloudInterface(config) {
    auto creds = gcs::oauth2::CreateServiceAccountCredentialsFromJsonFilePath(
        authCredPath);
    if (!creds) {
      GET_LOGGER(this)->error(
          "Failed to get Storage Client Credentials, status=%s",
          creds.status().message());
      throw CloudfsException(
          EAUTH, std::string("Failed to create Storage Client, status=") +
                     creds.status().message());
    }
    storageClient = std::make_shared<gcs::Client>(
        gcs::ClientOptions(*creds), gcs::StrictIdempotencyPolicy());
  }

 private:
  std::string authCredPath;
  std::shared_ptr<gcs::Client> storageClient;
};
}  // namespace cloudfs

#endif  // CLOUDFS_GOOGLE_CLOUD_FS_H
