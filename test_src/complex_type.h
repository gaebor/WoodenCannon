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
    struct MyMembers<ComplexParent> : public MembersHelper<ComplexParent>
    {
        typedef Members<ComplexParent, M<offsetof(ComplexParent, v), std::vector<int>>::Type> List;
    };

    template<>
    struct MyMembers<ComplexParent2>
    {
        typedef Members<ComplexParent2, Member<ComplexParent2, offsetof(ComplexParent2, w), std::vector<int>>> List;
    };

    template<>
    struct MyMembers<ComplexChild> : public MembersHelper<ComplexChild>
    {
        typedef Members<ComplexChild, M<offsetof(ComplexChild, vv), std::vector<int>>::Type> List;
    };

    //! defines the relation between ComplexChild and ComplexParent
    template<>
    struct MyParents<ComplexChild>
    {
        typedef Parents<ComplexChild, ComplexParent, ComplexParent2> List;
    };
}

#endif //INCLUDE_COMPLEX_TYPE_H
