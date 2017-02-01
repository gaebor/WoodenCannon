#ifndef INCLUDE_WC_VECTOR_H
#define INCLUDE_WC_VECTOR_H

#include "wc_core.h"
#include <vector>

namespace wc{

    template <class Ty, class All>
    struct MyMembers<std::vector<Ty, All>>
        :   MembersHelper<std::vector<Ty, All>>
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
        typedef std::_Vector_base<Ty, All> Base;
        typedef Members < container,
            Pointer<container, offsetof(Base, _M_impl._M_start)>,
            Pointer<container, offsetof(Base, _M_impl._M_finish)>,
            Pointer<container, offsetof(Base, _M_impl._M_end_of_storage)>
        > List;
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
            }
            else
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

}

#endif //INCLUDE_WC_VECTOR_H
