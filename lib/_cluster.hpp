
#ifndef KUBE_HEADER_CLUSTER
#define KUBE_HEADER_CLUSTER

#include <memory>
#include "_config.hpp"

namespace kube {

class Cluster {};

std::unique_ptr<Cluster> attach_cluster(const Config& cfg) {
  auto config = cfg.get_config();
  // TODO: prepare cluster client data
  return std::make_unique<Cluster>();
}

}  // namespace kube

#endif
