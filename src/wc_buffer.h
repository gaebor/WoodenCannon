#ifndef INCLUDE_WC_BUFFER_H
#define INCLUDE_WC_BUFFER_H

#include <map>
#include <unordered_map>

#include "wc_functions.h"

namespace wc{

    class Buffer
    {
    public:
        void Clear();
        Buffer();
        size_t GetSize()const;
        BufferType* ReArrange();
        //! this may throw
        void* Allocate(size_t s);
        ~Buffer();
        void* GetBin(void* p)const;
    private:
        BufferType* GetNew(size_t s = 0);
        void DestroyAll();
        size_t _blockSize;
        size_t _size;
        std::map<void*, BufferType*> _ordered;
        // std::unordered_map<void**, std::unordered_map<void*, intptr_t>> gaps;
    };

}

#endif //INCLUDE_WC_BUFFER_H
