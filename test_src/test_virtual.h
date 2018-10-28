#ifndef INCLUDE_TEST_VIRTUAL_H
#define INCLUDE_TEST_VIRTUAL_H

#include "wc.h"

class MyParentVirtual
{
public:
    int a, b;
    virtual int Op() = 0;
    virtual bool operator==(const MyParentVirtual& other)const;
    MyParentVirtual(){}
    virtual ~MyParentVirtual(){}
};

namespace wc {
    template<>
    struct MembersOf<MyParentVirtual> : Members<MyParentVirtual,
        Member<MyParentVirtual, offsetof(MyParentVirtual, a), int>,
        Member<MyParentVirtual, offsetof(MyParentVirtual, b), int>>
    {};
}

class Add : public MyParentVirtual
{
public:
    Add(){}
    bool operator==(const Add& other)const;
    virtual int Op();
    ~Add(){}
};

namespace wc {
    template<>
    struct ParentsOf<Add> : Parents<Add, MyParentVirtual>
    {};
}

class Mul : public MyParentVirtual
{
public:
    virtual int Op();
    bool flag;
    virtual bool operator==(const Mul& other)const;
};

namespace wc {
    template<>
    struct MembersOf<Mul> :
        Members<Mul,
        Member<Mul, offsetof(Mul, flag), bool>
        >
    {};

    template<>
    struct ParentsOf<Mul> :
        Parents<Mul, MyParentVirtual>
    {};

}

#endif // INCLUDE_TEST_VIRTUAL_H
