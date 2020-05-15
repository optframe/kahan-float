all: test
	./build/kahan_test

test:
	g++ --std=c++11 --coverage kahan.test.cpp -o build/kahan_test

test-coverage:
	mkdir -p reports
	lcov --directory . --capture --output-file reports/app.info
	(cd reports && genhtml app.info)

clean:
	rm -f *.gcda
	rm -f *.gcno
	rm -f *_test

