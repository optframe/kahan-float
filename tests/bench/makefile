all:
	g++ -Ofast --std=c++11 bench.cpp -I../../src -o appBench
	./appBench


toasm:
	g++ kahan.bench.cpp -O2 -c -S -o - -masm=intel \
	  | c++filt \
          | grep -vE '\s+\.'
