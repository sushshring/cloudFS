//
// Created by Sushrut Shringarputale on 2019-08-17.
//

#ifndef CLOUDFS_LIB_COMMON_TYPES_H_
#define CLOUDFS_LIB_COMMON_TYPES_H_
#include <vector>
#include <future>
#include <cstddef>

typedef std::vector<std::byte> bytebuffer;

namespace cloudfs::common {
    template <class ResultType>
    class Result : public std::future<ResultType> {

    };
}
#endif //CLOUDFS_LIB_COMMON_TYPES_H_
