//
// Created by Sushrut Shringarputale on 2019-08-04.
//

#ifndef CLOUDFS_STR_UTILS_H
#define CLOUDFS_STR_UTILS_H
namespace cloudfs {
  std::shared_ptr<char> copyStrToCPtr(const std::string &inString);
}
#endif //CLOUDFS_STR_UTILS_H
