
#ifndef KUBE_HEADER_CLUSTER
#define KUBE_HEADER_CLUSTER

#include <vector>
#include "_config.hpp"

namespace kube {

struct Namespace {
  static Namespace All;
};

class Cluster {
  config cfg;

 public:
  Cluster(config cfg) : cfg(cfg) {}

  template <typename Resource>
  Resource get(Namespace ns) {
    return Resource();
  }
  template <typename Resource>
  std::vector<Resource> list(Namespace ns) {
    return std::vector<Resource>();
  }
};

Cluster attach_cluster(const Config& cfg) {
  auto config = cfg.get_config();
  return Cluster(config);
}

}  // namespace kube

#endif
