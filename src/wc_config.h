
#ifndef WC_INITIAL_BUFFER_SIZE
#   define WC_INITIAL_BUFFER_SIZE 16
#endif // !WC_INITIAL_BUFFER_SIZE

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

#define STRINGIZE(x) #x

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
#endif //

#define WC_CPP_VERSION(cpp) STRINGIZE(cpp)

#define WC_COMPILER_STR WC_BIT_MODEL "_" WC_COMPILER_TYPE "_" WC_COMPILER_VERSION "_cpp" WC_CPP_VERSION(__cplusplus)
