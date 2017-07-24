namespace wc
{
    typedef std::__detail::_List_node_base nodebase;

    template<>
    struct MembersOf<nodebase> :
        Members<nodebase,
            Pointer<nodebase, offsetof(nodebase, _M_next)>,
            Pointer<nodebase, offsetof(nodebase, _M_prev)>
        >
    {};

    template<class Type>
    struct MembersOf<std::_List_node<Type>> :
        Members<std::_List_node<Type>,
            Member<std::_List_node<Type>, offsetof(std::_List_node<Type>, _M_data), Type>
        >
    {};

    template<class Type>
    struct ParentsOf<std::_List_node<Type>> :
        Parents<std::_List_node<Type>, nodebase>
    {};

    template <class Ty, class All>
    struct Callback<std::list<Ty, All>> : std::list<Ty, All>
    {
        typedef std::list<Ty, All> container;
        static void Do(container* v)
        {
            auto const head = &(((Callback<container>*)(v))->_M_impl._M_node);
            auto ptr = head->_M_next;
            Stitcher<nodebase>::Do(head);
            while (ptr != head)
            {
                auto const next = ptr->_M_next;
                Stitcher<std::_List_node<Ty>>::Do(
                    static_cast<std::_List_node<Ty>*>(ptr)
                    );
                ptr = next;
            }
        }
        static void UnDo(container* v)
        {
            auto const head = &(((Callback<container>*)(v))->_M_impl._M_node);
            Stitcher<nodebase>::UnDo(head);
            for (auto ptr = head->_M_next; ptr != head; ptr = ptr->_M_next)
            {
                Stitcher<std::_List_node<Ty>>::UnDo(
                    static_cast<std::_List_node<Ty>*>(ptr)
                    );
            }
        }
    };

}
