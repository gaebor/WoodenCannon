#include "MyClasses.h"

#include<string>

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
	std::string str;
	str = "UNUSED";
}

ClassWithUnusedData::~ClassWithUnusedData()
{
}

bool ClassWithUnusedData::operator == (const ClassWithUnusedData& other)const
{
	return true;
}
