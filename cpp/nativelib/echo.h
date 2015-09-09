// 1. Header Guards
#ifndef NATIVELIB_ECHO_H_INCLUDED
#define NATIVELIB_ECHO_H_INCLUDED
#ifdef __cplusplus
extern "C" {
#endif

// 2. Export Markers
#ifndef NATIVELIB_ECHO_API
# ifdef _WIN32
#  if defined(NATIVELIB_ECHO_BUILD_SHARED) // build dll
#   define NATIVELIB_ECHO_API __declspec(dllexport)
#  elif !defined(NATIVELIB_ECHO_BUILD_STATIC) // use dll
#   define NATIVELIB_ECHO_API __declspec(dllimport)
#  else // static lib
#   define NATIVELIB_ECHO_API
#  endif
# else
#  if __GNUC__ >= 4
#   define NATIVELIB_ECHO_API __attribute__((visibility("default")))
#  else
#   define NATIVELIB_ECHO_API
#  endif
# endif
#endif

// 3. Careful with Includes and Defines
// include files like 'stdint.h', instead of defining your own 'int32_t'
// NEVER include 'windows.h' in the library header, instead, include it in the internal header/source file

// 4. Stable ABI
#define NATIVELIB_ECHO_VERSION_MAJOR 1
#define NATIVELIB_ECHO_VERSION_MINOR 0
#define NATIVELIB_ECHO_VERSION ((NATIVELIB_ECHO_VERSION_MAJOR << 16) | NATIVELIB_ECHO_VERSION_MINOR)

NATIVELIB_ECHO_API unsigned int ne_get_version(void);
NATIVELIB_ECHO_API int ne_is_compatible_dll(void);

// 5. Exporting a C API
struct ne_echo_s;
typedef struct ne_echo_s ne_echo_t;

NATIVELIB_ECHO_API ne_echo_t* ne_echo_new(const char* msg);
NATIVELIB_ECHO_API void ne_echo_free(ne_echo_t* e);
NATIVELIB_ECHO_API const char* ne_echo_get_msg(const ne_echo_t* e);
NATIVELIB_ECHO_API void ne_echo_set_msg(ne_echo_t* e, const char* msg);
NATIVELIB_ECHO_API void ne_echo_echo(const ne_echo_t* e);

// 6. Context Objects (to repalce global variables)

// 7. Memory Allocation Customization
// custom malloc, realloc and free

// 8. Memory Allocators and C++
// class-specific |new|, |delete|, |new[]| and |delete[]| using custom malloc/free
// custom proxy allocator proxy and typedefs for all necessary stl containers (
// complex and annoying, better placed in seperate header)

// 9. Memory Allocation Failures
// just ignore it because there is few you could do

// 10. Building
// CMake or Premake

// 11. Testing
// cpp unit testing framework
// !test in another language such as Python (via cffi)!

#ifdef __cplusplus
}
#endif
#endif // NATIVELIB_ECHO_H_INCLUDED
