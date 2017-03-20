#ifndef INCLUDE_WC_CORE_H
#define INCLUDE_WC_CORE_H

#include <type_traits>

#include "wc_inheritance.h"
#include "wc_member.h"

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
    template<class F>
    static void Custom(F f, Class* x)
    {
        MyParents<Class>::List::Custom(f, x);
        MyMembers<Class>::List::Custom(f, x);
    }
    static void Do(Class* x)
    {
        StitcherProxy<Class, std::is_fundamental<Class>::value>::Do(x);
    }
    static void UnDo(Class* x)
    {
        StitcherProxy<Class, std::is_fundamental<Class>::value>::UnDo(x);
    }
};

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

    //! reconstructs an object from memory, in-place version
    template<class Class>
    static void UnDo(Class* place, unsigned char* data) throw()
    {
        // this thing is a pointer to a broken object
        auto thing = (Class*)data;
        // this is where it gets untangled (un-stitched)
        Stitcher<Class>::UnDo(thing);
        // calls a decent copy constructor, the buffer now contains a proper Class object
        new (place)Class(*thing);
    }
    //! reconstructs an object from memory
    /*!
        @param data should hold the uncorrupted, serialized data.
        The length of the data is not checked, user should guarantee that the whole object is there
        The pointed array in the memory shall be modified during the process.
        After the reconstruction one cannot reconstruct an other instance.
    */
    template<class Class>
    static Class* UnDo(unsigned char* data)
    {
        auto thing = (Class*)data;
        Stitcher<Class>::UnDo(thing);
        return new Class(*thing);
    }
};

}

#endif
