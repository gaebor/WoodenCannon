#ifndef INCLUDE_C_STYLE_H
#define INCLUDE_C_STYLE_H

#include "wc.h"
#include <stddef.h>

class MyParent
{
public:
    bool operator==(const MyParent& other)const{return a == other.a && b == other.b;}
    int a;
    double b;
};

struct Odd
{
public:
    //Odd(int a = 0, char c = 'o') :a(a), c(c){}
    bool operator==(const Odd& other)const{ return a == other.a && c == other.c;}
    char a;
    char c;
    char d;
};
namespace wc {
    template<>
    struct MembersOf<Odd> : Members<Odd,
        Member<Odd, offsetof(Odd, a), decltype(Odd::a)>,
        Member<Odd, offsetof(Odd, c), decltype(Odd::c)>,
        Member<Odd, offsetof(Odd, d), decltype(Odd::d)>>
    {};
}

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
    struct MembersOf<Z> : Members<Z,
        Member<Z, offsetof(Z, a), decltype(Z::a)>,
        Member<Z, offsetof(Z, b), decltype(Z::b)>,
        Member<Z, offsetof(Z, flag), decltype(Z::flag)>>
    {};
}

#endif //INCLUDE_C_STYLE_H
