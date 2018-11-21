#include "MyClasses.h"

static std::vector<int> v;

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
    v.push_back(v.size());
}

ClassWithUnusedData::ClassWithUnusedData(const ClassWithUnusedData&)
{
    v.push_back(v.size());
}

ClassWithUnusedData::~ClassWithUnusedData()
{
    v.push_back(v.size());
}

bool ClassWithUnusedData::operator== (const ClassWithUnusedData&)const
{
    v.push_back(v.size());
	return true;
}
