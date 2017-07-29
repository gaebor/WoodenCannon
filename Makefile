CPPFLAGS:=-c -Wall -Wfatal-errors -Wno-invalid-offsetof -std=c++11 -Iinc -DWC_NETWORK -O2 $(CPPFLAGS)
CPP=g++
AR=ar

TEST_SRCS=$(wildcard test_src/*.cpp)
TRGT_SRCS=$(wildcard src/*.cpp)

TEST_OBJS=$(TEST_SRCS:cpp=o)
TRGT_OBJS=$(TRGT_SRCS:cpp=o)

test: lib $(TEST_OBJS)
	$(CPP) -L. $(TEST_OBJS) -lwc -o $@

run: test
	./test && ./test

test_src/%.o :: test_src/%.cpp
	$(CPP) $(CPPFLAGS) $< -o $@

src/%.o :: src/%.cpp
	$(CPP) $(CPPFLAGS) -Isrc $< -o $@

all: clean run

lib: libwc.a

libwc.a: $(TRGT_OBJS)
	$(AR) rcs $@ $(TRGT_OBJS)

clean_bin:
	rm -f *.bin

clean: clean_bin
	rm -f test_src/*.o src/*.o *.a test
