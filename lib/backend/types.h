#ifndef CLOUDFS_LIB_BACKEND_TYPES_H_
#define CLOUDFS_LIB_BACKEND_TYPES_H_
#include "../common/types.h"
namespace cloudfs::backend {
template <class ResultType>
class BackendResult : public common::Result<ResultType> {
 public:
  /**
   * Return true if the given result indicates a temporary problem. Most
   * instance methods automatically retry the request in this case, so the
   * caller does not need to worry about temporary failures.
   * However, in same cases (e.g. when reading or writing an object), the
   * request cannot automatically be retried. In these case this method can
   * be used to check for temporary problems and so that the request can be
   * manually restarted if applicable.
   * @tparam ResultType
   * @param result the result to check for temporary problem
   * @return BackendResult<bool> whether or not the problem is temporary
   */
  BackendResult<bool> isTempFailure();
};

struct FileMetadata {};
}  // namespace cloudfs::backend

#endif