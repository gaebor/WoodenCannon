
#include "wc.h"

class ClassWithStrongPtr
{
public:
    int i;
    bool* b;
public:
    ClassWithStrongPtr(int i);
    ClassWithStrongPtr(const ClassWithStrongPtr& other);
    ~ClassWithStrongPtr();
    bool operator == (const ClassWithStrongPtr& other)const;
};

class ClassWithWeakPtr
{
public:
	int i;
	void* p;
public:
	ClassWithWeakPtr(int i);
    ClassWithWeakPtr(const ClassWithWeakPtr& other);
	~ClassWithWeakPtr();
	bool operator == (const ClassWithWeakPtr& other)const;
};

namespace wc {
    template<>
    struct MembersOf<ClassWithStrongPtr> : Members<ClassWithStrongPtr,
        Member2<decltype(&ClassWithStrongPtr::i), &ClassWithStrongPtr::i>,
        Responsible2<decltype(&ClassWithStrongPtr::b), &ClassWithStrongPtr::b>>
    {};

	template<>
	struct MembersOf<ClassWithWeakPtr> : Members<ClassWithWeakPtr,
        Member2<decltype(&ClassWithWeakPtr::i), &ClassWithWeakPtr::i>,
        Pointer2<decltype(&ClassWithWeakPtr::p), &ClassWithWeakPtr::p>>
	{};
}

class ClassWithUnusedData
{
public:
public:
	ClassWithUnusedData();
	ClassWithUnusedData(const ClassWithUnusedData& other);
	~ClassWithUnusedData();
	bool operator == (const ClassWithUnusedData& other)const;
};
