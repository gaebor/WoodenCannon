#include "wc_buffer.h"

#include <algorithm>

namespace wc{

    Buffer::Buffer() : _blockSize(1024), _size(0), _ordered()
    {
        GetNew();
    }

    Buffer::~Buffer()
    {
        DestroyAll();
    }

    BufferType* Buffer::GetNew(size_t s)
    {
        auto buffer = new BufferType();
        while (_blockSize < s) _blockSize *= 2;
        buffer->reserve(_blockSize);
        buffer->resize(s);
        _size += s;
        _ordered[buffer->data()] = buffer;
        //std::unordered_map<void*, intptr_t> row;
        //row[buffer] = 0;
        //for (auto& g : gaps)
        //{
        //    g.second[] = ;
        //    row[] ;
        //}
        //gaps[buffer] = row;
        return buffer;
    }

    void* Buffer::Allocate(size_t s)
    {
        auto last = (--_ordered.end())->second;
        if (last->size() + s <= last->capacity())
        {
            auto end = last->data() + last->size();
            last->resize(last->size() + s); // no reallocation
            _size += s;
            return end;
        }else
        {
            return GetNew(s)->data();
        }
    }

    BufferType* Buffer::ReArrange()
    {
        if (_ordered.size() > 1)
        {
            auto buffer = new BufferType();
            for (auto& b : _ordered)
                buffer->insert(buffer->end(), b.second->begin(), b.second->end());
            _blockSize = buffer->capacity();
            DestroyAll();
            _ordered[buffer->data()] = buffer;
            return buffer;
        }else
        {
            return _ordered.begin()->second;
        }
    }

    void Buffer::DestroyAll()
    {
        for (auto& b : _ordered)
            delete b.second;
        _ordered.clear();
        _size = 0;
        // gaps.clear();
    }

    void Buffer::Clear()
    {
        DestroyAll();
        GetNew();
    }

    size_t Buffer::GetSize() const
    {
        return _size;
    }

    void* Buffer::GetBin(void* p) const
    {
        return (--_ordered.upper_bound(p))->first;
    }

}
