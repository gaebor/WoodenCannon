#ifndef INCLUDE_C_STYLE_H
#define INCLUDE_C_STYLE_H

#include "wc.h"
#include <stddef.h>

//! a plain old data, although the wc::MembersOf is not specified.
class POD
{
public:
    bool operator==(const POD& other)const;
    int a;
    double b;
};

//! a plain old data, the wc::MembersOf is properly specified.
struct POD2
{
public:
    bool operator==(const POD2& other)const;
    char a;
    char b;
    bool c;
};

namespace wc {
    template<>
    struct MembersOf<POD2> : Members<POD2,
        Member2<decltype(&POD2::a), &POD2::a>,
        Member2<decltype(&POD2::b), &POD2::b>,
        Member2<decltype(&POD2::c), &POD2::c>>
    {};
}

//! inheritance from plain old data
class X : public POD2
{
public:
    bool operator==(const X& other)const;
    int x;
};

namespace wc {
    template<>
    struct MembersOf<X> : Members<X,
        Member2<decltype(&X::x), &X::x>>
    {};
    template<>
    struct ParentsOf<X> : Parents<X, POD2>
    {};
}

//! still plain old data, but the inheritance structure is not well defined
class Y : public X
{
public:
    bool flag;
    bool operator==(const Y& other)const;
};

namespace wc {
    template<>
    struct MembersOf<Y> : Members<Y,
        Member2<decltype(&Y::a), &Y::a>,
        Member2<decltype(&Y::x), &Y::x>,
        Member2<decltype(&Y::flag), &Y::flag>>
    {};
}

#endif //INCLUDE_C_STYLE_H
