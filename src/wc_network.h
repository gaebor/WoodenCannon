#ifndef INCLUDE_WC_NETWORK_H
#define INCLUDE_WC_NETWORK_H

#include "wc_core.h"

namespace wc
{
    template<size_t size>
    struct ByteReorder
    {
        static void Do(void* c){}
        static void UnDo(void* c){}
    };

#ifdef WC_NETWORK
    //! bool should occupy one byte, but its representation is not defined!
    void reorderbool(bool* x);
    void deorderbool(bool* x);
    
    void reorder2(void* x);
    void deorder2(void* x);

    void reorder4(void* x);
    void deorder4(void* x);
    
    void reorder8(void* x);
    void deorder8(void* x);

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
#endif // WC_NETWORK
}

#endif // INCLUDE_WC_NETWORK_H