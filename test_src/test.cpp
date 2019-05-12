#include <map>
#include <vector>
#include <iostream>
#include <string>
#include <list>
#include <stddef.h>

#include "wc.h"

#include "wc_list.h"
#include "wc_vector.h"

#include "test_virtual.h"
#include "c_style_class.h"
#include "complex_type.h"
#include "responsible_member.h"
#include "MyClasses.h"
#include "test_diamond.h"

bool ommit_previous = false;
size_t repeate = 100;

struct LayoutPrinter
{
    LayoutPrinter(void* x = nullptr) : base(x){}
    void operator()(void* x, const char* name, size_t size)
    {
        std::cout << preface << name << " [" << std::ptrdiff_t(x) - ptrdiff_t(base) << "," << std::ptrdiff_t(x) - ptrdiff_t(base) + std::ptrdiff_t(size) << ")" << std::endl;
    }
    void operator[](void* p)
    {
        base = p;
        preface += "->";
    }
    void operator++()
    {
        preface += ' ';
    }
    void operator++(int)
    {
        preface += ':';
    }
    void* base;
    std::string preface;
};

template<class T>
void PrintLayout(const T* t = reinterpret_cast<T*>(sizeof(T)))
{
    LayoutPrinter f((void*)t);
    
    wc::Stitcher<T>::Custom(f, t);
}

template<class T, bool assign>
struct TestInternal
{
	static bool Do(T* original, unsigned char*);
};

template<class T>
struct TestInternal<T, true>
{
	static bool Do(T* original, unsigned char* buffer)
	{
		T reconstructed;
		wc::Serializer::UnDo<T>(reconstructed, buffer);
		// and then compare
		return reconstructed == *original;
	}
};

template<class T>
struct TestInternal<T, false>
{
	static bool Do(T* original, unsigned char* buffer)
	{
		T* reconstructed = wc::Serializer::UnDo<T>(buffer);
		const bool result(*reconstructed == *original);
		delete reconstructed;
		return result;
	}
};

template<class T, bool assign = false>
void Test(T* original, const char* fname)
{
	wc::BufferType* buffer;
    FILE* f;
	try
	{
		if ((!ommit_previous) && (f = fopen(fname, "rb")))
		{   // reads from file into buffer
			wc::ReadBuffer(f);
			fclose(f);
		}
		else
		{  // serializes
            for (size_t i = 0; i < repeate; ++i)
    			wc::Serializer::Do(original);
		}

		wc::PrintBuffer();
		std::cout << std::endl;

		buffer = wc::GetBuffer();
		auto tmp_buffer = *buffer;

		// de-serializes
		if (!TestInternal<T, assign>::Do(original, tmp_buffer.data()))
		{
			std::cerr << "reconstructed is not equal to original!" << std::endl;
			goto FAILED;
		}
	}catch (std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		goto FAILED;
	}

    // if successful then write serialized object into file
    f = fopen(fname, "wb");
	if (f)
	{
		auto const written = fwrite(buffer->data(), sizeof(wc::BufferType::value_type), buffer->size(), f);
		if (written != buffer->size() || fclose(f) == EOF)
		{
			std::cerr << "unable to write in file \"" << fname << "\"" <<std::endl;
			goto FAILED;
		}
	}else
	{
		std::cerr << "unable to open file \"" << fname << "\" for writing!" << std::endl;
		goto FAILED;
	}
 
    return;
FAILED:
    fprintf(stderr, "Failed!\n");
    exit(1);
}

int main(int argc, char* argv[])
{
    for (++argv; *argv; ++argv)
    {
        const std::string arg = *argv;
        if ((arg == "-n" || arg == "--repeat") && *(argv + 1) && atoi(*(argv+1)) > 0)
            repeate = atoi(*++argv);
        if (arg == "-o" || arg == "--omit" || arg == "--overwite")
            ommit_previous = true;
    }

    std::cout << wc::get_compile_info() << " initial buffer: " << WC_INITIAL_BUFFER_SIZE << std::endl;

    ComplexChild cc;

    cc.v.push_back(0);
    cc.vv.push_back(1);

    //std::vector<VirtualChild1> addons(2);
    //addons[0] = add;
    //add.b -= 1;
    //addons[1] = add;

    //std::map<char, VirtualChild1> addons2;
    //addons2['#'] = add;
    //add.b += 1;
    //addons2['a'] = add;

    std::vector<std::vector<unsigned char>> m(2);
    m[0].resize(2); m[1].resize(5);
    m[0][0] = 1; m[0][1] = 2;
    m[1][0] = 3; m[1][1] = 4; m[1][2] = 5; m[1][3] = 6; m[1][4] = 7;

    std::list<char> l = { 'A', '0', '#' };
    //std::list<VirtualChild1> l2(addons.begin(), addons.end());

#define PRINT_LAYOUT(X) PrintLayout< X > ()

    {
    const int const_test = 1;
    PRINT_LAYOUT(const int);
    Test(&const_test, "argc.bin");
    }
    {
        POD pod;
        POD2 pod2;
        X x;
        Y y;

        pod.a = 2; pod.b = -3.14;
        pod2.a = 'A'; pod2.b = 'B';
        pod2.c = true;

        static_cast<POD2&>(x) = pod2;
        x.x = 3;

        static_cast<X&>(y) = x;
        y.flag = true;

        PRINT_LAYOUT(POD);
        Test<POD, true>(&pod, "pod1.bin");

        PRINT_LAYOUT(POD2);
        Test<POD2, false>(&pod2, "pod2.bin");

        PRINT_LAYOUT(X);
        Test<X, true>(&x, "pod3.bin");

        PRINT_LAYOUT(Y);
        Test<Y, false>(&y, "pod4.bin");
    }
    {
        VirtualChild1 vc1;
        VirtualChild2 vc2;

        vc1.a = 5; vc1.b = 7;
        static_cast<VirtualParent&>(vc2) = vc1;
        vc2.i = 11;
        vc2.x = -3.14;

        PRINT_LAYOUT(VirtualChild1);
        Test<VirtualChild1, true>(&vc1, "virtual1.bin");

        PRINT_LAYOUT(VirtualChild2);
        Test<VirtualChild2, false>(&vc2, "virtual2.bin");
    }
    {
        ClassWithStrongPtr cp1(10);
        PrintLayout(&cp1);
        Test(&cp1, "strong_ptr.bin");
    
        ClassWithWeakPtr cp2(10);
		PRINT_LAYOUT(ClassWithWeakPtr);
		Test(&cp2, "weak_ptr.bin");
		
        ClassWithUnusedData cp3;
		PRINT_LAYOUT(ClassWithUnusedData);
		Test(&cp3, "unused.bin");
	}
    {
        D0 d0; D1 d1; D2 d2; D3 d3;

        static_cast<D1&>(d3).d0 = -1;
        static_cast<D2&>(d3).d0 = -2;
        d3.d1 = 1; d3.d2 = 2; d3.d3 = 3;

        PrintLayout(&d3);
        Test(&d3, "d3.bin");

        V0 v0; V1 v1; V2 v2; V3 v3;

        v3.v0 = 4; v3.v1 = 1; v3.v2 = 2; v3.v3 = 3;

        PrintLayout(&v3);
        Test(&v3, "v3.bin");

    }
    {
        std::vector<int> simple_v;
        PrintLayout(&simple_v);
        Test(&simple_v, "emptyvector.bin");
        simple_v.push_back(1); simple_v.push_back(2); simple_v.push_back(3);
        PrintLayout(&simple_v);
        Test(&simple_v, "vectorint.bin");
    }
    {
        std::vector<POD> vs;
        vs.emplace_back(); vs.back().a = 1;
        vs.emplace_back(); vs.back().a = 2;
        PrintLayout(&vs);
        Test(&vs, "vector_simple.bin");
    }

    PrintLayout(&cc);
    Test(&cc, "ComplexChild.bin");

    //{
    //    std::vector<wc::ResponsiblePtr<Odd>> vp;
    //    printf("vector<responsible>");
    //    vp.emplace_back(new Odd()); vp.back()->a = 1;
    //    vp.emplace_back(new Odd()); vp.back()->a = 2;
    //    Test(&vp, "vector_ptr.bin");
    //}

    PrintLayout(&m);
    Test(&m, "vector_vector.bin");

    PrintLayout(&l);
    Test(&l, "list_char.bin");

    //PrintLayout(&l2);
    //Test(&l2, "list_Add.bin");

    l.clear();
    PrintLayout(&l);
    Test(&l, "empty_list.bin");

    printf("Succeeded!\n");
    return 0;

    //printf("Map of Add ");
    //Test(&addons2, "map_add.bin");

    return 0;
}
