//
// Created by Sushrut Shringarputale on 2019-08-17.
//
#include <math.h>
#include <thread>
#include <utility>
#include "retry.h"
const int64_t max_backoff_milliseconds = 100000;

template <typename Predicate, typename Callable, typename... Args,
          // figure out what the callable returns
          typename R = std::decay_t<std::invoke_result_t<Callable&, Args...>>,
          // require that Predicate is actually a Predicate
          std::enable_if_t<
              std::is_convertible_v<std::invoke_result_t<Predicate&, R>, bool>,
              int> = 0>
R Retry(int max_retry_count, int64_t initial_delay_ms, Predicate&& isRetriable,
        Callable&& callable, Args&&... args) {
  int retry_count = 0;
  while (true) {
    auto status = callable(std::forward<Args...>(args)...);
    if (!IsRetriable(status)) {
      return status;
    }

    if (retry_count >= max_retry_count) {
      // Return status and abort retry
      return status;
    }
    int64_t delay_milliseconds = 0;
    if (initial_delay_ms > 0) {
      delay_milliseconds =
          std::min(initial_delay_ms << retry_count, max_backoff_milliseconds);
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(delay_milliseconds));
    retry_count++;
  }
}