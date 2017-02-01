#ifndef INCLUDE_WC_STL_H
#define INCLUDE_WC_STL_H

#include "wc_core.h"

#include "wc_vector.h"

#ifdef _MSC_VER
#   include "wc_list_msc.h"
#elif defined __GNUC__
#   include "wc_list_gnuc.h"
#endif // _MSC_VER

#endif //INCLUDE_WC_STL_H