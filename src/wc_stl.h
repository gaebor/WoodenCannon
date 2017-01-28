#ifndef INCLUDE_WC_STL_H
#define INCLUDE_WC_STL_H

#include "wc_core.h"

#include <vector>

namespace wc{

#ifdef _MSC_VER
	template <class Ty, class All>
	struct MyMembers<std::vector<Ty, All>>
	{
		typedef std::vector<Ty, All> myvector;
		typedef Members < myvector,
			Pointer<myvector, offsetof(myvector, _Myfirst)>,
			Pointer<myvector, offsetof(myvector, _Mylast) >,
			Pointer < myvector, offsetof(myvector, _Myend) >> List;
	};
#elif defined __GNUC__
	template <class Ty, class All>
	struct MyMembers<std::vector<Ty, All>>
	{
		typedef std::vector<Ty, All> myvector;
		// fuck everyone!
		typedef Members < myvector,
			Pointer<myvector, 0U>,
			Pointer<myvector, 8U>,
			Pointer < myvector, 16U>> List;
	};
#endif // _MSC_VER

template <class Ty, class All>
struct Callback<std::vector<Ty, All>>
{
	typedef std::vector<Ty, All> myvector;
	static void Do(myvector* v)
	{
		for (auto& m : *v)
			Stitcher<typename myvector::value_type>::Do(&m);
	}
	static void UnDo(myvector* v)
	{
		for (auto& m : *v)
			Stitcher<typename myvector::value_type>::UnDo(&m);
	}
};

//// see https://en.wikibooks.org/wiki/C_Programming/Preprocessor#X-Macros
//#define SOME_ENUM(X)     \
//    X(MyClass, _Myfirst) \
//	X(MyClass, _Mylast)  \
//    X(MyClass, _Myend)
//
//	WC_DEFINE_MAKERELATIVE(MyClass, SOME_ENUM)
//		WC_DEFINE_MAKEABSOLUTE(MyClass, SOME_ENUM)
//
//#undef SOME_ENUM
//
//	static const size_t offsets[]; //{ offsetof(MyClass, _Myfirst), offsetof(MyClass, _Mylast), offsetof(MyClass, _Myend) };
//	static const size_t pointersSize = 3;
//	/*static const size_t* GetPointers()
//	{
//		
//		std::extent<decltype(offsets)>::value;
//		return offsets;
//	}*/
//	
//};

}

#endif //INCLUDE_WC_STL_H