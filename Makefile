all: test
.PHONY: test install format
test:
	@mkdir -p build/
	@cd build/ &&cmake .. && make
	@./build/tests/tests
install:
	@mkdir -p build/
	@cd build/ &&cmake .. && make install
format:
	@clang-format -i $(shell find include | grep hpp)
	@clang-format -i $(shell find tests | grep cpp)
