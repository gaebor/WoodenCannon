namespace wc {

	template <class container>
	struct VectorHelper
	{
		typedef Pointer<container, offsetof(container, _Myfirst)> begin;
		typedef Pointer<container, offsetof(container, _Mylast)> end;
		typedef Pointer<container, offsetof(container, _Myend)> endend;

		typedef Members<container, begin, end, endend> Hacker;
	};

	template <class Ty, class All>
	struct MembersOf<std::vector<Ty, All>> :
		Members<std::vector<Ty, All>,
#ifdef _DEBUG
		Responsible<std::vector<Ty, All>, offsetof(container, _Myproxy), std::_Container_proxy>,
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