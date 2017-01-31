#include "complex_type.h"

bool ComplexParent::operator==(const ComplexParent& other) const
{
    return v == other.v;
}

bool ComplexParent2::operator==(const ComplexParent2& other) const
{
    return w == other.w;
}

bool ComplexChild::operator==(const ComplexChild& other) const
{
    return vv == other.vv && v == other.v && w == other.w;
}
