//
// Created by Sushrut Shringarputale on 2019-08-04.
//

#ifndef CLOUDFS_SYS_UTILS_H
#define CLOUDFS_SYS_UTILS_H

#include <cstdlib>
#include <zconf.h>

namespace cloudfs {
  enum SystemVariable {
    MAX_PASSWORD_SIZE = _SC_GETPW_R_SIZE_MAX,
    MAX_GROUPNAME_SIZE = _SC_GETGR_R_SIZE_MAX
  };

  class SysUtils {
  public:
    static size_t getSystemVariable(SystemVariable var);

  private:
    static size_t getSystemVariableInternal(SystemVariable var);
  };
  //REGISTER_LOGGER(SysUtils);
}
#endif //CLOUDFS_SYS_UTILS_H
