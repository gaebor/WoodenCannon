#ifndef INCLUDE_WC_CORE_H
#define INCLUDE_WC_CORE_H

#include <sys/types.h>
#include <new>
#include <vector>

void* operator new  (size_t count);
void* operator new  (size_t count, const std::nothrow_t& tag) throw();

namespace wc {

// see https://social.msdn.microsoft.com/Forums/en-US/9ac98582-ea26-4349-aafb-5678aca73eef/pointertomember-template-arguments-in-variadic-templates?forum=vcgeneral

//! this is for iterating over the members (compile time)
template <class C, typename ...Arguments>
struct Members
{
	static void Do(C* x);
	static void UnDo(C* x);
};

template <class C, class W, typename ...Arguments>
struct Members<C, W, Arguments...>
{
	static void Do(C* x)
	{
		W::Do(x);
		Members<C, Arguments...>::Do(x);
	}
	static void UnDo(C* x)
	{
		W::UnDo(x);
		Members<C, Arguments...>::UnDo(x);
	}
};

//! packs members together
template <class C>
struct Members<C>
{
	//! empty members do nothing
	static void Do(C* x){}
	static void UnDo(C* x){}
};

template<class C>
struct MyMembers
{
	//! by default, the class C has an empty member list
	typedef Members<C> List;
};

//! custom callback
template<class Class>
class Callback
{
public:
	static void Do(Class* x)
	{
	}
	static void UnDo(Class* x)
	{
	}
};

//! makes the pointers relative
template<class Class>
class Stitcher
{
public:
	static void Do(Class* x)
	{
		Callback<Class>::Do(x);
		MyMembers<Class>::List::Do(x);
	}
	static void UnDo(Class* x)
	{
		MyMembers<Class>::List::UnDo(x);
		Callback<Class>::UnDo(x);
	}
};


//! translates pointers to relative pointers
void memory2buffer(void** p);
//! translates relative pointers to absolute pointers
void buffer2memory(void** p);

//! packs a member
template <class C, size_t offset, typename T>
struct Member {
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
template <class C, size_t offset>
struct Pointer
{
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
template <class C, size_t offset, typename T>
struct Responsible
{
	static void Do(C* c)
	{
		Stitcher<T*>::Do(*(T**)((size_t)c + offset));
		memory2buffer((void**)((size_t)c + offset));
	}
	static void UnDo(C* c)
	{
		buffer2memory((void**)((size_t)c + offset));
		Stitcher<T*>::UnDo(*(T**)((size_t)c + offset));
	}
};

class Serializer
{
public:
	typedef std::vector<unsigned char> BufferType;
private:
	static void callback_one();
	static void callback_two();
public:
	static const BufferType* GetBuffer();
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
		Stitcher<Class>::Do(serialized);
		return GetBuffer();
	}

	template<class Class>
	static void UnDo(Class* place) throw()
	{
		// this thing is a pointer to a broken object
		auto thing = (Class*)(GetBuffer()->data());
		// this is where it gets untangled
		Stitcher<Class>::UnDo(thing);
		//! calls a decent copy constructor, the buffer now contains a proper Class object
		new (place)Class(*thing);
	}
	template<class Class>
	static Class* UnDo()
	{
		auto thing = (Class*)(GetBuffer()->data());
		Stitcher<Class>::UnDo(thing);
		return new Class(*thing);
	}

	static void print_buffer();
	//! sets maximum length of serialized buffer
	/*!
	If there is an already serialized object in the buffer, that might get compromised!
	*/
	static void set_max(size_t bytes);
};

}

#endif
