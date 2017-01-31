#ifndef INCLUDE_WC_NETWORK_H
#define INCLUDE_WC_NETWORK_H

#include "wc_core.h"

#ifdef WC_NETWORK

namespace wc
{
    void reorderbool(bool* x);
    void deorderbool(bool* x);
    
    void reorder2(void* x);
    void deorder2(void* x);

    void reorder4(void* x);
    void deorder4(void* x);
    
    void reorder8(void* x);
    void deorder8(void* x);

    template<size_t size>
    struct ByteReorder
    {
        static void Do(void* c){}
        static void UnDo(void* c){}
    };

    template<>
    struct ByteReorder<2>
    {
        static void Do(void* c){ reorder2(c); }
        static void UnDo(void* c){ deorder2(c); }
    };

    template<>
    struct ByteReorder<4>
    {
        static void Do(void* c){ reorder4(c); }
        static void UnDo(void* c){ deorder4(c); }
    };

    template<>
    struct ByteReorder<8>
    {
        static void Do(void* c){ reorder8(c); }
        static void UnDo(void* c){ deorder8(c); }
    };

    template<class Class>
    class StitcherProxy<Class, true>
    {
    public:
        static void Do(Class* x)
        {
            ByteReorder<sizeof(Class)>::Do(x);
        }
        static void UnDo(Class* x)
        {
            ByteReorder<sizeof(Class)>::UnDo(x);
        }
    };

    template<>
    class StitcherProxy<bool, true>
    {
    public:
        static void Do(bool* x)
        {
            reorderbool(x);
        }
        static void UnDo(bool* x)
        {
            deorderbool(x);
        }
    };

}

#endif // WC_NETWORK

#endif // INCLUDE_WC_NETWORK_H