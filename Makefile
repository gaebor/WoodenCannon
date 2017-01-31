CPPFLAGS=-Wall -Wfatal-errors -Wno-unknown-pragmas -O3 -std=c++11 -Isrc -D WC_NETWORK
TARGETS=config test
TEST_SRCS=$(wildcard test_src/*.cpp)
TRGT_SRCS=$(wildcard src/*.cpp)

TEST_OBJS=$(TEST_SRCS:cpp=o)
TRGT_OBJS=$(TRGT_SRCS:cpp=o)

test: $(TEST_SRCS) lib
	g++ $(CPPFLAGS) -L. $(TEST_SRCS) -lwc -o $@

test_src/%.o :: test_src/%.cpp
	g++ $(CPPFLAGS) -L. $< -o $@

src/%.o :: src/%.cpp
.o : .cpp
	g++ $(CPPFLAGS) -L. $< -o $@
    
all: test config

lib: libwc.a

config: src/config.o
	g++ $(CPPFLAGS) -o $@ $<

libwc.a: $(TRGT_OBJS)
	ar rcs $@ $(TRGT_OBJS)

clean:
	rm -f test_src/*.o src/*.o *.a $(TARGETS) *.bin
