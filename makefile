all:
	g++ --std=c++11 src/main_test.cpp -o appMain

test:
	cd tests && make
	./tests/kahan_test
