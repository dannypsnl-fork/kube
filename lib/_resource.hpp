#ifndef KUBE_HEADER_RESOURCE
#define KUBE_HEADER_RESOURCE

namespace kube {
namespace resource {

struct Pod {
  static const char *resource_type() { return "pods"; }
};

}  // namespace resource
}  // namespace kube

#endif
