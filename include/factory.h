//
// Created by Sushrut Shringarputale on 2019-08-03.
//

#ifndef CLOUDFS_FACTORY_H
#define CLOUDFS_FACTORY_H
#include <yaml-cpp/yaml.h>
#include <memory>
#include <unordered_map>
#include "cloudfs.h"

namespace cloudfs {
template <class T>
class Factory {
 private:
  // ensure static data is properly initialized before use
  static std::unordered_map<std::string, std::shared_ptr<T>>& nameInstanceMap();
  static std::unordered_map<std::string, std::shared_ptr<Factory<T>>>&
  typeFactoryMap();

  virtual std::shared_ptr<T> createObject(YAML::Node& config) = 0;

 protected:
  // register a type name to the corresponding factory
  static void registerType(std::string type,
                           std::shared_ptr<Factory<T>> factory);

 public:
  Factory() = default;
  virtual ~Factory() = default;

  // create an object
  static std::shared_ptr<T> create(YAML::Node& config);

  static std::unique_ptr<T> &createSingleOwned(YAML::Node& config);

  // destroy an object (if it exists)
  static void destroy(std::string name);

  // lookup an object by its name (set in the json)
  static std::shared_ptr<T> getObjectByName(std::string name);

  // get all objects
  static const std::unordered_map<std::string, std::shared_ptr<T>>&
  getObjects();
};

// class for registering classes with factory
template <class T, class ClassType>
class ClassFactory : public Factory<T>, public std::enable_shared_from_this<ClassFactory<T, ClassType>> {
 private:
  virtual std::shared_ptr<T> createObject(YAML::Node& config);

  virtual std::unique_ptr<T>& createUniqueObject(YAML::Node& config);

 public:
  explicit ClassFactory(std::string type) {
    Factory<T>::registerType(type, this->shared_from_this());
  }
  virtual ~ClassFactory() = default;
};

// macro for registering classes with factory
#define REGISTER_CLASS(BaseType, ClassType)                              \
  static ClassFactory<BaseType, ClassType> BaseType##ClassType##Factory( \
      #ClassType);

}  // namespace cloudfs
#endif  // CLOUDFS_FACTORY_H
