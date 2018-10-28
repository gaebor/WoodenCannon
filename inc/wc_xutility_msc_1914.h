namespace wc
{
    template<>
    struct MembersOf<std::_Container_base12> :
        Members<std::_Container_base12,
        Responsible<std::_Container_base12, offsetof(std::_Container_base12, _Myproxy), std::_Container_proxy>
        >
    {};

    template <>
    struct MembersOf<std::_Container_proxy>
        : Members< std::_Container_proxy,
        Pointer<std::_Container_proxy, offsetof(std::_Container_proxy, _Mycont), const std::_Container_base12>,
        Pointer<std::_Container_proxy, offsetof(std::_Container_proxy, _Myfirstiter), std::_Iterator_base12>
        >
    {};

    //! !?! there is still some problem with the _Container_proxy, even in Release!
    template <>
    struct MembersOf<std::_Iterator_base12>
        : Members< std::_Iterator_base12,
        Pointer<std::_Iterator_base12, offsetof(std::_Iterator_base12, _Myproxy), std::_Container_proxy>,
        Pointer<std::_Iterator_base12, offsetof(std::_Iterator_base12, _Mynextiter), std::_Iterator_base12>
        >
    {};
}
