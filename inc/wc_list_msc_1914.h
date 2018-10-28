#include <type_traits>

namespace wc {

    template <class _Ty, class _Alloc>
    struct ParentsOf<std::list<_Ty, _Alloc>> :
        Parents<std::list<_Ty, _Alloc>, std::_List_buy<_Ty, _Alloc>>
    {
    };

    template <class _Ty, class _Alloc>
    struct ParentsOf<std::_List_buy<_Ty, _Alloc>> :
        Parents<std::_List_buy<_Ty, _Alloc>, std::_List_alloc<std::_List_base_types<_Ty, _Alloc>>>
    {
    };

    template <class _Alloc>
    struct MembersOf<std::_List_alloc<_Alloc>> :
        Members<std::_List_alloc<_Alloc>,
            Member<std::_List_alloc<_Alloc>, 0, std::_List_val<typename _Alloc::_Val_types>>
        >
    {
        static_assert(sizeof(std::_List_alloc<_Alloc>) == sizeof(std::_List_val<typename _Alloc::_Val_types>),
            "sizeof(_Compressed_pair<_Ty1, _Ty2>) != sizeof(_Ty2) for _Ty2 = std::_List_val");
    };

    template <class _Alloc>
    struct ParentsOf<std::_List_val<_Alloc>> :
        Parents<std::_List_val<_Alloc>, std::_Container_base>
    {};

    template <class _Alloc>
    struct MembersOf<std::_List_val<_Alloc>> :
        Members<std::_List_val<_Alloc>,
            Responsible<std::_List_val<_Alloc>, offsetof(std::_List_val<_Alloc>, _Myhead), typename _Alloc::_Node>,
            Member<std::_List_val<_Alloc>, offsetof(std::_List_val<_Alloc>, _Mysize), typename std::_List_val<_Alloc>::size_type>
        >
    {};

    template<class _Value_type, class _Voidptr>
    struct ListNodeHelper
    {
        typedef std::_List_node<_Value_type, _Voidptr> Node;
        typedef Pointer<Node, offsetof(Node, _Next)> NextType;
        typedef Pointer<Node, offsetof(Node, _Prev)> PrevType;
        typedef Member<Node, offsetof(Node, _Myval), _Value_type> ValType;
    };

    template <class _Value_type, class _Voidptr>
    struct MembersOf<std::_List_node<_Value_type, _Voidptr>> :
        Members<std::_List_node<_Value_type, _Voidptr>,
            typename ListNodeHelper<_Value_type, _Voidptr>::NextType,
            typename ListNodeHelper<_Value_type, _Voidptr>::PrevType,
            typename ListNodeHelper<_Value_type, _Voidptr>::ValType
        >
    {};

    template <class Ty, class All>
    struct Callback<std::list<Ty, All>> : std::list<Ty, All>
    {
        typedef std::list<Ty, All> container;
        static void Do(container* v)
        {
            auto ptr = ((std::_List_val<std::_List_alloc<std::_List_base_types<Ty, All>>>*)v)->_Myhead->_Next;
            auto next = ptr;
            size_t n = ((std::_List_val<std::_List_alloc<std::_List_base_types<Ty, All>>>*)v)->_Mysize;
            while (n > 0)
            {
                next = ptr->_Next;
                Stitcher<std::remove_pointer<decltype(ptr)>::type>::Do(ptr);
                ptr = next;
                --n;
            }
        }
        static void UnDo(container* v)
        {
            auto ptr = ((std::_List_val<std::_List_alloc<std::_List_base_types<Ty, All>>>*)v)->_Myhead->_Next;
            size_t n = ((std::_List_val<std::_List_alloc<std::_List_base_types<Ty, All>>>*)v)->_Mysize;
            // head should be UnStitched at this point
            while (n > 0)
            {
                Stitcher<std::remove_pointer<decltype(ptr)>::type>::UnDo(ptr);
                ptr = ptr->_Next;
                --n;
            }
        }
        template<typename F>
        static void Custom(F& f, const container* v)
        {
            for (const auto& x : *v)
            {
                wc::Stitcher<typename container::value_type>::Custom(f, &x);
            }
        }
    };

}
