#include "lib/kube.hpp"
#include <gtest/gtest.h>
#include <iostream>

using kube::Config;
using kube::Namespace;
using kube::resource::Pod;

TEST(Kube, Cluster) {
  auto cluster =
      kube::attach_cluster(Config::Path("/Users/dannypsnl/.kube/config"));

  cluster.get<Pod>(Namespace::All, "nginx");
}

TEST(Kube, InCluster) {
  EXPECT_THROW(kube::attach_cluster(Config::InCluster),
               kube::KubeWrapException);
}