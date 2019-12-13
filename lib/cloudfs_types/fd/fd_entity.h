//
// Created by Sushrut Shringarputale on 2019-08-05.
//

#ifndef CLOUDFS_FD_ENTITY_H
#define CLOUDFS_FD_ENTITY_H
#include <memory>
#include <string>
#include "backend/backend.h"
namespace cloudfs {
class FdManager;

class FdEntity : std::enable_shared_from_this<FdEntity>,
                 std::shared_ptr<FdEntity> {
 public:
  FdEntity(const std::string& path, const std::string& cachePath)
      : _path(path), _cachePath(cachePath){};

  int rowFlush(const std::string& path, bool forceSync);

  /**
   * Returns the file system associatable file descriptor for this entity
   */
  int fd();

  std::string path();

  bool isOpen();

  void duplicate();

  friend class FdManagerImpl;

 private:
  std::string _path;
  std::string _cachePath;

  int open(const off_t size, const time_t time, bool dontWaitOnFdLock);

  int close();
};
}  // namespace cloudfs
#endif  // CLOUDFS_FD_ENTITY_H
