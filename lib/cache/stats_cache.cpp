//
// Created by Sushrut Shringarputale on 2019-08-03.
//
#include "stats_cache.h"
using namespace cloudfs;
REGISTER_CLASS(StatsCache, StatsCache);

void cloudfs::StatsCache::remove(const std::string &key) {
  auto it = _cache_items_map.find(key);
  if (it == _cache_items_map.end()) {
    throw std::range_error("There is no such key in cache");
  } else {
    _cache_items_list.erase(it->second);
    _cache_items_map.erase(it);
  }
}
