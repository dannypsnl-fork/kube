all: build
.PHONY: test build format
test:
	@CC=clang bazel test test:kube-test --test_output=errors
build: lib/kube_wrap.a
	@CC=clang bazel build //lib:kube
format:
	@clang-format -i $(shell find lib | grep hpp)
	@clang-format -i $(shell find lib | grep cpp)
	@clang-format -i $(shell find test | grep cpp)

lib/kube_wrap.a: lib/main.go
	@go build -o ./lib/kube_wrap.a -buildmode=c-archive ./lib/main.go
