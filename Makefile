.PHONY: all
.PHONY: clean
.PHONY: test

all: do_test test
clean:
	rm -f do_test
test:
	./do_test

CXX=c++
CXXFLAGS=-std=c++11 -Wall -Werror -O2

do_test: test.cpp scope_guard.h
	$(CXX) $(CXXFLAGS) -o do_test test.cpp
