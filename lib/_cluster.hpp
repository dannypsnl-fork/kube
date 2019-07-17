#ifndef KUBE_HEADER_CLUSTER
#define KUBE_HEADER_CLUSTER

#include <cstdint>
#include <exception>
#include <string>
#include <vector>
#include "_config.hpp"
#include "_namespace.hpp"
#include "kube_wrap.h"

namespace kube {

// KubeWrapException wrapping the Go error message as C++ exception.
struct KubeWrapException : public std::exception {
  KubeWrapException(std::string message) : _message(message) {}
  const char* what() const throw() { return _message.c_str(); }

 private:
  std::string _message;
};

class Cluster {
  uintptr_t _clientset_id;

 public:
  // create by in cluster config
  Cluster(const Config::inCluster in_cluster);
  // create by config path
  Cluster(const Config::Path path);
  ~Cluster();

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
