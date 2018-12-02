#ifndef INCLUDE_WC_BUFFER_H
#define INCLUDE_WC_BUFFER_H

#include <map>
#include <unordered_map>

#include "wc_functions.h"

namespace wc{

    //! internal storage of blocks of memory
    /*!
        This is not seen for the user in the library
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
			basically, this overrides operator new
            may throw
        */
        void* Allocate(size_t s);
        void StartFresh();
        ~Buffer();
        BufferType* GetFirst();
        //! melts the blocks together
        void ReArrange();
        //! determines the appropriate offset to move the pointer
        /*!
            takes the fragmented buffers into account
        */
        std::ptrdiff_t GetOffset(void** ptr)const;
        void CalculateOffsets();
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
        typedef OrderedType::key_type keyptr;
        HolderType buffers_;
        // OrderedType::const_iterator GetIterator(void* p)const;
        std::unordered_map<keyptr, std::unordered_map<keyptr, std::ptrdiff_t>> offsets_;

        //! determines which buffer is the pointed target in
        keyptr GetBin(void* p)const;
        std::ptrdiff_t GetOffset(void* place, void* target)const;
    };

}

#endif //INCLUDE_WC_BUFFER_H
