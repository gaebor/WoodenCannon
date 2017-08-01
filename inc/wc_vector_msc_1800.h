namespace wc {

	template <class Ty, class All>
	struct VectorHelper
	{
		typedef std::vector<Ty, All> container;
		typedef std::_Vector_base<Ty, All> Base;

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
		typename VectorHelper<Ty, All>::begin,
		typename VectorHelper<Ty, All>::end,
		typename VectorHelper<Ty, All>::endend
		>
	{};

	template <>
	struct MembersOf<std::_Container_proxy>
		: Members< std::_Container_proxy,
			Pointer<std::_Container_proxy, offsetof(std::_Container_proxy, _Mycont)>
		>
	{};

}