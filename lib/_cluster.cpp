#include "_cluster.hpp"

using ::kube::Cluster;

Cluster::Cluster(const Config::inCluster in_cluster) {
  auto err = create_clientset_in_cluster(&_clientset_id);
  if (err != nullptr) {
    throw KubeWrapException(err);
  }
}

Cluster::Cluster(const Config::Path path) {
  auto err = create_clientset_with_config_file(
      &_clientset_id, const_cast<char*>(path.path.c_str()));
  if (err != nullptr) {
    throw KubeWrapException(err);
  }
}

Cluster::~Cluster() {
  if (_clientset_id != 0) {
    // remove the clientset from map so that Go GC can delete it if we don't need it anymore(cluster object be deleted)
    delete_clientset(_clientset_id);
  }
}