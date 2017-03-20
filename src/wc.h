#ifndef INCLUDE_WC_H
#define INCLUDE_WC_H

#include "wc_core.h"
#include "wc_stl.h"

#ifdef WC_NETWORK

#include "wc_network.h"

/************************************************************************/
/* You can make custom byte reorders here!                              */
/*                                                                      */
/* If your architecture has a custom data representation                */
/* for which you would like to write a custom network reordering        */
/* then simply override the StitcherProxy<Class, true> template.        */
/* Like with the bool!                                                  */
/************************************************************************/

namespace wc{

    template<class Class>
    class StitcherProxy<Class, true>
    {
    public:
        static void Do(Class* x)
        {
            ByteReorder<sizeof(Class)>::Do((void*)x);
        }
        static void UnDo(Class* x)
        {
            ByteReorder<sizeof(Class)>::UnDo((void*)x);
        }
    };

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
#endif

#endif //INCLUDE_WC_H
