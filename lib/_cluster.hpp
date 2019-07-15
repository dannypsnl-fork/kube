
#ifndef KUBE_HEADER_CLUSTER
#define KUBE_HEADER_CLUSTER

#include <string>
#include <vector>
#include "_config.hpp"

namespace kube {

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
 public:
  Cluster(const Config::inCluster in_cluster) {
    // TODO:
    // config, err := rest.InClusterConfig()
    // clientset, err := kubernetes.NewForConfig(config)
  }
  Cluster(const Config::Path path) {
    // TODO:
    // config, err := clientcmd.BuildConfigFromFlags("", *kubeconfig)
    // clientset, err := kubernetes.NewForConfig(config)
  }

  template <typename Resource>
  Resource get(Namespace ns) {
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
