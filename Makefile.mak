CPPFLAGS=/EHsc /MT /Ot /Ox /nologo /Iinc /DWC_NETWORK
LIBFLAGS=/nologo

test_sources=test_src\*.cpp
test_objects=$(test_sources:cpp=obj)

test: test.exe

test.exe: lib $(test_objects)
	$(CPP) $(CPPFLAGS) $(test_objects) /link Ws2_32.lib wc.lib /OUT:$@

run: test
	test.exe && test.exe

{test_src\}.cpp{test_src\}.obj::
	cl /c $(CPPFLAGS) /Fo"test_src/" $<

{src\}.cpp{src\}.obj::
	cl /c $(CPPFLAGS) /Isrc /Fo"src/" $<
    
all: clean run

lib: wc.lib

wc.lib: src/wc_core.obj src/wc_network.obj src/wc_buffer.obj
	lib $(LIBFLAGS) /OUT:$@ $**

clean_bin:
	del /Q *.bin
    
clean: clean_bin
	del /Q *.lib *.exe src\*.obj test_src\*.obj
