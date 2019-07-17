#include "_namespace.hpp"

using ::kube::Namespace;

Namespace::Namespace() {}
Namespace::Namespace(std::string ns) : _namespace(ns) {}
std::string Namespace::get_namespace() { return _namespace; };

Namespace::all::all() {}
std::string Namespace::all::get_namespace() { return ""; }

Namespace::all Namespace::All{};
