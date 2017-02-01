CPPFLAGS=/EHsc /MT /Ot /Ox /nologo /Isrc /D"WC_NETWORK"
LIBFLAGS=/nologo

test: test_src/*.cpp lib
	$(CPP) $(CPPFLAGS) /Fo"test_src/" test_src/*.cpp /link Ws2_32.lib wc.lib /OUT:$@.exe

all: clean test

lib: wc.lib

wc.lib: src/wc_core.obj
	lib $(LIBFLAGS) /OUT:$@ src/*.obj

src/wc_core.obj:src/wc_core.cpp
	cl $(CPPFLAGS) /Fo"src/" /c src/*.cpp

clean:
	del /Q *.lib *.exe src\*.obj test_src\*.obj *.bin
