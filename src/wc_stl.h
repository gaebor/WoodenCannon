#ifndef INCLUDE_WC_STL_H
#define INCLUDE_WC_STL_H

#include <stddef.h>

#include "wc_core.h"

#include <vector>
#include <list>

namespace wc{

#pragma region vector

template <class Ty, class All>
struct MyMembers<std::vector<Ty, All>>
{
	typedef std::vector<Ty, All> container;
#ifdef _MSC_VER	
	typedef Members < container,
		Pointer<container, offsetof(container, _Myfirst)>,
		Pointer<container, offsetof(container, _Mylast) >,
		Pointer<container, offsetof(container, _Myend) >> List;
#elif defined __GNUC__
	// fuck everyone!
	typedef Members < container,
		Pointer<container, 0U>,
		Pointer<container, 8U>,
		Pointer<container, 16U>> List;
#endif // PLATFORM
};

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
};

#pragma endregion vector

#pragma region list

template <class Ty, class All>
struct MyMembers<std::list<Ty, All>>
{
	typedef std::list<Ty, All> container;
	typedef Members < container > List; // not implemented yet
};

template <class Ty, class All>
struct Callback<std::list<Ty, All>>
{
	typedef std::list<Ty, All> container;
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
};

#pragma endregion //list

}

#endif //INCLUDE_WC_STL_H