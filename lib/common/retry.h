//
// Created by Sushrut Shringarputale on 2019-08-17.
//

#ifndef CLOUDFS_LIB_COMMON_RETRY_H_
#define CLOUDFS_LIB_COMMON_RETRY_H_
#include <cstdlib>
#include <type_traits>
#include "result.h"
namespace utils {
template <typename Predicate, typename Callable, typename... Args,
          // figure out what the callable returns
          typename R = std::decay_t<std::invoke_result_t<Callable&, Args...>>,
          // require that Predicate is actually a Predicate
          std::enable_if_t<
              std::is_convertible_v<std::invoke_result_t<Predicate&, R>, bool>,
              int> = 0>
R Retry(int max_retry_count, int64_t initial_delay_ms, Predicate&& isRetriable,
        Callable&& callable, Args&&... args);

template <typename Predicate, typename Callable, typename... Args,
          // figure out what the callable returns
          typename R = std::decay_t<std::invoke_result_t<Callable&, Args...>>,
          // require that Predicate is actually a Predicate
          std::enable_if_t<
              std::is_convertible_v<std::invoke_result_t<Predicate&, R>, bool>,
              int> = 0>
R Retry(Predicate&& isRetriable, Callable&& callable, Args&&... args) {
  Retry(5, 100, isRetriable, callable, args...);
}
}  // namespace utils
#endif  // CLOUDFS_LIB_COMMON_RETRY_H_
