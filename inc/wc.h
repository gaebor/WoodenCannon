#ifndef INCLUDE_WC_H
#define INCLUDE_WC_H

#include "wc_core.h"

//namespace wc{
//    template<class C>
//    class ResponsiblePtr
//    {
//    protected:
//        C* ptr_;
//        friend MembersOf<ResponsiblePtr<C>>;
//    public:
//        ResponsiblePtr(C* ptr = nullptr) : ptr_(ptr){}
//        ResponsiblePtr(const ResponsiblePtr& other) : ptr_(new C(*other.ptr_)){}
//        bool operator==(const ResponsiblePtr& other)const{return *ptr_ == *other.ptr_;}
//        C* operator->(){return ptr_;}
//        const C* operator->() const { return ptr_; }
//    };
//
//    template<class C>
//    class MembersOf<ResponsiblePtr<C>> : public 
//        Responsible<ResponsiblePtr<C>, 0, C>
//    {
//    };
//
//}



/************************************************************************/
/* You can make custom byte reorders for fundamental types!             */
/*                                                                      */
/* If your architecture has a custom data representation                */
/* for which you would like to write a custom network reordering        */
/* then simply override the StitcherProxy<Class, true> template.        */
/* Like with the bool!                                                  */
/************************************************************************/

#ifdef WC_NETWORK

namespace wc{

    template<>
    class StitcherProxy<bool, true>
    {
    public:
        static void Do(bool* x)
        {
            reorderbool(x);
        }
        static void UnDo(bool* x)
        {
            deorderbool(x);
        }
    };
}

#endif // WC_NETWORK

#endif //INCLUDE_WC_H
