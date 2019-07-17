
#ifndef KUBE_HEADER_CLUSTER
#define KUBE_HEADER_CLUSTER

#include <cstdint>
#include <exception>
#include <iostream>
#include <string>
#include <vector>
#include "_config.hpp"
#include "kube_wrap.h"

namespace kube {

struct KubeWrapException : public std::exception {
  KubeWrapException(std::string message) : _message(message) {}
  const char* what() const throw() { return _message.c_str(); }

 private:
  std::string _message;
};

struct Namespace {
  struct all;
  static all All;

  Namespace() {}
  Namespace(std::string ns) : _namespace(ns) {}

  std::string get_namespace() { return _namespace; };

 private:
  std::string _namespace;
};

struct Namespace::all : public Namespace {
  all() {}
  std::string get_namespace() { return ""; }
};
Namespace::all Namespace::All{};

class Cluster {
  uintptr_t _clientset_id;

 public:
  Cluster(const Config::inCluster in_cluster) {
    auto err = create_clientset_in_cluster(&_clientset_id);
    if (err != nullptr) {
      throw KubeWrapException(err);
    }
  }
  Cluster(const Config::Path path) {
    auto err = create_clientset_with_config_file(
        &_clientset_id, const_cast<char*>(path.path.c_str()));
    if (err != nullptr) {
      throw KubeWrapException(err);
    }
  }
  ~Cluster() {
    if (_clientset_id != 0) {
      delete_clientset(_clientset_id);
    }
  }

  template <typename Resource>
  Resource get(Namespace ns, std::string resource_name) {
    return Resource();
  }
  template <typename Resource>
  std::vector<Resource> list(Namespace ns) {
    return std::vector<Resource>();
  }
};

template <typename T>
Cluster attach_cluster(T cfg) {
  return Cluster(cfg);
}

}  // namespace kube

#endif
