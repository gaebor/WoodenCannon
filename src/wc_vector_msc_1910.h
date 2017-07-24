namespace wc {

    template <class Ty, class All>
    struct MembersOf<std::vector<Ty, All>> : public std::_v
        : MembersHelper<std::vector<Ty, All>>
    {
        typedef std::vector<Ty, All> container;
        typedef Members < container,
#ifdef _DEBUG
            //typename R<offsetof(container, _Myproxy), std::_Container_proxy>::Type,
#endif // _DEBUG
            typename P<offsetof(container, _Mypair._Myval2._Myfirst)>::Type,
            typename P<offsetof(container, _Mypair._Myval2._Mylast) >::Type,
            typename P<offsetof(container, _Mypair._Myval2._Myend)  >::Type> List;
    };

    template <>
    struct MembersOf<std::_Container_proxy> : MembersHelper<std::_Container_proxy>
    {
        typedef Members < std::_Container_proxy,
            // this is left out intentionally!
            //typename P<offsetof(std::_Container_proxy, _Myfirstiter)>::Type,
            typename P<offsetof(std::_Container_proxy, _Mycont)     >::Type > List;
    };

    template <class Ty, class All>
    struct Callback<std::vector<Ty, All>>
    {
        typedef std::vector<Ty, All> container;
#if defined(_DEBUG)
        typedef typename MembersOf<container>::List::Next Hacker;
#else
        typedef typename MembersOf<container>::List Hacker;
#endif
        static void Do(container* v)
        {
            if (v->empty())
            {   //hack!
                auto const end = (void**)(v + 1);
                Hacker::Custom([&end](void** x) {*x = end; }, v);
            }
            else
                for (auto& m : *v)
                    Stitcher<typename container::value_type>::Do(&m);
        }
        static void UnDo(container* v)
        {   //hack!
            auto const end = (void**)(v + 1);
            bool hacked = true;
            Hacker::Custom([&end, &hacked](void** x) {hacked = hacked && (*x == end); }, v);
            if (hacked)
            {
                Hacker::Custom([&end](void** x) {*x = nullptr; }, v);
            }
            else
                for (auto& m : *v)
                    Stitcher<typename container::value_type>::UnDo(&m);
        }
    };

}