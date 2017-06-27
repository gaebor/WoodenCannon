#ifndef INCLUDE_WC_BUFFER_H
#define INCLUDE_WC_BUFFER_H

#include <map>
#include <unordered_map>

#include "wc_functions.h"

namespace wc{

    //! internal storage of blocks of memory
    /*!
        This is not seen for the user if the library
    */
    class Buffer
    {
    public:
        //! starts a new page
        void Clear();
        Buffer();
        //! Get the number of held bytes
        size_t GetSize()const;
        //! Get the block size
        size_t GetBlockSize()const;
        //! allocates given bytes into internal buffer(s)
        /*!
            this may throw
        */
        void* Allocate(size_t s);
        ~Buffer();
        const BufferType* GetBin(void* p = nullptr)const;
        const BufferType* GetFirst()const;
        //! melts the blocks together
        void ReArrange();
    private:
        BufferType* GetNew(size_t s = 0);
        bool IsAdjacentBuffer(BufferType* candidate)const;
        void InsertNew(BufferType* new_buffer);
        void DestroyAll();
        //! the allocation size of a new memory block
        /*!
            Fragmented allocation may occur if it is small,
            But it expands according to usage.
        */
        size_t blockSize_;
        //! number of held bytes
        size_t size_;
        typedef std::vector<BufferType*> HolderType;
        typedef std::map<BufferType::pointer, BufferType*> OrderedType;
        OrderedType ordered_;
        HolderType buffers_;
        OrderedType::const_iterator GetIterator(void* p)const;
    };

}

#endif //INCLUDE_WC_BUFFER_H
