#ifndef INCLUDE_WC_CORE_H
#define INCLUDE_WC_CORE_H

#include <sys/types.h>
#include <new>
#include <vector>
#include <type_traits>
#include <stdio.h>
#include <stddef.h>

#include "wc_inheritance.h"
#include "wc_member.h"
#include "wc_network.h"

void* operator new  (size_t count);
void* operator new  (size_t count, const std::nothrow_t& tag) throw();

namespace wc {

#define WC_ERROR_MESSAGE1 "This object is not trivially copyable, please override its serialization method. See http://www.cplusplus.com/reference/type_traits/is_trivially_copyable/"

// see https://social.msdn.microsoft.com/Forums/en-US/9ac98582-ea26-4349-aafb-5678aca73eef/pointertomember-template-arguments-in-variadic-templates?forum=vcgeneral

//! custom callback
template<class Class>
class Callback
{
public:
    static void Do(Class* x){}
    static void UnDo(Class* x){}
};

/************************************************************************/
/* Stitcher                                                             */
/************************************************************************/

template<class Class, bool>
class StitcherProxy
{
public:
    static void Do(Class* x)
    {
        Callback<Class>::Do(x);
        MyParents<Class>::List::Do(x);
        MyMembers<Class>::List::Do(x);
    }
    static void UnDo(Class* x)
    {
        MyMembers<Class>::List::UnDo(x);
        MyParents<Class>::List::UnDo(x);
        Callback<Class>::UnDo(x);
    }
};

template<class Class>
class Stitcher
{
public:
    static void Do(Class* x)
    {
        StitcherProxy<Class, std::is_fundamental<Class>::value>::Do(x);
    }
    static void UnDo(Class* x)
    {
        StitcherProxy<Class, std::is_fundamental<Class>::value>::UnDo(x);
    }
};

template<class Int, size_t d>
struct RoundD
{
    static Int Do(const Int& i)
    {
        return ((i + d - 1) / d)*d;
    }
};

//! translates pointers to relative pointers
void memory2buffer(void** p);
//! translates relative pointers to absolute pointers
void buffer2memory(void** p);

/************************************************************************/
/* Various types of members                                             */
/************************************************************************/
//! packs an ordinary member
template <class C, size_t _offset, typename T>
struct Member {
    static const size_t offset = _offset;
    typedef T Type;
    typedef C Base;

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
struct Pointer
{
    static const size_t offset = _offset;
    typedef T* Type;
    typedef C Base;

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
struct Responsible
{
    static const size_t offset = _offset;
    typedef T* Type;
    typedef C Base;

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
};

//! No way, man!
/*!
    see http://stackoverflow.com/questions/9889488/can-we-make-a-class-copy-constructor-virtual-in-c
*/
template <class C, size_t offset, typename T>
struct Polymorphic
{
};

template<class C>
struct MembersHelper
{
    template<size_t offset, typename T>
    struct M
    {
        typedef Member<C, offset, T> Type;
    };
    template<size_t offset, typename T = void>
    struct P
    {
        typedef Pointer<C, offset, T> Type;
    };
    template<size_t offset, typename T>
    struct R
    {
        typedef Responsible<C, offset, T> Type;
    };
};

typedef std::vector<unsigned char> BufferType;
const BufferType* GetBuffer();
//! Writes the current buffer into a file, as-is
bool WriteBuffer(FILE*);
//! Reads the content of a file into the current buffer, as-is
/*!
    @param s reads only s bytes from file, zero length means the all of it
*/
bool ReadBuffer(FILE*, size_t s = 0);

class Serializer
{
private:
    static void callback_one();
    static void callback_two();
public:
    //! Serializes a given object
    /*!
    @param object a pointer to the object to serialize
    @return pointer to the buffer, in which the serialized object has just appeared
    */
    template<class Class>
    static const BufferType* Do(const Class* object)
    {
        callback_one();
        // this is where the magic happens
        auto serialized = new Class(*object);
        callback_two();
        // this is where the stitching happens
        Stitcher<Class>::Do(serialized);
        return GetBuffer();
    }

    template<class Class>
    static void UnDo(Class* place) throw()
    {
        // this thing is a pointer to a broken object
        auto thing = (Class*)(GetBuffer()->data());
        // this is where it gets untangled (un-stitched)
        Stitcher<Class>::UnDo(thing);
        // calls a decent copy constructor, the buffer now contains a proper Class object
        new (place)Class(*thing);
    }
    template<class Class>
    static Class* UnDo()
    {
        auto thing = (Class*)(GetBuffer()->data());
        Stitcher<Class>::UnDo(thing);
        return new Class(*thing);
    }
    //! pretty prints the content of the buffer to stdout
    static void print_buffer();
    //! sets maximum length of serialized buffer
    /*!
    If there is an already serialized object in the buffer, that might get compromised!
    */
    static void set_max(size_t bytes);
};

}

#endif
