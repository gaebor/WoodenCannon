#ifndef INCLUDE_TEST_DIAMOND_H
#define INCLUDE_TEST_DIAMOND_H

#include "wc.h"

struct D0
{
    D0() {}
    ~D0() {}
    bool operator==(const D0& other)const { return d0 == other.d0; }
    int d0;
};

struct D1 : D0
{
    D1() {}
    ~D1() {}
    bool operator==(const D1& other)const
    {
        return (static_cast<const D0&>(*this) == static_cast<const D0&>(other)) &&
            d1 == other.d1;
    }
    int d1;
};

struct D2 : D0
{
    D2() {}
    ~D2() {}
    bool operator==(const D2& other)const
    {
        return (static_cast<const D0&>(*this) == static_cast<const D0&>(other)) &&
            d2 == other.d2;
    }
    int d2;
};

struct D3 : D1, D2
{
    D3() {}
    ~D3() {}
    bool operator==(const D3& other)const
    {
        return
            (static_cast<const D1&>(*this) == static_cast<const D1&>(other)) &&
            (static_cast<const D2&>(*this) == static_cast<const D2&>(other)) &&
            d3 == other.d3;
    }
    int d3;
};

namespace wc {
    template<>
    struct MembersOf<D0> :
        Members<D0,
            Member2<decltype(&D0::d0), &D0::d0>
        >
    {};

    template<>
    struct MembersOf<D1> :
        Members<D1,
        Member2<decltype(&D1::d1), &D1::d1>
        >
    {};

    template<>
    struct MembersOf<D2> :
        Members<D2,
        Member2<decltype(&D2::d2), &D2::d2>
        >
    {};

    template<>
    struct MembersOf<D3> :
        Members<D3,
        Member2<decltype(&D3::d3), &D3::d3>
        >
    {};

    template<> struct ParentsOf<D3> : Parents<D3, D1, D2> {};
    template<> struct ParentsOf<D2> : Parents<D2, D0> {};
    template<> struct ParentsOf<D1> : Parents<D1, D0> {};
}

struct V0
{
    V0() {}
    ~V0() {}
    bool operator==(const V0& other)const { return v0 == other.v0; }
    int v0;
};

struct V1 : virtual V0
{
    V1() {}
    ~V1() {}
    bool operator==(const V1& other)const
    {
        return (static_cast<const V0&>(*this) == static_cast<const V0&>(other)) &&
            v1 == other.v1;
    }
    int v1;
};

struct V2 : virtual V0
{
    V2() {}
    ~V2() {}
    bool operator==(const V2& other)const
    {
        return (static_cast<const V0&>(*this) == static_cast<const V0&>(other)) &&
            v2 == other.v2;
    }
    int v2;
};

//! WC thinks that there are two v0-s, but there is only one.
struct V3 : V1, V2
{
    V3() {}
    ~V3() {}
    bool operator==(const V3& other)const
    {
        return
            (static_cast<const V1&>(*this) == static_cast<const V1&>(other)) &&
            (static_cast<const V2&>(*this) == static_cast<const V2&>(other)) &&
            v3 == other.v3;
    }
    int v3;
};

namespace wc {
    //template<>
    //struct MembersOf<V0> :
    //    Members<V0,
    //    Member2<decltype(&V0::v0), &V0::v0>
    //    >
    //{};

    template<>
    struct MembersOf<V1> :
        Members<V1,
        Member2<decltype(&V1::v1), &V1::v1>
        >
    {};

    template<>
    struct MembersOf<V2> :
        Members<V2,
        Member2<decltype(&V2::v2), &V2::v2>
        >
    {};

    template<>
    struct MembersOf<V3> :
        Members<V3,
        Member2<decltype(&V3::v3), &V3::v3>
        >
    {};

    template<> struct ParentsOf<V3> : Parents<V3, V1, V2> {};
    template<> struct ParentsOf<V2> : Parents<V2, V0> {};
    template<> struct ParentsOf<V1> : Parents<V1, V0> {};
}

#endif // INCLUDE_TEST_DIAMOND_H
