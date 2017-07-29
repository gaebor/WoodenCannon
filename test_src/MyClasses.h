
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
	~ClassWithWeakPtr();
	bool operator == (const ClassWithWeakPtr& other)const;
};

namespace wc {
    template<>
    struct MembersOf<ClassWithStrongPtr> : Members<ClassWithStrongPtr,
        Member<ClassWithStrongPtr, offsetof(ClassWithStrongPtr, i), int>,
        Responsible<ClassWithStrongPtr, offsetof(ClassWithStrongPtr, b), bool>>
    {};

	template<>
	struct MembersOf<ClassWithWeakPtr> : Members<ClassWithWeakPtr,
		Member<ClassWithWeakPtr, offsetof(ClassWithWeakPtr, i), int>,
		Member<ClassWithWeakPtr, offsetof(ClassWithWeakPtr, p), void*>>
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
