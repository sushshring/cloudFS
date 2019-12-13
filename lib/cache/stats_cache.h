//
// Created by Sushrut Shringarputale on 2019-08-03.
//

#ifndef CLOUDFS_STATS_CACHE_H
#define CLOUDFS_STATS_CACHE_H
#include <fuse/fuse.h>
#include <factory.h>
#include "lru_cache.h"
namespace cloudfs {
struct stats_cache_entry_t {
  std::shared_ptr<struct stat> stbuf;
  unsigned long hit_count;
  struct timespec cache_date {};
  //  headers_t         meta;
  bool isforce;
  bool noobjcache;  // Flag: cache is no object for no listing.
  unsigned long
      notruncate;  // 0<:   not remove automatically at checking truncate

  stats_cache_entry_t()
      : stbuf(std::make_shared<struct stat>()),
        hit_count(0),
        isforce(false),
        noobjcache(false),
        notruncate(0L) {
    cache_date.tv_sec = 0;
    cache_date.tv_nsec = 0;
  }
  bool operator==(const stats_cache_entry_t& rhs) const {
    return cache_date.tv_sec == rhs.cache_date.tv_sec &&
           cache_date.tv_nsec == rhs.cache_date.tv_nsec;
  }

  bool operator<(const stats_cache_entry_t& rhs) const {
    if (cache_date.tv_sec == rhs.cache_date.tv_sec)
      return cache_date.tv_nsec < rhs.cache_date.tv_nsec;
    else
      return cache_date.tv_sec < rhs.cache_date.tv_sec;
  }
};

 class StatsCache : public cache::lru_cache<std::string, stats_cache_entry_t> {
  public:
   explicit StatsCache(YAML::Node &config) : lru_cache(config["size"].as<size_t>()) {}

   void remove(const std::string &key);
 };
}  // namespace cloudfs
#endif  // CLOUDFS_STATS_CACHE_H
