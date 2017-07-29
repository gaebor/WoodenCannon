#ifndef INCLUDE_WC_CONFIG_H
#define INCLUDE_WC_CONFIG_H

#ifndef WC_INITIAL_BUFFER_SIZE
#   define WC_INITIAL_BUFFER_SIZE 16
#endif // !WC_INITIAL_BUFFER_SIZE

#ifdef WC_NETWORK
#   define WC_BYTE_ORDER "network"
#elif defined _WIN32
#   define WC_BYTE_ORDER "little"
#elif defined __BYTE_ORDER__
#   if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
#       define WC_BYTE_ORDER "little"
#   elif __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
#       define WC_BYTE_ORDER "big"
#   elif __BYTE_ORDER__ == __ORDER_PDP_ENDIAN__
#       define WC_BYTE_ORDER "PDP"
#   endif
#else
#   define WC_BYTE_ORDER "unknown"
#endif

#ifdef _WIN64
#define WC_BIT_MODEL "LLP64"
#endif //  _WIN64

#ifdef __LP64__
#define WC_BIT_MODEL "LP64"
#endif //  _WIN64

// unknown or 32bit assumed
#ifndef WC_BIT_MODEL
#   define WC_BIT_MODEL "ILP32"
#endif // !WC_BIT_MODEL

#ifdef _WIN64
#define WC_BIT_WIDTH 64
#elif defined _WIN32
#define WC_BIT_WIDTH 32
#endif

#if __SIZEOF_POINTER__ == 8 && __CHAR_BIT__ == 8
#define WC_BIT_WIDTH 64
#elif __SIZEOF_POINTER__ == 4 && __CHAR_BIT__ == 8
#define WC_BIT_WIDTH 32
#endif

#define _WC_STRINGIZE(x) #x
#define WC_STRINGIZE(x) _WC_STRINGIZE(x)

#ifdef _MSC_VER
#ifdef _DEBUG
#   define WC_COMPILER_TYPE "MSVC_DEBUG"
#else
#   define WC_COMPILER_TYPE "MSVC"
#endif //  _DEBUG

#   define WC_COMPILER_VERSION_F(x) STRINGIZE(x)
#   define WC_COMPILER_VERSION WC_COMPILER_VERSION_F(_MSC_FULL_VER)
#elif defined __GNUC__
#   define WC_COMPILER_TYPE "GNUC"
#   define WC_COMPILER_VERSION_F(x,y,z) STRINGIZE(x) "." STRINGIZE(y) "." STRINGIZE(z)
#   define WC_COMPILER_VERSION WC_COMPILER_VERSION_F(__GNUC__,__GNUC_MINOR__,__GNUC_PATCHLEVEL__)
#else
#   define WC_COMPILER_STR ""
#   define WC_COMPILER_VERSION ""
#endif //

#define WC_COMPILER_STR WC_BIT_MODEL "_" WC_BYTE_ORDER "-endian_" WC_COMPILER_TYPE "_" WC_COMPILER_VERSION "_cpp" WC_STRINGIZE(__cplusplus)

//! this macro tells you the compile time properties of WoodenCannon
#define WC_COMPILED_CONFIG WC_STRINGIZE(WC_BIT_WIDTH) "bit_" WC_BYTE_ORDER "-endian"

#endif