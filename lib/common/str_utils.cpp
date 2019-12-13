//
// Created by Sushrut Shringarputale on 2019-08-04.
//

#include <libgen.h>
#include "log_config.h"
#include "security/permission_checker.h"
#include "common/str_utils.h"


std::shared_ptr<char> cloudfs::copyStrToCPtr(const std::string &inString) {
  auto charptr = std::shared_ptr<char>(new char[inString.size() + 1]);
  std::copy(inString.begin(), inString.end(), charptr.get());
  return charptr;
}
