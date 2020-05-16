all: test
	./build/kahan_test

test:
	g++ --std=c++11 -I../src --coverage kahan/kahan.test.cpp -o build/kahan_test

test-coverage:
	mkdir -p reports
	lcov --directory . --capture --output-file reports/app.info
	(cd reports && genhtml app.info)

CXXFLAGS_BCH=-std=c++11 -O3 -Wall -fno-exceptions -fno-rtti -pedantic -Werror
GBENCH_DIRS=-isystem thirdparty/gbenchmark/include -Lthirdparty/gbenchmark/build/src
GBENCH_LIBS=-lbenchmark

bench: all.bench.cpp
		g++ $< $(CXXFLAGS_BCH) -I../src $(GBENCH_DIRS) -o build/kahan_bench $(GBENCH_LIBS) -lpthread

perf: bench
	sudo cpupower frequency-set --governor performance
	#./build/kahan_bench
	# apt install linux-tools linux-tools-common linux-tools-`uname -r`
	# test perf: sudo perf record /bin/ls
	sudo perf stat ./build/kahan_bench 
	sudo cpupower frequency-set --governor powersave

deps:
	./get_gbenchmark.sh

.PHONY: bench

clean:
	rm -f *.gcda
	rm -f *.gcno
	rm -f *_test
