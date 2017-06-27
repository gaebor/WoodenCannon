#include "wc_config.h"
#include "wc_buffer.h"

#include <algorithm>

namespace wc{

    Buffer::Buffer() : blockSize_(WC_INITIAL_BUFFER_SIZE), size_(0), ordered_()
    {
        GetNew();
    }

    Buffer::~Buffer()
    {
        DestroyAll();
    }

    BufferType* Buffer::GetNew(size_t s)
    {
        // OrderedType::const_iterator adjacent;
        while (blockSize_ < s)
            blockSize_ *= 2;
        BufferType* new_buffer = nullptr;
        do {
            // this may be an infinite loop !!!
            delete new_buffer;
            new_buffer = new BufferType();
            new_buffer->reserve(blockSize_);
        } while (IsAdjacentBuffer(new_buffer));
        new_buffer->resize(s);
        InsertNew(new_buffer);
        return new_buffer;
    }

    void* Buffer::Allocate(size_t s)
    {
        auto last = *(--buffers_.end());
        if (last->size() + s <= last->capacity())
        { // fits in the last buffer
            auto end = last->data() + last->size();
            last->resize(last->size() + s); // no reallocation
            size_ += s;
            return end;
        }else // a new one is needed
            return GetNew(s)->data();
    }

    void Buffer::ReArrange()
    {
        if (ordered_.size() > 1)
        {
            auto buffer = new BufferType();
            for (auto& b : buffers_)
                buffer->insert(buffer->end(), b->begin(), b->end());
            blockSize_ = buffer->capacity();
            DestroyAll();
            InsertNew(buffer);
        }
    }

    void Buffer::DestroyAll()
    {
        for (auto& b : ordered_)
            delete b.second;
        ordered_.clear();
        buffers_.clear();
        size_ = 0;
    }

    void Buffer::Clear()
    {
        DestroyAll();
        GetNew();
    }

    size_t Buffer::GetSize() const
    {
        return size_;
    }

    //BufferType* Buffer::GetBin(void* p) const
    //{
    //    return GetIterator(p)->second;
    //}

    //Buffer::OrderedType::const_iterator Buffer::GetIterator(void* p) const
    //{
    //    //return --(ordered_.upper_bound((void**)p));
    //    return ordered_.end();
    //}

    void Buffer::InsertNew(BufferType* new_buffer)
    {
        buffers_.push_back(new_buffer);
        ordered_[new_buffer->data()] = new_buffer;
        size_ += new_buffer->size();
    }

    bool Buffer::IsAdjacentBuffer(BufferType* candidate) const
    {
        if (ordered_.size() == 0)
            return false; // no blocks so far
        if (candidate->data() + candidate->size() < ordered_.begin()->first)
            return false; // before the first
        auto last = (--ordered_.end())->second;
        if (candidate->data() > last->data() + last->size())
            return false; // after the last
        auto before = (--ordered_.upper_bound(candidate->data()))->second;
        auto after = before;
        ++after;
        return
            before->data() + before->size() < candidate->data() && 
            candidate->data() < after->data();
    }

    const BufferType* Buffer::GetFirst() const
    {
            return buffers_[0];
    }

    size_t Buffer::GetBlockSize() const
    {
        return blockSize_;
    }

}
