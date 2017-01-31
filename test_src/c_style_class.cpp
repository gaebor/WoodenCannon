#include "c_style_class.h"

bool Z::operator==(const Z& other) const
{
    return a == other.a && b == other.b && x == other.x && flag == other.flag;
}
