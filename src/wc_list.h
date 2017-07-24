#ifndef  INCLUDE_WC_LIST_H
#define INCLUDE_WC_LIST_H

#include <list>

#include "wc_core.h"

#define __wc_header(x) #x
#define _wc_header(PREFIX, VERSION) __wc_header(PREFIX##VERSION.h)
#define wc_header(PREFIX, VERSION) _wc_header(PREFIX, VERSION)

#ifdef _MSC_VER
#include wc_header(wc_list_msc_, _MSC_VER)
#elif defined __GNUC__
#   include "wc_list_gnu.h"
#endif

#endif

