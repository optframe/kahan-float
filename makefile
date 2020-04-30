all:
	g++ src/main_test.cpp -o appMain

test:
	cd tests && make
	./tests/kahan_test
