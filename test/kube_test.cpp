#include "lib/kube.hpp"
#include <gtest/gtest.h>

using kube::Config;
using kube::Namespace;
using kube::resource::Pod;

TEST(Kube, Cluster) {
  auto cluster = kube::attach_cluster(Config::Path("~/.kube/config"));

  cluster.get<Pod>(Namespace::All, "nginx");
}

TEST(Kube, InCluster) {
  auto cluster = kube::attach_cluster(Config::InCluster);

  cluster.list<Pod>(Namespace("kube-system"));
}