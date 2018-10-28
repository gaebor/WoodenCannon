#ifndef WC_XUTILITY_MSC_H
#define WC_XUTILITY_MSC_H

#include <xutility>

#include "wc_core.h"

#define __wc_header(x) #x
#define _wc_header(PREFIX, VERSION) __wc_header(PREFIX##VERSION.h)
#define wc_header(PREFIX, VERSION) _wc_header(PREFIX, VERSION)

#include wc_header(wc_xutility_msc_, _MSC_VER)

#endif
