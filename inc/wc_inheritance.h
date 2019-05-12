#ifndef INCLUDE_WC_PARENTS_H
#define INCLUDE_WC_PARENTS_H

#include <type_traits>

namespace wc {

    template<class C> struct Stitcher;

    //! this is for iterating over the parents (compile time)
    template <class C, typename ...Arguments>
    struct Parents
    {
        static void Do(C* x);
     
        template<typename F>
        static void Custom(F& f, const C* x);
        
        static void UnDo(C* x);
    };

    template <class C, class P, typename ...Arguments>
    struct Parents<C, P, Arguments...>
    {
        static_assert(std::is_base_of<P, C>::value, "Not a base class!");
        typedef Parents<C, Arguments...> Next;
        template<typename F>
        static void Custom(F& f, const C* x)
        {
            Stitcher<P>::Custom(f, static_cast<const P*>(x));
            Next::Custom(f, x);
        }
        static void Do(C* x)
        {
            Stitcher<P>::Do(static_cast<P*>(x));
            Next::Do(x);
        }
        static void UnDo(C* x)
        {
            Stitcher<P>::UnDo(static_cast<P*>(x));
            Next::UnDo(x);
        }
    };

    //! packs parents together
    template <class C>
    struct Parents<C>
    {
        //! if there aren't any parents then does nothing
        static void Do(C* x){}

        template<typename F>
        static void Custom(F& f, const C* x){}

        static void UnDo(C* x){}
    };

    //! by default, the class C has no parents
    template<class C>
    struct ParentsOf : public Parents<C>
    {
    };
}

#endif // INCLUDE_WC_PARENTS_H
