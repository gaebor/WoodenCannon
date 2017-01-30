#include "wc_core.h"

#include <stdlib.h>

#include <algorithm>
#include <iostream>

namespace wc {

static BufferType buffer; // TODO thread safe
static std::bad_alloc nomem;

static bool serialize = false; // TODO thread safe

void Serializer::callback_one()
{
    buffer.clear();
    buffer.reserve(1 << 20); // TODO stitching
    serialize = true;
}

void Serializer::callback_two()
{
    serialize = false;
}

const BufferType* GetBuffer()
{
    return &buffer;
}

void Serializer::print_buffer()
{
    std::cout << buffer.size() << " bytes";
    for (size_t i = 0; i < buffer.size(); ++i)
    {
        if (i % sizeof(void*) == 0)
        {
            std::cout.fill(' ');
            std::cout << "\n"; std::cout.width(4); std::cout << i << ". ("; std::cout.width(16); std::cout << i + (*(size_t*)(buffer.data() + i)) << ") ";
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

void memory2buffer(void** p)
{
    *p = (void*)((std::ptrdiff_t)(*p) - (std::ptrdiff_t)p);
}

void buffer2memory(void** p)
{
    *p = (void*)((std::ptrdiff_t)(*p) + (std::ptrdiff_t)p);
}

bool WriteBuffer(FILE* f)
{
    return fwrite(buffer.data(), sizeof(BufferType::value_type), buffer.size(), f) == buffer.size();
}

bool ReadBuffer(FILE* f, size_t s)
{
    if (s == 0)
    {
        fseek(f, 0, SEEK_END); // TODO 64 bit files
        s = ftell(f);
        fseek(f, 0, SEEK_SET);
    }
    buffer.resize(s);
    return fread(buffer.data(), sizeof(BufferType::value_type), s, f) == buffer.size();
}

}

void* operator new (size_t count, const std::nothrow_t& tag) throw()
{
    if (wc::serialize)
    {
        const auto former_size = wc::buffer.size();
        const auto former_size_round = wc::RoundD<decltype(former_size), std::alignment_of<void*>::value>::Do(former_size); // ((former_size + (sizeof(void*) - 1)) / sizeof(void*)) * sizeof(void*);
        wc::serialize = false; // TODO write C-style buffer in order to eliminate further calls of operator-new!
        try{
            wc::buffer.resize(former_size_round + count);
        }
        catch (...)
        {
            return nullptr;
        }
        wc::serialize = true;
        return wc::buffer.data() + former_size_round;
    }
    else
        return malloc(count);
}

void* operator new (size_t count)
{
    void * const p = ::operator new(count, std::nothrow);
    if (!p)
        throw wc::nomem;
    else
        return p;
}