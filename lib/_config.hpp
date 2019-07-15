#ifndef KUBE_HEADER_CONFIG
#define KUBE_HEADER_CONFIG

#include <cstdlib>
#include <exception>
#include <fstream>
#include <iostream>
#include <string>

namespace kube {

struct NotInCluster : public std::exception {
  const char *what() const throw() { return "not in cluster"; }
};

struct config {
  // in cluster
  bool in_cluster = false;
  // or has a path
  std::string path{""};

  config(bool in_cluster) : in_cluster(in_cluster) {}
  config(std::string path) : path(path) {}
};

struct Config {
  virtual config get_config() const = 0;
  virtual ~Config() {}

  // Define the tagged union types
  // type Config =
  //   Path(std::String)
  //   | InCluster
  struct Path;

 private:
  struct inCluster;

 public:
  static inCluster InCluster;
};

struct Config::Path : public Config {
  Path() = delete;
  Path(std::string path) : _path(path) {}
  config get_config() const { return config(_path); }

 private:
  std::string _path;
};

bool is_empty(const char *c_str) {
  return c_str == nullptr || std::char_traits<char>::length(c_str);
}

struct Config::inCluster : public Config {
  config get_config() const {
    char *host = std::getenv("KUBERNETES_SERVICE_HOST");
    char *port = std::getenv("KUBERNETES_SERVICE_PORT");
    if (is_empty(host) || is_empty(port)) {
      throw NotInCluster();
    }
    return config(true);
  }
};
// must initialize InCluster after Config::inCluster is completed.
Config::inCluster Config::InCluster{};

}  // namespace kube

#endif
