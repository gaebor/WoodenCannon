#ifndef WC_LIST_GNUC_H
#define WC_LIST_GNUC_H

#include "wc_core.h"
#include <list>

namespace wc
{
    typedef std::__detail::_List_node_base nodebase;

    template<>
    struct MyMembers<nodebase> : MembersHelper<nodebase>
    {
        typedef Members<nodebase,
            Pointer<nodebase, offsetof(nodebase, _M_next)>,
            Pointer<nodebase, offsetof(nodebase, _M_prev)>
        > List;
    };

    template<class Type>
    struct MyMembers<std::_List_node<Type>> : MembersHelper<std::_List_node<Type>>
    {
        typedef std::_List_node<Type> Self;
        typedef Members<Self, 
            Member<Self, offsetof(Self, _M_data), Type>
        > List;
    };

    template<class Type>
    struct MyParents<std::_List_node<Type>>
    {
        typedef Parents<std::_List_node<Type>, nodebase> List;
    };

    template <class Ty, class All>
    struct Callback<std::list<Ty, All>> : std::list<Ty, All>
    {
        typedef std::list<Ty, All> container;
        static void Do(container* v)
        {
            auto const wtf = &(((Callback<container>*)(v))->_M_impl._M_node);
            auto ptr = wtf->_M_next;
            Stitcher<nodebase>::Do(wtf);
            do
            {
                auto const next = ptr->_M_next;
                Stitcher<std::_List_node<Ty>>::Do(
                    static_cast<std::_List_node<Ty>*>(ptr)
                    );
                ptr = next;
            } while (ptr != wtf);
        }
        static void UnDo(container* v)
        {
            auto const wtf = &(((Callback<container>*)(v))->_M_impl._M_node);
            Stitcher<nodebase>::UnDo(wtf);
            for (auto ptr = wtf->_M_next; ptr != wtf; ptr = ptr->_M_next)
            {
                Stitcher<std::_List_node<Ty>>::UnDo(
                    static_cast<std::_List_node<Ty>*>(ptr)
                    );
            }
        }
    };

}

#endif //WC_LIST_GNUC_H
