#ifndef INCLUDE_C_STYLE_H
#define INCLUDE_C_STYLE_H

#include "wc.h"
#include <stddef.h>

class MyParent
{
public:
    int a;
    double b;
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
    bool operator==(const Z& other)const;
};

namespace wc {
    template<>
    struct MyMembers<Z> : MembersHelper<Z>
    {
        typedef Members<Z,
        M<offsetof(Z, a), decltype(Z::a)>::Type,
        M<offsetof(Z, b), decltype(Z::b)>::Type,
        M<offsetof(Z, flag), decltype(Z::flag)>::Type> List;
    };
}

#endif //INCLUDE_C_STYLE_H
