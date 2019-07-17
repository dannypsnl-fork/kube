#include "lib/kube.hpp"
#include <gtest/gtest.h>

using kube::Config;
using kube::Namespace;
using kube::resource::Pod;

TEST(Kube, Cluster) {
  auto cluster =
      kube::attach_cluster(Config::Path("/Users/dannypsnl/.kube/config"));
  // while using get, must assign a namespace, expected fail since no pod name
  // nginx here
  EXPECT_THROW(cluster.get<Pod>(Namespace("default"), "nginx"),
               kube::KubeWrapException);
  // should not throw exception here since list all namespaces is valid
  cluster.list<Pod>(Namespace::All);
}

TEST(Kube, InCluster) {
  EXPECT_THROW(kube::attach_cluster(Config::InCluster),
               kube::KubeWrapException);
}