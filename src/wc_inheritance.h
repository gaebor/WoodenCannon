#ifndef INCLUDE_WC_PARENTS_H
#define INCLUDE_WC_PARENTS_H

namespace wc {

	template<class C> class Stitcher;

    //! this is for iterating over the parents (compile time)
	template <class C, typename ...Arguments>
	struct Parents
	{
		static void Do(C* x);
		static void UnDo(C* x);
	};

	template <class C, class P, typename ...Arguments>
	struct Parents<C, P, Arguments...>
	{
		static void Do(C* x)
		{
			Stitcher<P>::Do(static_cast<P*>(x));
			Parents<C, Arguments...>::Do(x);
		}
		static void UnDo(C* x)
		{
			Stitcher<P>::UnDo(static_cast<P*>(x));
			Parents<C, Arguments...>::UnDo(x);
		}
	};

	//! packs parents together
	template <class C>
	struct Parents<C>
	{
		//! if there aren't any parents then does nothing
		static void Do(C* x){}
		static void UnDo(C* x){}
	};

	template<class C>
	struct MyParents
	{
		//! by default, the class C has an empty member list
		typedef Parents<C> List;
	};
}

#endif // INCLUDE_WC_PARENTS_H
