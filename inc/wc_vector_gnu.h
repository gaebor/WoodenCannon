namespace wc {

    template <class container>
    struct VectorHelper
    {
        typedef std::_Vector_base<typename container::value_type, typename container::allocator_type> Base;
        typedef Pointer<container, offsetof(Base, _M_impl._M_start)> begin;
        typedef Pointer<container, offsetof(Base, _M_impl._M_finish)> end;
        typedef Pointer<container, offsetof(Base, _M_impl._M_end_of_storage)> endend;

		typedef Members<container, begin, end, endend> Hacker;
    };

    template <class Ty, class All>
    struct MembersOf<std::vector<Ty, All>> :
        Members<std::vector<Ty, All>,
            typename VectorHelper<std::vector<Ty, All>>::begin,
            typename VectorHelper<std::vector<Ty, All>>::end,
            typename VectorHelper<std::vector<Ty, All>>::endend
        >
    {};

}