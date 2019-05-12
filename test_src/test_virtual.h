#ifndef INCLUDE_TEST_VIRTUAL_H
#define INCLUDE_TEST_VIRTUAL_H

#include "wc.h"

class VirtualParent
{
public:
    int a, b;
    virtual int Op() = 0;
    virtual bool operator==(const VirtualParent& other)const;
    VirtualParent(){}
    virtual ~VirtualParent(){}
};

namespace wc {
    template<>
    struct MembersOf<VirtualParent> : Members<VirtualParent,
        Member2<decltype(&VirtualParent::a), &VirtualParent::a>,
        Member2<decltype(&VirtualParent::b), &VirtualParent::b>>
    {};
}

class VirtualChild1 : public VirtualParent
{
public:
    VirtualChild1(){}
    virtual int Op();
    ~VirtualChild1(){}
};

class VirtualChild11 : public VirtualChild1
{
public:
    VirtualChild11() {}
    virtual int Op();
    ~VirtualChild11() {}
};

namespace wc {
    template<>
    struct ParentsOf<VirtualChild1> : Parents<VirtualChild1, VirtualParent>
    {};

    template<>
    struct ParentsOf<VirtualChild11> : Parents<VirtualChild11, VirtualChild1>
    {};
}

class VirtualParent2
{
public:
    virtual int f() = 0;
    bool operator==(const VirtualParent2& other)const;
    int i;
};

class VirtualChild2 : public VirtualParent, public VirtualParent2
{
public:
    virtual int f();
    virtual int Op();
    bool operator==(const VirtualChild2& other)const;
    double x;
};

namespace wc {
    template<>
    struct MembersOf<VirtualParent2> :
        Members<VirtualParent2,
        Member2<decltype(&VirtualParent2::i), &VirtualParent2::i>
        >
    {};

    template<>
    struct ParentsOf<VirtualChild2> :
        Parents<VirtualChild2, VirtualParent, VirtualParent2>
    {};

    template<>
    struct MembersOf<VirtualChild2> :
        Members<VirtualChild2,
        Member2<decltype(&VirtualChild2::x), &VirtualChild2::x>
        >
    {};

}

#endif // INCLUDE_TEST_VIRTUAL_H
