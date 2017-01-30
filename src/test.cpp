#include <map>
#include <vector>
#include <iostream>
#include <list>
#include <stddef.h>

#include "wc.h"

/*************************************************************************
MyParentVirtual       MyParent
 |     \               |     \
 |      \              |      \
Add     Mul            X       Y
 |                     |
 |                     |
Zero                   Z
*************************************************************************/

class MyParentVirtual
{
public:
    int a, b;
    virtual int Op() = 0;
    virtual bool operator==(const MyParentVirtual& other)const
    {
        return a == other.a && b == other.b;
    }
};

class Add : public MyParentVirtual
{
public:
    bool operator==(const Add& other)const
    {
        return static_cast<const MyParentVirtual*>(this)->operator==((const MyParentVirtual&)other);
    }
    virtual int Op()
    {
        return a + b;
    };
};

class Zero : public Add
{
public:
    virtual int Op()
    {
        return a;
    };
};

class Mul: public MyParentVirtual
{
public:
    virtual int Op()
    {
        return flag ? -(a * b) : a * b;
    };
    bool flag;
    virtual bool operator==(const Mul& other)const
    {
        return a == other.a && b == other.b && flag == other.flag;
    }
};

class MyParent
{
public:
    int a;
    char b;
};

class X : public MyParent
{
public:
    int x;
};

class Y : public MyParent
{
public:
    char y;
};

class Z : public X
{
public:
    bool flag;
    bool operator==(const Z& other)const
    {
        return a == other.a && b == other.b && x == other.x && flag == other.flag;
    }
};

class ComplexParent
{
public:
    std::vector<int> v;
    bool operator==(const ComplexParent& other)const
    {
        return v == other.v;
    }
};

class ComplexChild : public ComplexParent
{
public:
    std::vector<int> vv;
    bool operator==(const ComplexChild& other)const
    {
        return vv == other.vv && static_cast<const ComplexParent*>(this)->operator==((const ComplexParent&)other);
    }
};

// this is what You have to do!
namespace wc{
    //! defines the (one and only) member of ComplexParent
    template<>
    struct MyMembers<ComplexParent> : public MembersHelper<ComplexParent>
    {
        typedef Members<ComplexParent, M<offsetof(ComplexParent, v), std::vector<int>>::Type> List;
    };

    //! defines the (one and only) member of ComplexChild
    template<>
    struct MyMembers<ComplexChild> : public MembersHelper<ComplexChild>
    {
        typedef Members<ComplexChild, M<offsetof(ComplexChild, vv), std::vector<int>>::Type> List;
    };

    //! defines the relation between ComplexChild and ComplexParent
    template<>
    struct MyParents<ComplexChild>
    {
        typedef Parents<ComplexChild, ComplexParent> List;
    };
}

using namespace wc;

template<typename T>
void Test(T* t, const char* fname)
{
    T reference;
    FILE* f = fopen(fname, "rb");
    if (f)
    {   // reads from file
        ReadBuffer(f);
        fclose(f);
    }else
    {  // serializes
        Serializer::Do(t);
    }
    {
        f = fopen(fname, "wb");
        if (f)
        {
            WriteBuffer(f);
            fclose(f);
        }
    }

    Serializer::print_buffer();
    std::cout << std::endl;

    // de-serializes
    Serializer::UnDo(&reference);
    // and then compare
    if (!(reference == *t))
    {
        fprintf(stderr, "Failed!\n");
        exit(1);
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

    printf("Z ");
    Test(&z, "Z.bin");

    printf("Add ");
    Test(&add, "Add.bin");

    printf("Mul ");
    Test(&mul, "mul.bin");

    printf("ComplexChild ");
    Test(&cc, "cc.bin");

    std::vector<Add> addons(2);
    addons[0] = add;
    add.b -= 1;
    addons[1] = add;
    
    printf("Vector of Add ");
    Test(&addons, "vector_add.bin");

    std::map<char, Add> addons2;
    addons2['#'] = add;
    add.b += 1;
    addons2['a'] = add;

    return 0;

    printf("Map of Add ");
    Test(&addons2, "map_add.bin");

    std::vector<std::vector<unsigned char>> m(2);
    m[0].resize(2); m[1].resize(5);
    m[0][0] = 1; m[0][1] = 2;
    m[1][0] = 3; m[1][1] = 4; m[1][2] = 5; m[1][3] = 6; m[1][4] = 7;

    printf("vector of vector ");
    Test(&m, "vector_vector.bin");

    std::list<char> l = { 'A', '0', '#' };
    printf("list ");
    Test(&l, "list.bin");

    return 0;
}
