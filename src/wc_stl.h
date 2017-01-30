#ifndef INCLUDE_WC_STL_H
#define INCLUDE_WC_STL_H

#include <stddef.h>

#include "wc_core.h"

#include <vector>
#include <list>

namespace wc{

#pragma region vector

template <class Ty, class All>
struct MyMembers<std::vector<Ty, All>> : public MembersHelper<std::vector<Ty, All>>
{
    typedef std::vector<Ty, All> container;
#ifdef _MSC_VER
    typedef Members < container,
        typename P<offsetof(container, _Myfirst)>::Type,
        typename P<offsetof(container, _Mylast) >::Type,
        typename P<offsetof(container, _Myend)  >::Type> List;
#else
    // hope for the best
    typedef Members < container, Pointer<container, 0>, Pointer<container, 8>, Pointer<container, 16>> List;
#endif // PLATFORM
};

template <class Ty, class All>
struct Callback<std::vector<Ty, All>>
{
    typedef std::vector<Ty, All> container;
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