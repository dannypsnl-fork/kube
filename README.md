# kube

A kubernetes C++ client

### Using

This is a one header library(at least for now).

Just put the `./bazel-kube/lib/`(run `make build` to get) directory into search path and write:
```
#include "kube.hpp"
```

### Develop

test: `make test`
build: `make build`

To ensure you format the code correctly using `make format` before you commit.
We use `clang-format` as standard.
