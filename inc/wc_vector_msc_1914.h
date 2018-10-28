
namespace wc {

    template <class _Alloc>
    struct VectorHelper
    {
        typedef std::_Vector_alloc<_Alloc> _Mybase;
        typedef std::_Vector_val<typename _Mybase::_Val_types> VectorVal;
    };

    template<class _Val_types>
    struct ParentsOf<std::_Vector_val<_Val_types>> :
        Parents<std::_Vector_val<_Val_types>, std::_Container_base>
    {};

    template<class _Val_types>
    struct MembersOf<std::_Vector_val<_Val_types>> :
        Members<std::_Vector_val<_Val_types>,
        Pointer<std::_Vector_val<_Val_types>, offsetof(std::_Vector_val<_Val_types>, _Myfirst), typename _Val_types::value_type>,
        Pointer<std::_Vector_val<_Val_types>, offsetof(std::_Vector_val<_Val_types>, _Mylast), typename _Val_types::value_type>,
        Pointer<std::_Vector_val<_Val_types>, offsetof(std::_Vector_val<_Val_types>, _Myend), typename _Val_types::value_type>
        >
    {};

    template <class _Ty, class _Alloc>
    struct ParentsOf<std::vector<_Ty, _Alloc>> :
        Parents<std::vector<_Ty, _Alloc>, std::_Vector_alloc<std::_Vec_base_types<_Ty, _Alloc>>>
    {};

    template <class _Alloc>
    struct MembersOf<std::_Vector_alloc<_Alloc>> :
        Members<std::_Vector_alloc<_Alloc>,
        Member<std::_Vector_alloc<_Alloc>, 0, typename VectorHelper<_Alloc>::VectorVal>
        >
    {
        static_assert(sizeof(std::_Vector_alloc<_Alloc>) == sizeof(typename VectorHelper<_Alloc>::VectorVal),
            "sizeof(_Compressed_pair<_Ty1, _Ty2>) != sizeof(_Ty2) for _Ty2 = std::_Vector_val");
    };

}
