//
// Created by Sushrut Shringarputale on 2019-08-03.
//

#ifndef CLOUDFS_FACTORY_H
#define CLOUDFS_FACTORY_H

#include <yaml-cpp/yaml.h>
#include <memory>
#include <string>
#include <unordered_map>
#include <variant>
#include "log_config.h"

namespace cloudfs {
template <class T>
using NamedInstanceType = std::variant<std::shared_ptr<T>, std::unique_ptr<T>>;

template <class T>
class Factory {
 private:
  // ensure static data is properly initialized before use
  static std::unordered_map<std::string, NamedInstanceType<T>>&
  nameInstanceMap();

  static std::unordered_map<std::string, std::shared_ptr<Factory<T>>>
  typeFactoryMap();

  virtual std::shared_ptr<T> createObject(YAML::Node& config) = 0;

  virtual std::unique_ptr<T> createUniqueObject(YAML::Node& config) = 0;

  static std::unique_ptr<T> createUnique(const YAML::Node& config);

  static std::shared_ptr<T> createShared(const YAML::Node& config);

 protected:
  // register a type name to the corresponding factory
  static void registerType(std::string type,
                           std::shared_ptr<Factory<T>> factory);

 public:
  Factory() = default;

  virtual ~Factory() = default;

  // create an object
  static NamedInstanceType<T> create(const YAML::Node& config);

  // destroy an object (if it exists)
  static void destroy(std::string name);

  // lookup an object by its name (set in the json)
  static NamedInstanceType<T> getObjectByName(std::string name);

  // get all objects
  static std::unordered_map<std::string, NamedInstanceType<T>>& getObjects();
};

// class for registering classes with factory
template <class T, class ClassType>
class ClassFactory
    : public Factory<T>,
      public std::enable_shared_from_this<ClassFactory<T, ClassType>> {
 private:
  virtual std::shared_ptr<T> createObject(YAML::Node& config);

  virtual std::unique_ptr<T> createUniqueObject(YAML::Node& config);

 public:
  explicit ClassFactory(std::string type);

  virtual ~ClassFactory() = default;
};

// macro for registering classes with factory
#define REGISTER_CLASS(BaseType, ClassType)                              \
  static ClassFactory<BaseType, ClassType> BaseType##ClassType##Factory( \
      #ClassType);

#define GET_SHARED(T, config) \
  std::get<std::shared_ptr<T>>(Factory<T>::create(config))

#define GET_UNIQUE(T, config) \
  std::move(std::get<std::unique_ptr<T>>(Factory<T>::create(config)))

}  // namespace cloudfs
#endif  // CLOUDFS_FACTORY_H
