all: demo

demo: demo/main_test.cpp
	echo "building demo/app_demo"
	g++ --std=c++11 $< -Isrc -o demo/app_demo
	echo "running demo/app_demo"
	demo/app_demo

test:
	cd tests && make
	./tests/kahan_test

.PHONY: demo
