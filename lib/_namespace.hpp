#ifndef KUBE_HEADER_NAMESPACE
#define KUBE_HEADER_NAMESPACE

#include <string>

namespace kube {

class Namespace {
  struct all;

  std::string _namespace;

 public:
  // usage:
  // Namespace::All
  static all All;
  // for Namespace::all
  Namespace();
  // usage:
  // Namespace("kube-system")
  Namespace(std::string ns);

  std::string get_namespace();
};
struct Namespace::all : public Namespace {
  all();
  std::string get_namespace();
};

}  // namespace kube

#endif