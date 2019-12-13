//
// Created by Sushrut Shringarputale on 2019-08-03.
//

#include "cloudfs_exception.h"
#include <string>
const char* CloudfsException::what() const noexcept {
  return ("errno: " + std::to_string(this->err) +
          std::string(std::runtime_error::what()))
      .data();
}

CloudfsException::CloudfsException(int err, std::string message)
    : std::runtime_error(message), err(err) {}
