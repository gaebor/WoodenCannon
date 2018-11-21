#include <new>
#include <cstddef>
//#include <type_traits>

#include "wc_functions.h"
#include "wc_buffer.h"

#if __cplusplus > 201100L || _MSC_VER >= 1900
#   define THREAD thread_local
#else 
#   define THREAD 
#endif

namespace wc {
    static const std::bad_alloc nomem;
    THREAD Buffer buffer;
    THREAD bool serialize;
}

#if __GNUC__ == 4 && (__GNUC_MINOR__ < 9)
// see https://gcc.gnu.org/bugzilla/show_bug.cgi?id=56019
#define max_align_t_namespace 
#else 
#define max_align_t_namespace std
#endif

typedef wc::RoundD<size_t, alignof(max_align_t_namespace::max_align_t)> rounder;

void* operator new(size_t count)
{
    if (wc::serialize)
    {
        const auto former_size = wc::buffer.GetSize();
        const auto former_size_round = rounder::Do(former_size);

        wc::serialize = false;
        //--------------------
        if (former_size_round != former_size)
            wc::buffer.Allocate(former_size_round - former_size);
        auto result = wc::buffer.Allocate(count);
        // wc::allocated_pointers.insert(result);
        //--------------------
        wc::serialize = true;
        return result;
    }
    else
    {
        auto const ptr = malloc(count);
        if (ptr)
            return ptr;
        else
            throw wc::nomem;
    }
}

void* operator new (size_t count, const std::nothrow_t&) noexcept
{
    try {
        return operator new (count);
    }
    catch (std::exception&) {
        return nullptr;
    }
}

void* operator new[](size_t count)
{
    return operator new(count);
}

void* operator new[](size_t count, const std::nothrow_t&) noexcept
{
    return operator new (count, std::nothrow);
}

void operator delete(void* ptr) noexcept
{
    if (!wc::serialize)
        free(ptr);
}

void operator delete(void* ptr, const std::nothrow_t&) noexcept
{
    operator delete(ptr);
}

void operator delete[](void* ptr)
{    
    operator delete(ptr);
}

void operator delete[](void* ptr, const std::nothrow_t&) noexcept
{
    operator delete(ptr);
}

void operator delete(void* ptr, size_t)
{
    operator delete(ptr);
}

void operator delete[](void* ptr, size_t)
{
    operator delete(ptr);
}
