#include "MyClasses.h"

ClassWithStrongPtr::ClassWithStrongPtr(int i) :i(i), b(new bool(true))
{

}

ClassWithStrongPtr::~ClassWithStrongPtr()
{
    delete b;
}

bool ClassWithStrongPtr::operator==(const ClassWithStrongPtr& other)const
{
    return i == other.i;
}

ClassWithStrongPtr::ClassWithStrongPtr(const ClassWithStrongPtr& other)
    : i(other.i), b(new bool(*other.b))
{
}

ClassWithWeakPtr::ClassWithWeakPtr(int i)
: i(i), p(this)
{
}

ClassWithWeakPtr::ClassWithWeakPtr(const ClassWithWeakPtr& other)
    : i(other.i), p(this)
{
}

ClassWithWeakPtr::~ClassWithWeakPtr(void){}

bool ClassWithWeakPtr::operator==(const ClassWithWeakPtr& other)const
{
    return i == other.i;
}

ClassWithUnusedData::ClassWithUnusedData()
{
}

ClassWithUnusedData::ClassWithUnusedData(const ClassWithUnusedData& other)
{
    std::vector<size_t> unused;
    for (size_t n = 2; n < 10; ++n)
    {
        size_t d;
        for (d = 2; d < n; ++d)
            if (n%d == 0)
                break;
        if (d == n)
            unused.push_back(n);
    }
}

ClassWithUnusedData::~ClassWithUnusedData()
{
}

bool ClassWithUnusedData::operator== (const ClassWithUnusedData&)const
{
	return true;
}
