#include "test_virtual.h"

bool VirtualParent::operator==(const VirtualParent& other) const
{
    return a == other.a && b == other.b;
}


int VirtualChild1::Op()
{
    return a + b;
}

int VirtualChild2::f()
{
    return i ? -(a + b) : a + b;
}

int VirtualChild2::Op()
{
    return i ? -(a * b) : a * b;
}

bool VirtualParent2::operator==(const VirtualParent2& other) const
{
    return i == other.i;
}

bool VirtualChild2::operator==(const VirtualChild2& other) const
{
    return
        static_cast<const VirtualParent&>(*this) == other && 
        static_cast<const VirtualParent2&>(*this) == other &&
        x == other.x;
}

int VirtualChild11::Op()
{
    return 0;
}
