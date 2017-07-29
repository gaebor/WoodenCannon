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

ClassWithWeakPtr::~ClassWithWeakPtr(void){}

bool ClassWithWeakPtr::operator==(const ClassWithWeakPtr& other)const
{
	return i == other.i;
}

//{
//public:
//	int i;
//	void* p;
//public:
//	ClassWithWeakPtr(int i);
//	ClassWithWeakPtr(const ClassWithWeakPtr& other);
//	~ClassWithWeakPtr();
//	bool operator == (const ClassWithWeakPtr& other);
//};
