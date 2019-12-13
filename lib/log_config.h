//
// Created by Sushrut Shringarputale on 2019-08-03.
//

#ifndef CLOUDFS_LOG_CONFIG_H
#define CLOUDFS_LOG_CONFIG_H
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

#define REGISTER_LOGGER(ClassName) spdlog::stdout_color_mt(ClassName);

#define GET_LOGGER(Object) spdlog::get(typeid(Object).name())

#endif  // CLOUDFS_LOG_CONFIG_H
