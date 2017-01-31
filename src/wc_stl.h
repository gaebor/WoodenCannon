#ifndef INCLUDE_WC_STL_H
#define INCLUDE_WC_STL_H

#include <stddef.h>

#include "wc_core.h"

#include <vector>
#include <list>

namespace wc{

#pragma region vector

template <class Ty, class All>
struct MyMembers<std::vector<Ty, All>> : MembersHelper<std::vector<Ty, All>>
{
    typedef std::vector<Ty, All> container;
#ifdef _MSC_VER
    typedef Members < container,
#ifdef _DEBUG
        typename R<offsetof(container, _Myproxy), std::_Container_proxy>::Type,
#endif // _DEBUG
        typename P<offsetof(container, _Myfirst)>::Type,
        typename P<offsetof(container, _Mylast) >::Type,
        typename P<offsetof(container, _Myend)  >::Type> List;
#else
    // hope for the best
    typedef Members < container, Pointer<container, 0>, Pointer<container, 8>, Pointer<container, 16>> List;
#endif // PLATFORM
};

#ifdef _MSC_VER
template <>
struct MyMembers<std::_Container_proxy> : MembersHelper<std::_Container_proxy>
{
    typedef Members < std::_Container_proxy,
    // this is left out intentionally!
    //typename P<offsetof(std::_Container_proxy, _Myfirstiter)>::Type,
    typename P<offsetof(std::_Container_proxy, _Mycont)     >::Type > List;
};
#endif // _MSC_VER

template <class Ty, class All>
struct Callback<std::vector<Ty, All>>
{
    typedef std::vector<Ty, All> container;
#if defined(_MSC_VER) && defined(_DEBUG)
    typedef typename MyMembers<container>::List::Next Hacker;
#else
    typedef typename MyMembers<container>::List Hacker;
#endif
    static void Do(container* v)
    {
        if (v->empty())
        {   //hack!
            auto const end = (void**)(v + 1);
            Hacker::Do([&end](void** x){*x = end; }, v);
        }else
            for (auto& m : *v)
                Stitcher<typename container::value_type>::Do(&m);
    }
    static void UnDo(container* v)
    {   //hack!
        auto const end = (void**)(v + 1);
        bool hacked = true;
        Hacker::Do([&end, &hacked](void** x){hacked = hacked && (*x == end); }, v);
        if (hacked)
        {
            Hacker::Do([&end](void** x){*x = nullptr; }, v);
        }
        else
            for (auto& m : *v)
                Stitcher<typename container::value_type>::UnDo(&m);
    }
};

#pragma endregion vector

#pragma region list

template <class Ty, class All>
struct MyMembers<std::list<Ty, All>>
{
    typedef std::list<Ty, All> container;
    typedef Members < container > List; // not implemented yet
};

template <class Ty, class All>
struct Callback<std::list<Ty, All>>
{
    typedef std::list<Ty, All> container;
    static void Do(container* v)
    {
        for (auto& m : *v)
            Stitcher<typename container::value_type>::Do(&m);
    }
    static void UnDo(container* v)
    {
        for (auto& m : *v)
            Stitcher<typename container::value_type>::UnDo(&m);
    }
};

#pragma endregion //list

}

#endif //INCLUDE_WC_STL_H