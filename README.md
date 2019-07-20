# kube

A kubernetes C++ client

### Dependencies

#### Binary

- pkg-config: https://linux.die.net/man/1/pkg-config
- protoc
- bazel
- make
- clang

#### Library

- lib protobuf: for `pkg-config --cflags --libs protobuf`

### Using

Put the `./lib/`(run `make build` to get) directory into search path and write:
```
#include "kube.hpp"
```

Then you have to link object file `./bazel-bin/lib/libkube.so` into your binary.

### Develop

test: `make test`
build: `make build`

To ensure you format the code correctly using `make format` before you commit.
We use `clang-format` as standard.
