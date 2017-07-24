namespace wc {

    template <class Ty, class All>
    struct VectorHelper : MembersHelper<std::vector<Ty, All>>
    {
        typedef std::vector<Ty, All> container;
        typedef std::_Vector_base<Ty, All> Base;
        typedef Pointer<container, offsetof(Base, _M_impl._M_start)> start;
        typedef Pointer<container, offsetof(Base, _M_impl._M_finish)> finish;
        typedef Pointer<container, offsetof(Base, _M_impl._M_end_of_storage)> end;
    };

    template <class Ty, class All>
    struct MembersOf<std::vector<Ty, All>> :
        Members<std::vector<Ty, All>,
            typename VectorHelper<Ty, All>::start,
            typename VectorHelper<Ty, All>::finish,
            typename VectorHelper<Ty, All>::end
        >
    {};

    template <class Ty, class All>
    struct Callback<std::vector<Ty, All>>
    {
        typedef std::vector<Ty, All> container;
        typedef MembersOf<container> Hacker;
        static void Do(container* v)
        {
            if (v->empty())
            {   //hack!
                auto const end = (void**)(v + 1);
                Hacker::Custom([&end](size_t x) {*(void**)x = end; }, v);
            }
            else
                for (auto& m : *v)
                    Stitcher<typename container::value_type>::Do(&m);
        }
        static void UnDo(container* v)
        {   //hack!
            auto const end = (void**)(v + 1);
            bool hacked = true;
            Hacker::Custom([&end, &hacked](size_t x) {hacked = (hacked && (*(void**)x == end)); }, v);
            if (hacked)
            {
                Hacker::Custom([&end](size_t x) {*(void**)x = nullptr; }, v);
            }
            else
                for (auto& m : *v)
                    Stitcher<typename container::value_type>::UnDo(&m);
        }
    };

}