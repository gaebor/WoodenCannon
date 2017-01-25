#include "Serializer.h"

#include <stdlib.h>

#include <algorithm>
#include <iostream>

static Serializer::BufferType buffer; // TODO thread safe
static const std::bad_alloc nomem;
static std::vector<void*> pointers; // TODO thread safe

static bool serialize = false; // TODO thread safe

void* operator new (size_t count, const std::nothrow_t& tag) throw()
{
	if (serialize)
	{
		const auto former_size = buffer.size();
		const auto former_size_round = ((former_size + (sizeof(void*) - 1)) / sizeof(void*)) * sizeof(void*);
		serialize = false; // TODO write C-style buffer in order to eliminate further calls of operator-new!
		try{
			pointers.emplace_back(buffer.data() + former_size_round); // also do the stitching!
			// buffer.resize(former_size + count);
			buffer.resize(former_size_round + count);
		}catch (...)
		{
			return nullptr;
		}
		serialize = true;
		return buffer.data() + former_size_round;
	}else
		return malloc(count);
}

void* operator new (size_t count)
{
	void * const p = ::operator new(count, std::nothrow);
	if (!p)
		throw nomem;
	else
		return p;
}

void Serializer::callback_one()
{
	buffer.clear();
	buffer.reserve(1 << 20); // TODO stitching
	serialize = true;
	pointers.clear();
}

const Serializer::BufferType* Serializer::callback_two()
{
	serialize = false;
	// TODO end pointer in std::vector (and possibly others)
	// pointers.emplace_back(buffer.data()+buffer.size());
	return &buffer;
}

void Serializer::print_buffer()
{
	std::cout << buffer.size() << " bytes";
	auto const begin = buffer.data();
	for (size_t i = 0; i < buffer.size(); ++i)
	{
		if (i % sizeof(void*) == 0)
		{
			std::cout.fill(' ');
			std::cout << "\n"; std::cout.width(4); std::cout << i << ". ";
			auto const pp = *(void**)(begin + i);
			if (std::find(pointers.begin(), pointers.end(), pp) != pointers.end())
			{
				std::cout << "->"; std::cout.width(6); std::cout << ((size_t)(pp) - (size_t)begin);
				i += sizeof(void*) - 1;
				continue;
			}
		}
		std::cout.fill('0'); std::cout.width(2);
		std::cout << std::hex << (int)(buffer[i]) << std::dec;
	}
	std::cout << std::endl;
}

void Serializer::set_max(size_t bytes)
{
	buffer.reserve(bytes);
}
