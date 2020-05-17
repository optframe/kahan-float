all: test
	./build/kahan_test

test:
	g++ --std=c++11 -I../src --coverage kahan/kahan.test.cpp -o build/kahan_test

test-coverage:
	mkdir -p reports
	lcov --directory . --capture --output-file reports/app.info
	(cd reports && genhtml app.info)

CLANGFLAGS_BCH=-std=c++14 -O3 -Wall -fno-exceptions -fno-rtti -pedantic -Werror -stdlib=libc++  -lc++abi -Wl,-rpath,/usr/local/gcc-10.1.0/lib64
CXXFLAGS_BCH=-std=c++14 -O3 -Wall -fno-exceptions -fno-rtti -pedantic -Werror -fno-omit-frame-pointer
GBENCH_DIRS=-isystem thirdparty/gbenchmark/include -Lthirdparty/gbenchmark/build/src
GBENCH_LIBS=-lbenchmark 

bench: all.bench.cpp
	#clang++ $< $(CLANGFLAGS_BCH) -I../src $(GBENCH_DIRS) -o build/kahan_bench $(GBENCH_LIBS) -lpthread
	g++-10.1 $< $(CXXFLAGS_BCH) -I../src $(GBENCH_DIRS) -o build/kahan_bench $(GBENCH_LIBS) -lpthread

#PERF=./perf-4.19.0/tools/perf/perf
PERF=linux-4.16.12/tools/perf/perf


perf: bench
	sudo cpupower frequency-set --governor performance
	#./build/kahan_bench
	# apt install linux-tools linux-tools-common linux-tools-`uname -r`
	# apt install linux-cloud-tools-`uname -r`
	#
	# wget https://mirrors.edge.kernel.org/pub/linux/kernel/tools/perf/v4.19.0/perf-4.19.0.tar.xz
	# tar xf perf-4.19.0.tar.xz && cd perf-4.19.0/ && make -C tools/perf install
	# perf-4.19.0/tools/perf
	#
	# https://stackoverflow.com/questions/33057653/perf-enable-demangling-of-callgraph
	#
	#sudo apt-get install libiberty-dev binutils-dev
	#apt-get source linux-tools-`uname -r`
	#sudo apt-get build-dep linux-tools-`uname -r`
	#cd linux-`uname -r | sed 's/-.*//'`/tools/perf
	#make
	#
	#
	# edit: /proc/sys/kernel/perf_event_paranoid
	# add value '-1' instead of '3'
	#
	# sudo su
	# echo 0 > /proc/sys/kernel/kptr_restrict
	#
	# test perf: sudo perf record /bin/ls
	# test perf: perf report
	# wget https://raw.githubusercontent.com/torvalds/linux/master/tools/perf/Documentation/tips.txt && sudo mkdir /usr/share/doc/perf-tip/ && sudo cp tips.txt /usr/share/doc/perf-tip/
	${PERF} stat ./build/kahan_bench 
	# sudo apt install clang-tools-7 libc++-7-dev libc++abi-7-dev 
	sudo -u `whoami` ${PERF} record -g ./build/kahan_bench 
	sudo cpupower frequency-set --governor powersave

perf-report:
	${PERF} report -g 'graph,0.5,caller'

deps:
    # export CXX=/usr/bin/clang++ && make deps
	./get_gbenchmark.sh

.PHONY: bench

clean:
	rm -f *.gcda
	rm -f *.gcno
	rm -f *_test
