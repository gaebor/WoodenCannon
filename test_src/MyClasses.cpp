#include "MyClasses.h"

ClassWithPtr::ClassWithPtr(int i) :i(i), b(new bool(true))
{

}

ClassWithPtr::~ClassWithPtr()
{
    delete b;
}

bool ClassWithPtr::operator==(const ClassWithPtr& other)
{
    return i == other.i;
}

ClassWithPtr::ClassWithPtr(const ClassWithPtr& other)
    : i(other.i), b(new bool(*other.b))
{
}
