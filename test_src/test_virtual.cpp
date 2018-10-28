#include "test_virtual.h"

bool MyParentVirtual::operator==(const MyParentVirtual& other) const
{
    return a == other.a && b == other.b;
}

bool Add::operator==(const Add& other) const
{
    return static_cast<const MyParentVirtual*>(this)->operator==((const MyParentVirtual&)other);
}

int Add::Op()
{
    return a + b;
}

int Mul::Op()
{
    return flag ? -(a * b) : a * b;
}

bool Mul::operator==(const Mul& other) const
{
    return a == other.a && b == other.b && flag == other.flag;
}
