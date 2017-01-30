#include <sys/types.h>
#include <stddef.h>

#include <vector>
#include <iostream>

#ifdef _MSC_VER
#   define VECTOR_ONE _Myfirst
#   define VECTOR_TWO _Mylast
#   define VECTOR_THR _Myend
#elif defined __GNUC__
#   define VECTOR_ONE _M_impl._M_start
#   define VECTOR_TWO _M_impl._M_finish
#   define VECTOR_THR _M_impl._M_end_of_storage
#elif defined __clang__
#   define VECTOR_ONE __begin_
#   define VECTOR_TWO __end_
#   define VECTOR_THR __end_cap_
#else
    // define yourself !
#endif // PLATFORM

class MyVector : public std::vector<int>
{
public:
    static void PrintPointers()
    {
        std::cout <<
            offsetof(MyVector, VECTOR_ONE) << " " <<
            offsetof(MyVector, VECTOR_TWO) << " " <<
            offsetof(MyVector, VECTOR_THR) <<
            std::endl;
    }
};

int main(int, char*[])
{
    std::cout << "vector "; MyVector::PrintPointers();
    return 0;
}