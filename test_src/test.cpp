#include <map>
#include <vector>
#include <iostream>
#include <list>
#include <stddef.h>

#include "wc.h"

#ifdef __GNUC__
#include "wc_stl.h"
#endif 

#include "test_virtual.h"
#include "c_style_class.h"
#include "complex_type.h"
#include "responsible_member.h"
#include "MyClasses.h"

template<typename T>
void PrintLayout(const char* name, T* t = nullptr)
{
    std::cout << name;
    wc::Stitcher<T>::Custom(
        [&t](size_t x)
		{
			std::cout << ' ' << x-(size_t)t;
		}
    , t);
    std::cout << ' ' << sizeof(T);
}

template<typename T>
void Test(T* original, const char* fname)
{
    try
    {
        FILE* f = fopen(fname, "rb");
        if (f)
        {   // reads from file into buffer
            wc::ReadBuffer(f);
            fclose(f);
        }
        else
        {  // serializes
            wc::Serializer::Do(original);
        }

        wc::PrintBuffer();
        std::cout << std::endl;

        auto buffer = *wc::GetBuffer();

        // de-serializes
        T* reconstructed = wc::Serializer::UnDo<T>(wc::BufferType(buffer).data());
        // and then compare
        if (!(*reconstructed == *original))
        {
            goto FAILED;
        }
        // if successful then write serialized object into file
        f = fopen(fname, "wb");
        if (f)
        {
            auto const written = fwrite(buffer.data(), sizeof(wc::BufferType::value_type), buffer.size(), f);
            if (written != buffer.size() || fclose(f) == EOF)
                goto FAILED;
        }else
            goto FAILED;
        delete reconstructed;
    }
    catch (...)
    {
        goto FAILED;
    }
    return;
FAILED:
    fprintf(stderr, "Failed!\n");
    exit(1);
}

int main(int argc, char* argv[])
{
    std::cerr << wc::get_compile_info() << std::endl;

    Add add; Mul mul;
    Z z; ComplexChild cc;

    z.a = 5;
    z.b = 'a';
    z.x = 7;
    z.flag = true;

    add.a = 5; add.b = 7;
    
    mul.a = 5; mul.b = 7;
    mul.flag = false;

    cc.v.push_back(0);
    cc.vv.push_back(1);

    std::vector<Add> addons(2);
    addons[0] = add;
    add.b -= 1;
    addons[1] = add;

    std::map<char, Add> addons2;
    addons2['#'] = add;
    add.b += 1;
    addons2['a'] = add;

    std::vector<std::vector<unsigned char>> m(2);
    m[0].resize(2); m[1].resize(5);
    m[0][0] = 1; m[0][1] = 2;
    m[1][0] = 3; m[1][1] = 4; m[1][2] = 5; m[1][3] = 6; m[1][4] = 7;

    std::list<char> l = { 'A', '0', '#' };
    std::list<Add> l2(addons.begin(), addons.end());

#define PRINT_LAYOUT(X) PrintLayout< X > ( #X )

    {
    const int const_test = argc;
    PRINT_LAYOUT(const int);
    Test(&const_test, "argc.bin");
    }

    PRINT_LAYOUT(Z);
    Test(&z, "Z.bin");

    PRINT_LAYOUT(Add);
    Test(&add, "Add.bin");

    PRINT_LAYOUT(Mul);
    Test(&mul, "Mul.bin");

    {
        ClassWithStrongPtr cp(10);
        PrintLayout("ClassWithStrongPtr", &cp);
        Test(&cp, "class_strong_ptr.bin");
    }

	{
		ClassWithWeakPtr cp(10);
		PRINT_LAYOUT(ClassWithWeakPtr);
		Test(&cp, "class_weak_ptr.bin");
	}

	{
		ClassWithUnusedData cp;
		printf("ClassWithUnusedData\n");
		Test(&cp, "class_with_unused.bin");
	}

    PRINT_LAYOUT(ComplexChild);
    Test(&cc, "ComplexChild.bin");

    {
    std::vector<MyParent> vs;
    vs.emplace_back(); vs.back().a = 1;
    vs.emplace_back(); vs.back().a = 2;
    PrintLayout<decltype(vs)>("vector_simple");
    Test(&vs, "vector_simple.bin");
    }

    {
    std::vector<Odd> vo;
    vo.emplace_back(); vo.back().a = 1;
    vo.emplace_back(); vo.back().a = 2;
    printf("vector_odd");
    Test(&vo, "vector_odd.bin");
    }

    //{
    //    std::vector<wc::ResponsiblePtr<Odd>> vp;
    //    printf("vector<responsible>");
    //    vp.emplace_back(new Odd()); vp.back()->a = 1;
    //    vp.emplace_back(new Odd()); vp.back()->a = 2;
    //    Test(&vp, "vector_ptr.bin");
    //}

    printf("vector_vector");
    Test(&m, "vector_vector.bin");

    printf("list_char");
    Test(&l, "list_char.bin");

    printf("list_Add");
    Test(&l2, "list_Add.bin");

    l.clear();
    printf("empty_list");
    Test(&l, "empty_list.bin");

    printf("Succeeded!\n");
    return 0;

    printf("Map of Add ");
    Test(&addons2, "map_add.bin");

    return 0;
}
