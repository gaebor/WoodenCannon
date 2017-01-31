#ifndef INCLUDE_WC_MEMBER_H
#define INCLUDE_WC_MEMBER_H

namespace wc{
    //! this is for iterating over the members (compile time)
    template <class C, typename ...Arguments>
    struct Members
    {
        static void Do(C* x);
        static void UnDo(C* x);
        template<typename F>
        static void Do(F f, C* x);
    };

    template <class C, class M, typename ...Arguments>
    struct Members<C, M, Arguments...>
    {
        static void Do(C* x)
        {
            M::Do(x);
            Members<C, Arguments...>::Do(x);
        }
        static void UnDo(C* x)
        {
            M::UnDo(x);
            Members<C, Arguments...>::UnDo(x);
        }
        template<typename F>
        static void Do(F f, C* x)
        {
            f((void**)((char*)x + M::offset));
            Members<C, Arguments...>::Do(f, x);
        }
    };

    //! packs members together
    template <class C>
    struct Members<C>
    {
        //! if there aren't any members then does nothing
        static void Do(C* x){}
        static void UnDo(C* x){}
        template<typename F>
        static void Do(F f, C* x){}
    };

    template<class C>
    struct MyMembers
    {
        //! by default, the class C has an empty member list
        typedef Members<C> List;
    };

}

#endif //INCLUDE_WC_MEMBER_H
