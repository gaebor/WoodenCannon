CPPFLAGS=-Wall -Wfatal-errors -Wno-invalid-offsetof -std=c++11 -Isrc -D WC_NETWORK -O2
CPP=g++

TARGETS=config test
TEST_SRCS=$(wildcard test_src/*.cpp)
TRGT_SRCS=$(wildcard src/*.cpp)

TEST_OBJS=$(TEST_SRCS:cpp=o)
TRGT_OBJS=$(TRGT_SRCS:cpp=o)

test: lib $(TEST_OBJS)
	g++ $(CPPFLAGS) -L. $(TEST_OBJS) -lwc -o $@

do_test: clean test
	./test && ./test

test_src/%.o :: test_src/%.cpp
src/%.o :: src/%.cpp
.o :: .cpp

all: do_test

lib: libwc.a

libwc.a: $(TRGT_OBJS)
	ar rcs $@ $(TRGT_OBJS)

clean:
	rm -f test_src/*.o src/*.o *.a $(TARGETS) *.bin
