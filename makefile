all: demo

demo: demo/main_test.cpp
	echo "building demo/app_demo"
	g++ --std=c++11 $< -Iinclude/ -o demo/app_demo
	echo "running demo/app_demo"
	demo/app_demo

test:
	cd tests && make

test-coverage: test
	cd tests && make test-coverage
	echo "see tests/reports/index.html for more information"

CATCH2_DIR=tests/thirdparty/Catch2
vendor:
	git init
	mkdir -p tests/thirdparty
	git submodule add https://github.com/catchorg/Catch2.git $(CATCH2_DIR) || true
	git submodule update --remote 
	(cd $(CATCH2_DIR) && git checkout v3.3.2)
	#
	(cd tests/thirdparty/catch2/ && curl -LO https://github.com/catchorg/Catch2/releases/download/v3.3.2/catch_amalgamated.hpp)
	(cd tests/thirdparty/catch2/ && curl -LO https://github.com/catchorg/Catch2/releases/download/v3.3.2/catch_amalgamated.cpp)


.PHONY: demo
