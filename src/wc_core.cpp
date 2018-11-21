
#include "wc_core.h"

#include <stdlib.h>
#include <cstddef>
#include <algorithm>
#include <iostream>
#include <map>
#include <unordered_set>

#include "wc_network.h"
#include "wc_buffer.h"

#if __cplusplus > 201100L || _MSC_VER >= 1900
#   define THREAD thread_local
#else 
#   define THREAD 
#endif

namespace wc {
    extern THREAD Buffer buffer;
    extern THREAD bool serialize;

void Serializer::callback_one()
{
    buffer.Clear();
    buffer.Allocate(0);
    serialize = true;
}

void Serializer::callback_two()
{
    serialize = false;
    buffer.CalculateOffsets();
}

void Serializer::callback_three()
{
    buffer.ReArrange();
}

BufferType* GetBuffer()
{
    return buffer.GetFirst();
}

void PrintBuffer()
{
    auto& b = *GetBuffer();
    for (size_t i = 0; i < b.size(); ++i)
    {
        if (i % sizeof(void*) == 0)
        {
            std::cout.fill(' ');
            auto ptr = *(std::ptrdiff_t*)(b.data() + i);
            ByteReorder<sizeof(ptr)>::UnDo(&ptr);
            ptr += i;
            std::cout << "\n"; std::cout.width(4); std::cout << i <<
                ".\t("; std::cout.width(20); std::cout << ptr << ")\t";
        }
        std::cout.fill('0'); std::cout.width(2);
        std::cout << std::hex << (int)(b[i]) << std::dec;
    }
    std::cout << std::endl;
}

void memory2buffer(void** p)
{
    auto const relative = *p ? ((std::ptrdiff_t)(*p) - (std::ptrdiff_t)p + buffer.GetOffset(p)) : std::numeric_limits<std::ptrdiff_t>::min();
    *p = (void*)relative;
    ByteReorder<sizeof(void*)>::Do(p);
}

void buffer2memory(void** p)
{
    ByteReorder<sizeof(void*)>::UnDo(p);
    auto const absolute = (std::ptrdiff_t)(*p) == std::numeric_limits<std::ptrdiff_t>::min() ? (std::ptrdiff_t)(nullptr) : (std::ptrdiff_t)(*p) + (std::ptrdiff_t)p;
    *p = (void*)absolute;
}

bool WriteBuffer(FILE* f)
{
    auto& b = *GetBuffer();
    return fwrite(b.data(), sizeof(BufferType::value_type), b.size(), f) == b.size();
}

bool ReadBuffer(FILE* f, size_t s)
{
    if (s == 0)
    {
        fseek(f, 0, SEEK_END); // TODO 64 bit files
        s = ftell(f);
        fseek(f, 0, SEEK_SET);
    }
    buffer.Clear();
    auto data = buffer.Allocate(s);
    return fread(data, sizeof(BufferType::value_type), s, f) == sizeof(BufferType::value_type) * s;
}

const char* get_compile_info()
{
    return WC_COMPILED_CONFIG;
}

}
