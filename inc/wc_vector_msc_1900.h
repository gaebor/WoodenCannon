namespace wc {

	template <class container>
	struct VectorHelper
	{
#ifdef _DEBUG
		static const size_t debug_offset = sizeof(void*);
#else
		static const size_t debug_offset = 0;
#endif
		typedef Pointer<container, debug_offset + 0 * sizeof(void*)> begin;
		typedef Pointer<container, debug_offset + 1 * sizeof(void*)> end;
		typedef Pointer<container, debug_offset + 2 * sizeof(void*)> endend;

		typedef Members<container, begin, end, endend> Hacker;
	};

	template <class Ty, class All>
	struct MembersOf<std::vector<Ty, All>> :
		Members<std::vector<Ty, All>,
#ifdef _DEBUG
		Responsible<std::vector<Ty, All>, 0, std::_Container_proxy>,
#endif // _DEBUG
		typename VectorHelper<std::vector<Ty, All>>::begin,
		typename VectorHelper<std::vector<Ty, All>>::end,
		typename VectorHelper<std::vector<Ty, All>>::endend
		>
	{};

	template <>
	struct MembersOf<std::_Container_proxy>
		: Members< std::_Container_proxy,
		Pointer<std::_Container_proxy, offsetof(std::_Container_proxy, _Mycont)>
		>
	{};

}