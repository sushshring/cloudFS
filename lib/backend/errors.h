//
// Created by Sushrut Shringarputale on 2019-08-17.
//

#ifndef CLOUDFS_LIB_BACKEND_ERRORS_H_
#define CLOUDFS_LIB_BACKEND_ERRORS_H_
namespace cloudfs::backend {
class BaseException :
    public std::exception {
 private:
  bool isRetriable;
 protected:
  BaseException(const bool isRetriable)
      : isRetriable(isRetriable) {};
};
class NoSuchObjectException :
    public BaseException {
 public:
  NoSuchObjectException(const std::string &path)
      : path(path),
        BaseException(false) {}
};

class AuthorizationException :
    public BaseException {

};
}
#endif //CLOUDFS_LIB_BACKEND_ERRORS_H_
