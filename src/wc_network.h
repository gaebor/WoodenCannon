#ifndef INCLUDE_WC_NETWORK_H
#define INCLUDE_WC_NETWORK_H

#include <stddef.h>

namespace wc
{
    template<size_t size>
    struct ByteReorder
    {
        static void Do(void* c){}
        static void UnDo(void* c){}
    };

#ifdef WC_NETWORK
    //! reorders the bits in a byte from native to network representation
    /*!
        bool occupies only one byte, but its representation is not defined!
    */
    void reorderbool(bool* x);
    //! de-orders the bits in a byte from network order to native representation
    void deorderbool(bool* x);
    
    //! reorders two bytes from native to network order
    void reorder2(void* x);
    //! de-orders two bytes from network to native order
    void deorder2(void* x);

    //! reorders four bytes from native to network order
    void reorder4(void* x);
    //! de-orders four bytes from network to native order
    void deorder4(void* x);
    
    //! reorders eight bytes from native to network order
    void reorder8(void* x);
    //! de-orders eight bytes from network to native order
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