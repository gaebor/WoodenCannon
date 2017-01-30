CPPFLAGS=-Wall -Wfatal-errors -Wno-unknown-pragmas -O3 -std=c++11 -Isrc
TARGETS=config test

test: src/test.cpp lib
	g++ $(CPPFLAGS) -L. $< -lwc -o $@

all: test config

lib: libwc.a

config: src/config.cpp
	g++ $(CPPFLAGS) -o $@ $<

libwc.a: src/wc_core.o
	ar rcs $@ $<

src/wc_core.o:src/wc_core.cpp

.o:.cpp
	g++ $(CPPFLAGS) -o $@ -c $<

clean:
	rm -f src/*.o *.a $(TARGETS)
