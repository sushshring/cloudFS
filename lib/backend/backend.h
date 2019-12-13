//
// Created by Sushrut Shringarputale on 2019-08-17.
//

#ifndef CLOUDFS_LIB_BACKEND_BACKEND_H_
#define CLOUDFS_LIB_BACKEND_BACKEND_H_
#include <yaml-cpp/yaml.h>
#include <iterator>
#include <string>
#include "backendfh.h"
#include "common/types.h"
#include "types.h"

namespace cloudfs::backend {
class Backend {
 public:
  virtual ~Backend() = default;

  // Public methods
  /**
   * Read object data from path, retry on temporary failure
      Open object for reading, read data, and close object. If a temporary
      error (as defined by `is_temp_failure`) occurs during opening, closing
      or execution of read, the operation is retried.
   * @param path
   * @return
   */
  BackendResult<bytebuffer> performRead(const std::string& path);

  /**
   *  Open object for writing, write the object, and close object. If a
   temporary error (as defined by `is_temp_failure`) occurs during opening,
   closing or execution of write, the operation is retried.
   * @param path
   * @param buf
   * @param metadata
   * @param isCompressed
   * @return
   */
  BackendResult<void> performWrite(
      const std::string& path, const bytebuffer buf,
      const std::shared_ptr<FileMetadata>& metadata = nullptr,
      const bool isCompressed = false);

 protected:
  Backend(const YAML::Node& /*config*/) {}

  // Abstract methods

  /**
   * Return metadata for given path.
   * If the path does not exist, `NoSuchObject` is raised.
   */
  virtual BackendResult<FileMetadata> lookup(const std::string& path) = 0;

  /**
   * Get the size for the file stored at path
   * @param path
   */
  virtual BackendResult<size_t> getSize(const std::string& path) = 0;

  /**
   * Open object for writing
   * `metadata` can be mapping with additional attributes to store with the
   * object. Keys have to be of type `str`, values have to be of elementary
   * type (`str`, `bytes`, `int`, `float` or `bool`).
   * Returns a file- like object. The object must be closed closed
   * explicitly. After closing, the *get_obj_size* may be used to retrieve
   * the size of the stored object (which may differ from the size of the
   * written data).
   * The *is_compressed* parameter indicates that the caller is going to
   * write compressed data, and may be used to avoid recompression by the
   * backend.
   * @param path
   * @param metadata
   * @param isCompressed
   * @return
   */
  virtual BackendResult<BackendFileHandle> openWrite(
      const std::string& path,
      const std::shared_ptr<FileMetadata>& metadata = nullptr,
      const bool isCompressed = false) = 0;

  /**
   * Delete object stored at path
   * is *force* is true, do not return an error if the object does not exist
   */
  virtual BackendResult<void> deleteObject(const std::string& path,
                                           const bool force = false) = 0;

  /**
   * Returns an iterator to all the available keys at a path
   * @param path
   * @return
   */
  virtual BackendResult<std::iterator<std::input_iterator_tag, std::string, int,
                                      std::string*, std::string&>>
  list(const std::string& path) = 0;

  /**
   * Copy data stored under key `src` to key `dest`
   * If `dest` already exists, it will be overwritten. If *metadata* is
   * None` metadata will be copied from the source as well, otherwise
   * *metadata* becomes the metadata for the new object.
   * Copying will be done on the remote side without retrieving object data.
   * @param pathFrom
   * @param pathTo
   * @param metadata
   * @return
   */
  virtual BackendResult<void> copy(
      const std::string& pathFrom, const std::string& pathTo,
      const std::shared_ptr<FileMetadata>& metadata = nullptr) = 0;

  /**
   * Updates the metadata of object at path
   * @param path
   * @param metadata
   * @return
   */
  virtual BackendResult<void> updateMetadata(
      const std::string& path,
      const std::shared_ptr<FileMetadata>& metadata) = 0;
};
}  // namespace cloudfs::backend
#endif  // CLOUDFS_LIB_BACKEND_BACKEND_H_
