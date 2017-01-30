CPPFLAGS=/EHsc /MT /Ot /Ox /nologo
LIBFLAGS=/nologo

test: src/test.cpp lib
	$(CPP) $(CPPFLAGS) /Fosrc/$*.obj src/test.cpp /link wc.lib /OUT:$@.exe

config.exe: src/config.cpp
	$(CPP) $(CPPFLAGS) /Fosrc/$*.obj $** /link /OUT:$@

all: test config

lib: wc.lib

config: config.exe

wc.lib: src/wc_core.obj
	lib $(LIBFLAGS) /OUT:$@ $**

src/wc_core.obj:src/wc_core.cpp
	cl $(CPPFLAGS) /c $** /Fo$@

clean:
	del /Q config.exe wc.lib
