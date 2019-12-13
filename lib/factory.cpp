//
// Created by Sushrut Shringarputale on 2019-08-03.
//

#include <variant>
#include "factory.h"

using namespace cloudfs;

template <class T>
std::variant<std::shared_ptr<T>, std::unique_ptr<T>>
cloudfs::Factory<T>::create(const YAML::Node& config) {
  if (config["singleton"].IsDefined() && config["singleton"].as<bool>(false))
    return std::variant<std::unique_ptr<T>>(createUnique(config));
  return std::variant<std::shared_ptr<T>>(createShared(config));
}

template <class T>
std::shared_ptr<T> Factory<T>::createShared(const YAML::Node& config) {
  if (config.IsScalar()) {
    return std::get<std::shared_ptr<T>>(
        Factory<T>::getObjectByName(config.as<std::string>()));
  } else {
    // check all necessary properties are set
    assert(config["type"].IsDefined());
    assert(config["name"].IsDefined());
    // check type exists
    assert(Factory<T>::typeFactoryMap().count(
               config["type"].as<std::string>()) == 1);
    // create object
    std::shared_ptr<T> obj =
        Factory<T>::typeFactoryMap()[config["type"].as<std::string>()]
            ->createObject(config);
    // check object name still available
    assert(Factory<T>::nameInstanceMap().count(
               config["name"].as<std::string>()) == 0);
    // store in object map
    Factory<T>::nameInstanceMap()[config["name"].as<std::string>()] = obj;
    return obj;
  }
}

template <class T>
std::unique_ptr<T> Factory<T>::createUnique(const YAML::Node& config) {
  if (config.IsScalar()) {
    return std::get<std::unique_ptr<T>>(
        Factory<T>::getObjectByName(config.as<std::string>()));
  } else {
    // check all necessary properties are set
    assert(config["type"].IsDefined());
    // check type exists
    assert(Factory<T>::typeFactoryMap().count(
               config["type"].as<std::string>()) == 1);
    // create object
    std::unique_ptr<T> obj =
        Factory<T>::typeFactoryMap()[config["type"].as<std::string>()]
            ->createUniqueObject(config);
    // check object name still available
    assert(Factory<T>::nameInstanceMap().count(
               config["type"].as<std::string>()) == 0);
    // store in object map
    Factory<T>::nameInstanceMap()[config["type"].as<std::string>()] = obj;
    return obj;
  }
}

template <class T>
NamedInstanceType<T> cloudfs::Factory<T>::getObjectByName(std::string name) {
  return Factory<T>::nameInstanceMap()[name];
}

template <class T>
std::unordered_map<std::string, NamedInstanceType<T>>&
cloudfs::Factory<T>::getObjects() {
  return Factory<T>::nameInstanceMap();
}

template <class T>
void cloudfs::Factory<T>::destroy(std::string name) {
  // find object
  auto it = Factory<T>::nameInstanceMap().find(name);
  if (it != Factory<T>::nameInstanceMap().end()) {
    // delete object
    Factory<T>::nameInstanceMap().erase(it);
  }
}

template <class T>
std::unordered_map<std::string, std::shared_ptr<cloudfs::Factory<T>>>
cloudfs::Factory<T>::typeFactoryMap() {
  static auto data = std::make_shared<
      std::unordered_map<std::string, std::shared_ptr<Factory<T>>>>();
  return *data;
}

template <class T>
std::unordered_map<std::string, NamedInstanceType<T>>&
Factory<T>::nameInstanceMap() {
  static auto data =
      std::make_shared<std::unordered_map<std::string, NamedInstanceType<T>>>();
  return *data;
}

template <class T>
void Factory<T>::registerType(std::string type,
                              std::shared_ptr<Factory<T>> factory) {
  Factory<T>::typeFactoryMap()[type] = factory;
}

template <class T, class ClassType>
ClassFactory<T, ClassType>::ClassFactory(std::string type) {
  Factory<T>::registerType(type, this->shared_from_this());
  REGISTER_LOGGER(std::string(typeid(T).name()));
}

template <class T, class ClassType>
std::shared_ptr<T> ClassFactory<T, ClassType>::createObject(
    YAML::Node& config) {
  return std::make_shared<ClassType>(config);
}

template <class T, class ClassType>
std::unique_ptr<T> ClassFactory<T, ClassType>::createUniqueObject(
    YAML::Node& config) {
  return std::make_unique<ClassType>(config);
}