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
        Member<VirtualParent, offsetof(VirtualParent, a), int>,
        Member<VirtualParent, offsetof(VirtualParent, b), int>>
    {};
}

class VirtualChild1 : public VirtualParent
{
public:
    VirtualChild1(){}
    virtual int Op();
    ~VirtualChild1(){}
};

namespace wc {
    template<>
    struct ParentsOf<VirtualChild1> : Parents<VirtualChild1, VirtualParent>
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
            Member<VirtualParent2, offsetof(VirtualParent2, i), int>
        >
    {};

    template<>
    struct ParentsOf<VirtualChild2> :
        Parents<VirtualChild2, VirtualParent, VirtualParent2>
    {};

    template<>
    struct MembersOf<VirtualChild2> :
        Members<VirtualChild2,
        Member<VirtualChild2, offsetof(VirtualChild2, x), double>
        >
    {};

}

#endif // INCLUDE_TEST_VIRTUAL_H
