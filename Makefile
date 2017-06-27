CPPFLAGS=-c -Wall -Wfatal-errors -Wno-invalid-offsetof -Isrc -DWC_NETWORK -O2
CPP=g++
AR=ar

TEST_SRCS=$(wildcard test_src/*.cpp)
TRGT_SRCS=$(wildcard src/*.cpp)

TEST_OBJS=$(TEST_SRCS:cpp=o)
TRGT_OBJS=$(TRGT_SRCS:cpp=o)

test: lib $(TEST_OBJS)
	$(CPP) -L. $(TEST_OBJS) -lwc -o $@

do_test: clean test
	./test && ./test

test_src/%.o :: test_src/%.cpp
	$(CPP) $(CPPFLAGS) $< -o $@
src/%.o :: src/%.cpp
	$(CPP) $(CPPFLAGS) $< -o $@
.o :: .cpp
	$(CPP) $(CPPFLAGS) $< -o $@

all: do_test

lib: libwc.a

libwc.a: $(TRGT_OBJS)
	$(AR) rcs $@ $(TRGT_OBJS)

clean:
	rm -f test_src/*.o src/*.o *.a test *.bin
