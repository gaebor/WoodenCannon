CPPFLAGS=-Wall -Wfatal-errors -Wno-invalid-offsetof -O3 -std=c++11 -Isrc -D WC_NETWORK
TARGETS=config test
TEST_SRCS=$(wildcard test_src/*.cpp)
TRGT_SRCS=$(wildcard src/*.cpp)

TEST_OBJS=$(TEST_SRCS:cpp=o)
TRGT_OBJS=$(TRGT_SRCS:cpp=o)

test: $(TEST_OBJS) lib
	g++ $(CPPFLAGS) -L. $(TEST_OBJS) -lwc -o $@

test_src/%.o :: test_src/%.cpp
src/%.o :: src/%.cpp
.o : .cpp
	g++ $(CPPFLAGS) -L. $< -o $@
    
all: clean test

lib: libwc.a

libwc.a: $(TRGT_OBJS)
	ar rcs $@ $(TRGT_OBJS)

clean:
	rm -f test_src/*.o src/*.o *.a $(TARGETS) *.bin
