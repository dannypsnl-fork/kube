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
  std::string host;
  std::string bearer_token;

  config() {}
  config(std::string host, std::string bearer_token)
      : host(host), bearer_token(bearer_token) {}
};
bool operator==(config l_cfg, config r_cfg) {
  if (l_cfg.host == r_cfg.host) {
    return true;
  } else {
    return false;
  }
}

struct Config {
  virtual config get_config() = 0;
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

struct Config::Path {
  Path() = delete;
  Path(std::string path) : _path(path) {}
  config get_config() {
    // TODO: complete the config get by path
    return config();
  }

 private:
  std::string _path;
};

bool is_empty(const char *c_str) {
  return c_str == nullptr || std::char_traits<char>::length(c_str);
}

struct Config::inCluster {
  config get_config() {
    char *host = std::getenv("KUBERNETES_SERVICE_HOST");
    char *port = std::getenv("KUBERNETES_SERVICE_PORT");
    if (is_empty(host) || is_empty(port)) {
      throw NotInCluster();
    }
    std::ifstream token_file{
        "/var/run/secrets/kubernetes.io/serviceaccount/token"};
    std::string token{(std::istreambuf_iterator<char>(token_file)),
                      (std::istreambuf_iterator<char>())};
    // TODO: read ca file
    std::ifstream root_ca_file{
        "/var/run/secrets/kubernetes.io/serviceaccount/ca.crt"};
    return config(host, token);
  }
};
// must initialize InCluster after Config::inCluster is completed.
Config::inCluster Config::InCluster{};

}  // namespace kube

#endif
