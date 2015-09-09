#include "echo.h"

unsigned int ne_get_version(void)
{
    return NATIVELIB_ECHO_VERSION;
}

int ne_is_compatible_dll(void)
{
    unsigned int major = ne_get_version() >> 16;
    return major == NATIVELIB_ECHO_VERSION_MAJOR;
}
