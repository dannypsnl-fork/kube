#ifndef KUBE_HEADER_RESOURCE
#define KUBE_HEADER_RESOURCE

#include <sstream>
#include "k8s.io/apiserver/pkg/apis/example/v1/generated.pb.h"

namespace kube {
namespace resource {

using kube_pod = k8s::io::apiserver::pkg::apis::example::v1::Pod;

struct Pod {
  static const char *resource_type() { return "pods"; }

  void from_c_string(const char * input) {
    std::stringstream ss{input};
    _resource.ParseFromIstream(&ss);
  }

  private:
  kube_pod _resource;
};

}  // namespace resource
}  // namespace kube

#endif
