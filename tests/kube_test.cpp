#include "kube.hpp"
#include <gtest/gtest.h>

TEST(Kube, Config) {
  ASSERT_THROW(Config::InCluster.get_config(), NotInCluster);
}
