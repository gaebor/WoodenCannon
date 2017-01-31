#include "wc_network.h"

#ifdef WC_NETWORK

#ifdef _MSC_VER
#   include <WinSock2.h>
#elif __GNUC__
#   include <arpa/inet.h>
#   include <byteswap.h>
#else

#endif // _MSC_VER

namespace wc {
    void reorderbool(bool* x)
    {
        switch (*((char*)x) == 0)
        {
        case false:
            *((char*)x) = -1;
            break;
        default:
            break;
        }
    }
    void deorderbool(bool* x)
    {
        // I hope this is how they define it
        // although it is totally compiler dependent
        *x = (*((char*)x) != 0);
    }

    void reorder2(void* x)
    {
        short& s = *((short*)x);
        s = htons(s);
    }
    void deorder2(void* x)
    {
        short& s = *((short*)x);
        s = ntohs(s);
    }

    void reorder4(void* x)
    {
        int& i = *((int*)x);
        i = htonl(i);
    }
    void deorder4(void* x)
    {
        int& i = *((int*)x);
        i = ntohl(i);
    }

    void reorder8(void* x)
    {
        long long& l = *((long long*)x);
#ifdef __GNUC__
        l = bswap_64(l);
#else
        l = htonll(l);
#endif // __GNUC__
    }
    void deorder8(void* x)
    {
        long long& l = *((long long*)x);
#ifdef __GNUC__
        l = bswap_64(l);
#else
        l = ntohll(l);
#endif // __GNUC__        
    }
}

#endif // WC_NETWORK
