CPPFLAGS=/EHsc /I../inc $(CPPFLAGS)

default: run

vector: msvc_vector_test.exe
test_objects=$(test_sources:cpp=obj)

test: test.exe

test.exe: lib $(test_objects)
	$(CPP) $(CPPFLAGS) $(test_objects) /link Ws2_32.lib wc.lib /OUT:$@

run: vector
	msvc_vector_test.exe

.exe.cpp::
	cl $(CPPFLAGS) $<

clean:
	del /Q *.exe *.obj
