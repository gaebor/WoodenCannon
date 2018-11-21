#include "c_style_class.h"

bool Y::operator==(const Y& other) const
{
    return a == other.a && b == other.b && x == other.x && flag == other.flag;
}

bool POD::operator==(const POD& other)const
{
    return a == other.a && b == other.b;
}

bool X::operator==(const X& other)const
{
    return static_cast<POD2>(*this) == other && x == other.x;
}

bool POD2::operator==(const POD2& other)const
{
    return a == other.a && b == other.b && c == other.c;
}
