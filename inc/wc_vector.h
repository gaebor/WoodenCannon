#ifndef INCLUDE_WC_VECTOR_H
#define INCLUDE_WC_VECTOR_H

#include <vector>

#include "wc_core.h"

#define __wc_header(x) #x
#define _wc_header(PREFIX, VERSION) __wc_header(PREFIX##VERSION.h)
#define wc_header(PREFIX, VERSION) _wc_header(PREFIX, VERSION)

#ifdef _MSC_VER
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
			if (v->empty())
			{   //hack!
				auto const end = (void**)(v + 1);
				VectorHelper<Ty, All>::Hacker::Custom([&end](void* x, size_t, const char*)
				{
					*(void**)x = end;
				}, v);
			}
			else
				for (auto& m : *v)
					Stitcher<typename container::value_type>::Do(&m);
		}
		static void UnDo(container* v)
		{   //hack!
			auto const end = (void**)(v + 1);
			bool hacked = true;
			VectorHelper<Ty, All>::Hacker::Custom([&end, &hacked](void* x, size_t, const char*)
			{
				hacked = (hacked && (*(void**)x == end));
			}, v);
			if (hacked)
			{
				VectorHelper<Ty, All>::Hacker::Custom([&end](void* x, size_t, const char*)
				{
					*(void**)x = nullptr;
				}, v);
			}
			else
				for (auto& m : *v)
					Stitcher<typename container::value_type>::UnDo(&m);
		}
	};

}

#endif
