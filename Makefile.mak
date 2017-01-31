CPPFLAGS=/EHsc /MT /Ot /Ox /nologo /Isrc
LIBFLAGS=/nologo

test: test_src/*.cpp lib
	$(CPP) $(CPPFLAGS) /Fo"test_src/" test_src/*.cpp /link wc.lib /OUT:$@.exe

config.exe: src/config.cpp
	$(CPP) $(CPPFLAGS) /Fo"src/" src/config.cpp /link /OUT:$@

all: test config

lib: wc.lib

config: config.exe

wc.lib: src/wc_core.obj
	lib $(LIBFLAGS) /OUT:$@ src/*.obj

src/wc_core.obj:src/wc_core.cpp
	cl $(CPPFLAGS) /Fo"src/" /c src/*.cpp

clean:
	del /Q config.exe wc.lib test.exe src\*.obj test_src\*.obj
