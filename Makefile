all: build
.PHONY: test build format
test:
	@CC=clang bazel test test:kube-test --test_output=errors
build:
	@CC=clang bazel build //lib:kube
format:
	@clang-format -i $(shell find lib | grep hpp)
	@clang-format -i $(shell find test | grep cpp)
