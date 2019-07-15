#ifndef KUBE_HEADER_CONFIG
#define KUBE_HEADER_CONFIG

#include <string>

namespace kube {

struct Config {
  // Define the tagged union types
  // type Config =
  //   Path(std::String)
  //   | InCluster
  struct Path;
  struct inCluster;

 public:
  static inCluster InCluster;
};

struct Config::Path {
  Path() = delete;
  Path(std::string path) : path(path) {}
  std::string path;
};

struct Config::inCluster {};
// must initialize InCluster after Config::inCluster is completed.
Config::inCluster Config::InCluster{};

}  // namespace kube

#endif
