#ifndef INCLUDE_COMPLEX_TYPE_H
#define INCLUDE_COMPLEX_TYPE_H

#include "wc.h"

#include <vector>

class ComplexParent
{
public:
    std::vector<int> v;
    bool operator==(const ComplexParent& other)const;
};

class ComplexParent2
{
public:
    std::vector<int> w;
    bool operator==(const ComplexParent2& other)const;
};

class ComplexChild : public ComplexParent, public ComplexParent2
{
public:
    std::vector<int> vv;
    bool operator==(const ComplexChild& other)const;
};

// this is what You have to do!
namespace wc{
    //! defines the (one and only) member of ComplexParent
    template<>
    struct MembersOf<ComplexParent> :
        Members<ComplexParent,
            Member<ComplexParent, offsetof(ComplexParent, v), std::vector<int>>
        >
    {};

    template<>
    struct MembersOf<ComplexParent2> : 
        Members<ComplexParent2,
            Member<ComplexParent2, offsetof(ComplexParent2, w), std::vector<int>>
        >
    {};

    template<>
    struct MembersOf<ComplexChild> :
        Members<ComplexChild,
            Member<ComplexChild, offsetof(ComplexChild, vv), std::vector<int>>
        >
    {};

    //! defines the relation between ComplexChild and ComplexParent
    template<>
    struct ParentsOf<ComplexChild> : 
        Parents<ComplexChild, ComplexParent, ComplexParent2>
    {};
}

#endif //INCLUDE_COMPLEX_TYPE_H
