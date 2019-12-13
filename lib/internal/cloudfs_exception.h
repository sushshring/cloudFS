//
// Created by Sushrut Shringarputale on 2019-08-03.
//

#ifndef CLOUDFS_CLOUDFS_EXCEPTION_H
#define CLOUDFS_CLOUDFS_EXCEPTION_H
#include <string>
#include <stdexcept>
class CloudfsException : std::runtime_error {
 public:
  [[nodiscard]] const char* what() const noexcept override;

  CloudfsException(int err, std::string message);

 private:
  int err;
};

#endif  // CLOUDFS_CLOUDFS_EXCEPTION_H
