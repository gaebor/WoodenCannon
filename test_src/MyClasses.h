
#include "wc.h"

class ClassWithPtr
{
public:
    int i;
    bool* b;
public:
    ClassWithPtr(int i);
    ClassWithPtr(const ClassWithPtr& other);
    ~ClassWithPtr();
    bool operator == (const ClassWithPtr& other);
};

namespace wc {
    template<>
    struct MembersOf<ClassWithPtr> : Members<ClassWithPtr,
        Member<ClassWithPtr, offsetof(ClassWithPtr, i), decltype(ClassWithPtr::i)>,
        Responsible<ClassWithPtr, offsetof(ClassWithPtr, b), bool>>
    {};
}
