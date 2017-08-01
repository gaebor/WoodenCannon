namespace wc {

    template <class Ty, class All>
    struct VectorHelper
    {
        typedef std::vector<Ty, All> container;
        typedef std::_Vector_base<Ty, All> Base;
        typedef Pointer<container, offsetof(Base, _M_impl._M_start)> begin;
        typedef Pointer<container, offsetof(Base, _M_impl._M_finish)> end;
        typedef Pointer<container, offsetof(Base, _M_impl._M_end_of_storage)> endend;

		typedef Members<container, begin, end, endend> Hacker;
    };

    template <class Ty, class All>
    struct MembersOf<std::vector<Ty, All>> :
        Members<std::vector<Ty, All>,
            typename VectorHelper<Ty, All>::begin,
            typename VectorHelper<Ty, All>::end,
            typename VectorHelper<Ty, All>::endend
        >
    {};

}