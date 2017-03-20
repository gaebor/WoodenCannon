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
    struct MyMembers<MyParentVirtual> : MembersHelper<MyParentVirtual>
    {
        typedef Members<MyParentVirtual,
        M<offsetof(MyParentVirtual, a), int>::Type,
        M<offsetof(MyParentVirtual, b), int>::Type> List;
    };
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
    struct MyParents<Add>
    {
        typedef Parents<Add, MyParentVirtual> List;
    };
}

class Zero : public Add
{
public:
    virtual int Op();
};

class Mul : public MyParentVirtual
{
public:
    virtual int Op();
    bool flag;
    virtual bool operator==(const Mul& other)const;
};

#endif // INCLUDE_TEST_VIRTUAL_H
