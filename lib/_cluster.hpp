
#ifndef KUBE_HEADER_CLUSTER
#define KUBE_HEADER_CLUSTER

#include <cstdint>
#include <exception>
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
    auto ns1 = const_cast<char*>(ns.get_namespace().c_str());
    auto name = const_cast<char*>(resource_name.c_str());
    auto r_type = const_cast<char*>(Resource::resource_type());
    auto result = get_resource(_clientset_id, ns1, r_type, name);
    if (result.r1 != nullptr) {
      throw KubeWrapException(result.r1);
    }
    // TODO: unmarshal result.r2 to get resource
    // for that, we have to generate C++ class via protobuffer file from
    // client-go(official kubernetes)
    return Resource();
  }
  template <typename Resource>
  std::vector<Resource> list(Namespace ns) {
    auto ns1 = const_cast<char*>(ns.get_namespace().c_str());
    auto r_type = const_cast<char*>(Resource::resource_type());
    auto result = list_resource(_clientset_id, ns1, r_type);
    if (result.r1 != nullptr) {
      throw KubeWrapException(result.r1);
    }
    // TODO: unmarshal result.r2 to get resource
    return std::vector<Resource>();
  }
};

template <typename T>
Cluster attach_cluster(T cfg) {
  return Cluster(cfg);
}

}  // namespace kube

#endif
