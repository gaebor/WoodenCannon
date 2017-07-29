#ifndef INCLUDE_WC_CORE_H
#define INCLUDE_WC_CORE_H

#include <type_traits>
#include <stddef.h>

#include "wc_config.h"

#include "wc_stitcher.h"

namespace wc {

//! tells you the properties of WC
/*!
	If you can link against wc and the returned string of this function
	is the same as WC_COMPILED_CONFIG then you should be OK
*/
const char* get_compile_info();

class Serializer
{
private:
    static void callback_one();
    static void callback_two();
    static void callback_three();
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
        auto serialized = new Class(*object);   // this is where the magic happens
        callback_two(); // also a bit magic
        Stitcher<Class>::Do(serialized); // magic continues
        callback_three(); // this is less magic
        return GetBuffer();
    }

    //! reconstructs an object from memory, in-place version
	/*!
		@param target this object is assigned via an `operator=`
		If your class has no operator= (for example is const qualified)
		then this generates a compiler error
	*/
    template<class Class>
    static void UnDo(Class& target, unsigned char* data) throw()
    {
        // this thing is a pointer to a broken object
        auto thing = (Class*)data;
        // this is where it gets untangled (un-stitched)
        Stitcher<Class>::UnDo(thing);
        // calls an `operator=`
		// the buffer now contains a proper Class object
		target = *((Class*)thing);
    }
    //! reconstructs an object from memory
    /*!
        @param data should hold the uncorrupted, serialized data.
        The length of the data is not checked, user should guarantee that the whole object is there
        Also, the function does not provide any information about the consumed bytes (size of the object).
		The pointed array in the memory will be modified during the process.
        After the reconstruction one cannot reconstruct an other instance.
		@return the returned object is made via a copy constructor.
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
