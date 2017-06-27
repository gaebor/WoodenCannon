#ifndef INCLUDE_WC_FUNCTIONS_H
#define INCLUDE_WC_FUNCTIONS_H

#include <stddef.h>
#include <vector>
#include <stdio.h>
#include <new>

void* operator new (size_t count);
void* operator new (size_t count, const std::nothrow_t& tag) throw();

namespace wc {
    template<class Int, size_t d>
    struct RoundD
    {
        static Int Do(const Int& i)
        {
            return ((i + d - 1) / d)*d;
        }
    };

    //! translates pointers to relative pointers
    /*!
        this is where an other magic happens!
        Also host to network reordering happens here, if configured.
    */
    void memory2buffer(void** p);
    //! translates relative pointers to absolute pointers
    /*!
        this is where that magic is reversed!
        Also network to host reordering happens here, if configured.
    */
    void buffer2memory(void** p);

    typedef std::vector<unsigned char> BufferType;

    const BufferType* GetBuffer();
    //! Writes the current buffer into a file, as-is
    bool WriteBuffer(FILE*);
    //! Reads the content of a file into the current buffer, as-is
    /*!
        @param s reads only s bytes from file, zero length means the all of it
    */
    bool ReadBuffer(FILE*, size_t s = 0);

    //! pretty prints the content of the buffer to stdout
    void PrintBuffer();
    //! sets maximum length of serialized buffer
    /*!
    If there is an already serialized object in the buffer, that might get compromised!
    */
    void SetMax(size_t bytes);
}

#endif // INCLUDE_WC_FUNCTIONS_H
