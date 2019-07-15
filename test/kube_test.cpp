#include "lib/kube.hpp"
#include <gtest/gtest.h>

using kube::Config;
using kube::NotInCluster;

TEST(Kube, Config) {
  ASSERT_THROW(Config::InCluster.get_config(), NotInCluster);
}

TEST(Kube, Cluster) {
  auto cluster = kube::attach_cluster(Config::Path("~/.kube/config"));
}
