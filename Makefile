all: test
.PHONY: test format
test:
	@CC=clang bazel test test:kube-test --test_output=errors
format:
	@clang-format -i $(shell find include | grep hpp)
	@clang-format -i $(shell find tests | grep cpp)
