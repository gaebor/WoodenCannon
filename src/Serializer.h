#ifndef INCLUDE_SERIALIZER_H
#define INCLUDE_SERIALIZER_H

#include <sys/types.h>
#include <new>
#include <vector>

void* operator new  (size_t count);
//void* ::operator new  (size_t count, void* ptr) NO_THROW;
void* operator new  (size_t count, const std::nothrow_t& tag) throw();

//void* ::operator new[](size_t count);
//void* ::operator new[](size_t count, void* ptr) NO_THROW;
//void* ::operator new[](size_t count, const std::nothrow_t& tag) NO_THROW;

class Serializer
{
public:
	typedef std::vector<unsigned char> BufferType;
private:
	static void callback_one();
	static const BufferType* callback_two();
public:
	//! watch this!
	template<class Class>
	static const BufferType* Do(Class* object) throw()
	{
		callback_one();
		new (std::nothrow) Class(*object);
		return callback_two();
	}
	static void print_buffer();
	//! sets maximum length of serialized buffer
	/*!
		If there is an already serialized object in the buffer, that might get compromised!
	*/
	static void set_max(size_t bytes);
};

#endif //INCLUDE_SERIALIZER_H