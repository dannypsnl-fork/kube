# kube

A kubernetes C++ client

### Using

Put the `./lib/`(run `make build` to get) directory into search path and write:
```
#include "kube.hpp"
```

Then you have to link object file `./lib/kube_wrap.a` into your binary.

### Develop

test: `make test`
build: `make build`

To ensure you format the code correctly using `make format` before you commit.
We use `clang-format` as standard.
