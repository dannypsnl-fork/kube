#include "lib/kube.hpp"
#include <gtest/gtest.h>

using kube::Config;
using kube::Namespace;
using kube::NotInCluster;
using kube::resource::Pod;

TEST(Kube, Config) {
  ASSERT_THROW(Config::InCluster.get_config(), NotInCluster);
}

TEST(Kube, Cluster) {
  auto cluster = kube::attach_cluster(Config::Path("~/.kube/config"));

  cluster.get<Pod>(Namespace::All);
  cluster.list<Pod>(Namespace::All);
}
