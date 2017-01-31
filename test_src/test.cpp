#include <map>
#include <vector>
#include <iostream>
#include <list>
#include <stddef.h>

#include "test_virtual.h"
#include "c_style_class.h"
#include "complex_type.h"
#include "responsible_member.h"

using namespace wc;

template<typename T>
void Test(T* t, const char* fname)
{
    T reference;
    FILE* f = fopen(fname, "rb");
    if (f)
    {   // reads from file into buffer
        ReadBuffer(f);
        fclose(f);
    }
    else
    {  // serializes
        Serializer::Do(t);
    }

    Serializer::print_buffer();
    std::cout << std::endl;

    auto buffer = *wc::GetBuffer();

    // de-serializes
    Serializer::UnDo(&reference);
    // and then compare
    if (!(reference == *t))
    {
        fprintf(stderr, "Failed!\n");
        exit(1);
    }
    { // if successful then write serialized object into file
        f = fopen(fname, "wb");
        if (f)
        {
            fwrite(buffer.data(), sizeof(wc::BufferType::value_type), buffer.size(), f);
            fclose(f);
        }
    }
    // de-constructs reference
}

int main(int argc, char* argv[])
{
    Add add; Mul mul;
    Z z; ComplexChild cc;

    z.a = 5;
    z.b = 'a';
    z.x = 7;
    z.flag = false;

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

    printf("Z ");
    Test(&z, "Z.bin");

    printf("Add ");
    Test(&add, "Add.bin");

    printf("Mul ");
    Test(&mul, "mul.bin");

    printf("ComplexChild ");
    Test(&cc, "cc.bin");

    printf("Vector of Add ");
    Test(&addons, "vector_add.bin");

    printf("vector of vector ");
    Test(&m, "vector_vector.bin");

    return 0;

    printf("Map of Add ");
    Test(&addons2, "map_add.bin");

    std::list<char> l = { 'A', '0', '#' };
    printf("list ");
    Test(&l, "list.bin");

    return 0;
}
