#ifndef INCLUDE_RESPONSIBLE_H
#define INCLUDE_RESPONSIBLE_H

#include "wc.h"

#include <vector>

class responsible_member
{
public:
    responsible_member();
    ~responsible_member();
private:
    std::vector<int>* v;
};

#endif // INCLUDE_RESPONSIBLE_H
