#include "wc_config.h"
#include "wc_buffer.h"

#include <algorithm>

namespace wc{

    Buffer::Buffer() : blockSize_(WC_INITIAL_BUFFER_SIZE), size_(0), ordered_()
    {
        StartFresh();
    }

    Buffer::~Buffer()
    {
        DestroyAll();
    }

    BufferType* Buffer::GetNew(size_t s)
    {
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
		if (buffers_.empty())
		{
			return GetNew(s)->data();
		}
        auto last = buffers_.back();
        if (last->size() + s <= last->capacity())
        { // fits in the last buffer
            auto end = last->data() + last->size();
            last->resize(last->size() + s); // no reallocation
            size_ += s;
            return end;
		}else // a new one is needed
		{
			return GetNew(s)->data();
		}
    }

    void Buffer::StartFresh()
    {
        if (buffers_.size() == 1)
        {
            GetFirst()->clear();
            size_ = 0;
        }
        else
        {
            Clear();
            Allocate(0);
        }
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
    }

    size_t Buffer::GetSize() const
    {
        return size_;
    }

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
        const auto begin = candidate->data();
        const auto end = begin + candidate->capacity();

        if (end < ordered_.begin()->first)
            return false; // before the first
        
        auto last = ordered_.rbegin()->second;
        if (begin > last->data() + last->capacity())
            return false; // after the last
        auto before_it = ordered_.upper_bound(candidate->data());
        const auto after = before_it->first;
        --before_it;
        const auto before = before_it->second;

        //              |candidate|
        // [ before   ) [begin end) [after ...)

        return !((before->data() + before->capacity()) < begin && end < after);
    }

    BufferType* Buffer::GetFirst()
    {
            return buffers_[0];
    }

    size_t Buffer::GetBlockSize() const
    {
        return blockSize_;
    }

    std::ptrdiff_t Buffer::GetOffset(void* place, void* target) const
    {
        return offsets_.at(GetBin(place)).at(GetBin(target));
    }

    std::ptrdiff_t Buffer::GetOffset(void** ptr) const
    {
        return GetOffset(ptr, *ptr);
    }

    Buffer::keyptr Buffer::GetBin(void* p) const
    {
        if (ordered_.empty())
            return nullptr;
        auto const first = ordered_.begin()->first;
        if (p < first)
            return first;
        return (--ordered_.upper_bound((OrderedType::key_type)p))->first;
    }

    void Buffer::CalculateOffsets()
    {
        offsets_.clear();
        for (auto source = ordered_.begin(); source != ordered_.end(); ++source)
        {
            auto target = ordered_.begin();
            for (; target != source; ++target)
            {
                std::ptrdiff_t result = 0;
                auto next = target;
                auto prev = target;
                for (++next; prev != source; prev = next, ++next)
                    result += next->second->data() - (prev->second->data() + prev->second->size());
                offsets_[source->first][target->first] = result;
            }
            offsets_[source->first][target->first] = 0;
            for (++target; target != ordered_.end(); ++target)
            {
                std::ptrdiff_t result = 0;
                auto next = source;
                auto prev = source;
                for (++next; prev != target; prev = next, ++next)
                    result -= next->second->data() - (prev->second->data() + prev->second->size());
                offsets_[source->first][target->first] = result;
            }
        }
    }

}
