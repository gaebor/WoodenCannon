#ifndef INCLUDE_WC_LIST_MSC
#define INCLUDE_WC_LIST_MSC

#include "wc_core.h"

#include <list>
#include <type_traits>

namespace wc{

    template <class Ty, class All>
    struct MyMembers<std::list<Ty, All>> : MembersHelper<std::list<Ty, All>>
    {
        typedef std::list<Ty, All> container;
        typedef typename container::_Nodeptr NodePtr;
        typedef typename std::remove_pointer<NodePtr>::type NodeType;
        typedef Members < container,
#ifdef _DEBUG
            typename R<offsetof(container, _Myproxy), std::_Container_proxy>::Type,
#endif // _DEBUG
            typename P<offsetof(container, _Myhead)>::Type,
            typename M<offsetof(container, _Mysize), size_t>::Type> List;
    };

    template<class Type, class Ptr>
    struct MyMembers<std::_List_node<Type, Ptr>> : MembersHelper<std::_List_node<Type, Ptr>>
    {
        typedef std::_List_node<Type, Ptr> Self;
        typedef Members<Self,
            typename P<offsetof(Self, _Next)>::Type,
            typename P<offsetof(Self, _Prev)>::Type,
            typename M<offsetof(Self, _Myval), Type>::Type> List;
    };

    template <class Ty, class All>
    struct Callback<std::list<Ty, All>>
    {
        typedef std::list<Ty, All> container;
        static void Do(container* v)
        {
            auto ptr = v->_Myhead;
            for (size_t i = 0; i <= v->_Mysize; ++i)
            {
                auto const next = ptr->_Next;
                Stitcher<std::remove_pointer<decltype(ptr)>::type>::Do(ptr);
                ptr = next;
            }
        }
        static void UnDo(container* v)
        {
            auto ptr = v->_Myhead;
            Stitcher<std::remove_pointer<decltype(ptr)>::type>::UnDo(ptr);
            for (size_t i = 0; i < v->_Mysize; ++i)
            {
                ptr = ptr->_Next;
                Stitcher<std::remove_pointer<decltype(ptr)>::type>::UnDo(ptr);
            }
        }
    };
}

#endif //INCLUDE_WC_LIST_MSC
