#ifndef INCLUDE_WC_STITCHER_H
#define INCLUDE_WC_STITCHER_H

#include <typeinfo>

#include "wc_inheritance.h"
#include "wc_member.h"

#include "wc_network.h"

namespace wc {

	//! custom callback
	template<class Class>
	struct Callback
	{
		static void Do(Class* x) {}
		static void UnDo(Class* x) {}
        template<typename F>
		static void Custom(F& f, const  Class* x) {}
	};

	//!
	/*!
	@tparam Class the type of the stitched object
	@tparam fundamental whether the class is fundamental
	if the class in fundamental, then the stitching should be only network byte reordering
	*/
	template<class Class, bool fundamental>
	struct StitcherProxy
	{
		static void Do(Class* x)
		{
			Callback<Class>::Do(x);
			ParentsOf<Class>::Do(x);
			MembersOf<Class>::Do(x);
		}
		static void UnDo(Class* x)
		{
			MembersOf<Class>::UnDo(x);
			ParentsOf<Class>::UnDo(x);
			Callback<Class>::UnDo(x);
		}
	};

	template<class Class>
	struct Stitcher
	{
		template<typename F>
		static void Custom(F& f, const Class* x)
		{
            f((void*)x, typeid(Class).name(), sizeof(Class));
            F f2 = f;
            f2++;
			ParentsOf<Class>::Custom(f2, x);
            F f3 = f;
            ++f3;
			MembersOf<Class>::Custom(f3, x);

            Callback<Class>::Custom(f, x);
		}
		static void Do(Class* x)
		{
			StitcherProxy<Class, std::is_fundamental<Class>::value>::Do(x);
		}
		static void UnDo(Class* x)
		{
			StitcherProxy<Class, std::is_fundamental<Class>::value>::UnDo(x);
		}
	};

	/* template specifications */

	template<class Class>
	struct StitcherProxy<Class, true>
	{
		static void Do(Class* x)
		{
			ByteReorder<sizeof(Class)>::Do((void*)x);
		}
		static void UnDo(Class* x)
		{
			ByteReorder<sizeof(Class)>::UnDo((void*)x);
		}
	};

	////!
	///*!
	//	pointers are not fundamental
	//*/
	//template<class Class>
	//struct Stitcher<Class*> : StitcherProxy<Class*, true>
	//{
	//	template<typename F>
	//	static void Custom(F f, Class** x)
	//	{
	//		f(x);
	//	}
	//};

}

#endif
