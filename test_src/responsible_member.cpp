#include "responsible_member.h"

responsible_member::responsible_member() :v(new std::vector<int>())
{

}

responsible_member::~responsible_member()
{
    delete v;
}
