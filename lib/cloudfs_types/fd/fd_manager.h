//
// Created by Sushrut Shringarputale on 2019-08-05.
//

#ifndef CLOUDFS_FD_MANAGER_H
#define CLOUDFS_FD_MANAGER_H

#include <yaml-cpp/yaml.h>
#include <set>
#include <unordered_map>
#include "factory.h"
#include "fd_entity.h"
#define NOCACHE_PATH_PREFIX_FORM " __CLOUDFS_UNEXISTED_PATH_"

namespace cloudfs {
class FdManager {
 public:
  virtual std::shared_ptr<FdEntity> open(const std::string& path, off_t size,
                                         time_t time, bool forceTemporaryFile,
                                         bool create,
                                         bool dontWaitOnFdLock) = 0;

  virtual bool close(const std::shared_ptr<FdEntity>& fdEntity) = 0;

  virtual ~FdManager() = default;

 protected:
  virtual const std::string getCachePath(const std::string& path,
                                         bool isMirrorPath) = 0;
};

typedef std::unordered_map<std::string, std::shared_ptr<FdEntity>> FdTable;

typedef std::multiset<std::shared_ptr<FdEntity>,
                      bool (*)(std::shared_ptr<FdEntity>,
                               std::shared_ptr<FdEntity>)>
    OpenTable;

class FdManagerImpl : public FdManager {
 public:
  explicit FdManagerImpl(YAML::Node& config)
      : entryTable(
            std::unordered_map<std::string, std::shared_ptr<FdEntity>>()),
        openTable(OpenTable(
            [](std::shared_ptr<FdEntity> lhs, std::shared_ptr<FdEntity> rhs) {
              return lhs->path() < rhs->path();
            })){};

  /**
   * This function sets up a file descriptor for the file system
   * and returns a handle to it
   * @param path Path to find where the file should be stored
   * @param size size of the file
   * @param time time of opening
   * @param forceTemporaryFile ????
   * @param create ??????
   * @param dontWaitOnFdLock ?????
   * @return std::shared_ptr<FdEntity>
   */
  std::shared_ptr<FdEntity> open(const std::string& path, off_t size,
                                 time_t time, bool forceTemporaryFile,
                                 bool create, bool dontWaitOnFdLock) override;

 private:
  std::mutex fdManagerLock;

  FdTable entryTable;

  OpenTable openTable;

  std::string cacheDir;

  static std::string makeRandomTempPath(const std::string& path);

  inline bool cacheDirExists() { return cacheDir.size() >= 0; }

  bool makeCachePath(const std::string& path, std::string& cachePath,
                     bool shouldMakeDir, bool isMirrorPath);
};
}  // namespace cloudfs

#endif  // CLOUDFS_FD_MANAGER_H
