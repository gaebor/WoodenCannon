#ifndef INCLUDE_WC_VECTOR_H
#define INCLUDE_WC_VECTOR_H

#include <vector>

#include "wc_core.h"

#define __wc_header(x) #x
#define _wc_header(PREFIX, VERSION) __wc_header(PREFIX##VERSION.h)
#define wc_header(PREFIX, VERSION) _wc_header(PREFIX, VERSION)

#ifdef _MSC_VER
#include "wc_xutility_msc.h"

#include wc_header(wc_vector_msc_, _MSC_VER)
#elif defined __GNUC__
#   include "wc_vector_gnu.h"
#endif

namespace wc {

	template <class Ty, class All>
	struct Callback<std::vector<Ty, All>>
	{
		typedef std::vector<Ty, All> container;
		static void Do(container* v)
		{
			for (auto& m : *v)
				Stitcher<typename container::value_type>::Do(&m);
		}
		static void UnDo(container* v)
		{
			for (auto& m : *v)
				Stitcher<typename container::value_type>::UnDo(&m);
		}
        template<typename F>
        static void Custom(F& f, const container* v)
        {
            for (const auto& x : *v)
            {
                wc::Stitcher<typename container::value_type>::Custom(f, &x);
            }
        }
	};

}

#endif
