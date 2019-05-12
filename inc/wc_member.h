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
        template<typename F>
        static void Custom(F& f, const C* c)
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
        static void Custom(F& f, const C* x);
    };

    //! iteratively calls Member::Do on argument list
    template <class C, class M, typename ...Arguments>
    struct Members<C, M, Arguments...>
    {
        typedef Members<C, Arguments...> Next;
        static void Do(C* x)
        {
            M::Do(x);
            Next::Do(x);
        }
        static void UnDo(C* x)
        {
            M::UnDo(x);
            Next::UnDo(x);
        }
        template<typename F>
        static void Custom(F& f, const C* x)
        {
            M::Custom(f, x);
            Next::Custom(f, x);
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
        static void Custom(F& f, const C* x){}
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
        template<typename F>
        static void Custom(F& f, const C* c)
        {
			Stitcher<T*>::Custom(f, (T**)((size_t)c + offset));
            F f2 = f;
            f2[(void*)(*(T**)((size_t)c + offset))];
            Stitcher<T>::Custom(f2, *(T**)((size_t)c + offset));
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

    template<class _Ty>
    struct is_member_object_pointer : std::false_type
    {
        typedef void MemberType;
        typedef void ClassType;
    };

    template<class _Ty1, class _Ty2>
    struct is_member_object_pointer<_Ty1 _Ty2::*> : std::true_type
    {
        typedef _Ty1 MemberType;
        typedef _Ty2 ClassType;
    };
    
    //! represents a member of a class in general
    /*!
        see https://stackoverflow.com/a/5281732/3583290
    */
    template<typename V, V p>
    struct AbstractMember2 : std::integral_constant<V, p>
    {
    private:
        typedef is_member_object_pointer<V> Info;
        static_assert(Info::value, "Template parameter is not a pointer-to-member!");
    public:
        typedef typename is_member_object_pointer<V>::MemberType MemberType;
        typedef typename is_member_object_pointer<V>::ClassType ClassType;
        static constexpr MemberType* apply(ClassType* c)
        {
            return &(c->*p);
        }
        static constexpr const MemberType* apply(const ClassType* c)
        {
            return &(c->*p);
        }
        template<typename F>
        static void Custom(F& f, const ClassType* c)
        {
            Stitcher<MemberType>::Custom(f, apply(c));
        }
    };
    
    //! packs an ordinary member
    template<typename V, V p>
    struct Member2 : AbstractMember2<V, p>
    {
        using typename AbstractMember2<V, p>::ClassType;
        using typename AbstractMember2<V, p>::MemberType;
        using AbstractMember2<V, p>::apply;

        static void Do(ClassType* c)
        {
            Stitcher<MemberType>::Do(apply(c));
        }
        static void UnDo(ClassType* c)
        {
            Stitcher<MemberType>::UnDo(apply(c));
        }
    };

    //! a member pointer, which is not to be copied
    template<typename V, V p>
    struct Pointer2 : AbstractMember2<V, p>
    {
        using typename AbstractMember2<V, p>::ClassType;
        using typename AbstractMember2<V, p>::MemberType;
        using AbstractMember2<V, p>::apply;

        static_assert(std::is_pointer<MemberType>::value, "This member is not a pointer!");
        static void Do(ClassType* c)
        {
            memory2buffer(reinterpret_cast<void**>(apply(c)));
        }
        static void UnDo(ClassType* c)
        {
            buffer2memory(reinterpret_cast<void**>(apply(c)));
        }
    };

    //! a member pointer, the object is responsible for it
    template<typename V, V p>
    struct Responsible2 : AbstractMember2<V, p>
    {
        using typename AbstractMember2<V, p>::ClassType;
        using typename AbstractMember2<V, p>::MemberType;
        using AbstractMember2<V, p>::apply;

        static void Do(ClassType* c)
        {
            Stitcher<typename std::remove_pointer<MemberType>::type>::Do(*apply(c));
            memory2buffer(reinterpret_cast<void**>(apply(c)));
        }
        static void UnDo(ClassType* c)
        {
            buffer2memory(reinterpret_cast<void**>(apply(c)));
            Stitcher<typename std::remove_pointer<MemberType>::type>::UnDo(*apply(c));
        }
        template<typename F>
        static void Custom(F& f, const ClassType* c)
        {
			Stitcher<MemberType>::Custom(f, apply(c));
            F f2 = f;
            f2[reinterpret_cast<void*>(*apply(c))];
            Stitcher<typename std::remove_pointer<MemberType>::type>::Custom(f2, *apply(c));
        }
    };

}

#endif //INCLUDE_WC_MEMBER_H
