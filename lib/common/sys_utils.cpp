//
// Created by Sushrut Shringarputale on 2019-08-04.
//

#include <log_config.h>
#include "sys_utils.h"

size_t cloudfs::SysUtils::getSystemVariableInternal(cloudfs::SystemVariable var) {
  auto res = static_cast<size_t>(sysconf(var));
  if(0 > res){
    if (errno != 0){
      GET_LOGGER(SysUtils())->error("could not get max pw length");
    }
    res = 1024; // default initial length
  }
  return res;
}

size_t cloudfs::SysUtils::getSystemVariable(cloudfs::SystemVariable var) {
  static size_t maxPasswordSize, maxGroupNameSize;
  switch (var) {
    case MAX_PASSWORD_SIZE:
      if (!maxPasswordSize)
        maxPasswordSize = getSystemVariableInternal(MAX_PASSWORD_SIZE);
      return maxPasswordSize;
    case MAX_GROUPNAME_SIZE:
      if (!maxGroupNameSize)
        maxGroupNameSize = getSystemVariableInternal(MAX_GROUPNAME_SIZE);
      return maxGroupNameSize;
  }
}
