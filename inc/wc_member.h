#ifndef INCLUDE_WC_MEMBER_H
#define INCLUDE_WC_MEMBER_H

#include "wc_functions.h"

namespace wc{

    template<class T> struct Stitcher;

    //! represents a member of a class in general
    /*!
    @param C the owner of the member
    @param _offset the position of the member inside the object
    @param T the type of the member
    */
    template <class C, size_t _offset, typename T>
    struct AbstractMember
    {
        static const size_t offset = _offset;
        typedef T Type;
        typedef C Base;
        template<class F>
        static void Custom(F f, C* c)
        {
            Stitcher<T>::Custom(f, (T*)((size_t)c + offset));
        }
    };

    //! this is for iterating over the members (compile time)
    template <class C, typename ...Arguments>
    struct Members
    {
        //! stitch member
        static void Do(C* x);
        //! un-stitch member
        static void UnDo(C* x);
        //! you can call your function for the members in a for loop
        template<typename F>
        static void Custom(F f, C* x);
    };

    //! iteratively calls Member::Do on argument list
    template <class C, class M, typename ...Arguments>
    struct Members<C, M, Arguments...>
    {
        typedef Members<C, Arguments...> Next;
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
        static void Custom(F f, C* x)
        {
            M::Custom(f, x);
            Members<C, Arguments...>::Custom(f, x);
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
        static void Custom(F f, C* x){}
    };

    //! by default, the class C has an empty member list
    template<class C>
    struct MembersOf: public Members<C>
    {
    };

    //! packs an ordinary member
    template <class C, size_t _offset, typename T>
    struct Member : public AbstractMember<C, _offset, T>
    {
        using AbstractMember<C, _offset, T>::offset;
        static void Do(C* c)
        {
            Stitcher<T>::Do((T*)((size_t)c + offset));
        }
        static void UnDo(C* c)
        {
            Stitcher<T>::UnDo((T*)((size_t)c + offset));
        }
    };

    //! a member pointer, which is not to be copied
    template <class C, size_t _offset, typename T = void>
    struct Pointer : public AbstractMember<C, _offset, T*>
    {
        using AbstractMember<C, _offset, T*>::offset;
        static void Do(C* c)
        {
            memory2buffer((void**)((size_t)c + offset));
        }
        static void UnDo(C* c)
        {
            buffer2memory((void**)((size_t)c + offset));
        }
    };

    //! a member pointer, the object is responsible for it
    template <class C, size_t _offset, typename T>
    struct Responsible : public AbstractMember<C, _offset, T*>
    {
        using AbstractMember<C, _offset, T*>::offset;
        static void Do(C* c)
        {
            Stitcher<T>::Do(*(T**)((size_t)c + offset));
            memory2buffer((void**)((size_t)c + offset));
        }
        static void UnDo(C* c)
        {
            buffer2memory((void**)((size_t)c + offset));
            Stitcher<T>::UnDo(*(T**)((size_t)c + offset));
        }
        template<class F>
        static void Custom(F f, C* c)
        {
			Stitcher<T*>::Custom(f, (T**)((size_t)c + offset));
            Stitcher<T>::Custom(f, *(T**)((size_t)c + offset));
        }
    };

    //! No way, man!
    /*!
    see http://stackoverflow.com/questions/9889488/can-we-make-a-class-copy-constructor-virtual-in-c
    */
    template <class C, size_t _offset, typename T>
    struct Polymorphic
    {
    };

    //! handy helper for brevity
    template<class C>
    struct MembersHelper
    {
        template<class T C::*member>
        struct m
        {
            static const size_t offset = ((size_t)&reinterpret_cast<char const volatile&>((((C*)nullptr)->*member)));
            typedef Member<C, offset, T> Type;
        };

        template<size_t _offset, typename T>
        struct M
        {
            typedef Member<C, _offset, T> Type;
        };
        template<size_t _offset, typename T = void>
        struct P
        {
            typedef Pointer<C, _offset, T> Type;
        };
        template<size_t _offset, typename T>
        struct R
        {
            typedef Responsible<C, _offset, T> Type;
        };
    };

}

#endif //INCLUDE_WC_MEMBER_H
