#ifndef INCLUDE_WC_H
#define INCLUDE_WC_H

#include "wc_core.h"
#include "wc_stl.h"

#ifdef WC_NETWORK

namespace wc{

    template<class Class>
    class StitcherProxy<Class, true>
    {
    public:
        static void Do(Class* x)
        {
            ByteReorder<sizeof(Class)>::Do(x);
        }
        static void UnDo(Class* x)
        {
            ByteReorder<sizeof(Class)>::UnDo(x);
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
