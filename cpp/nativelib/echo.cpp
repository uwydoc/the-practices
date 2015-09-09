#include "echo.h"
#include "echo_p.h"

#define AS_TYPE(T, p) reinterpret_cast<T*>(p)
#define AS_CTYPE(T, p) reinterpret_cast<const T*>(p)

ne_echo_t* ne_echo_new(const char* msg)
{
    return AS_TYPE(ne_echo_t, new nativelib::Echo(msg));
}

void ne_echo_free(ne_echo_t* e)
{
    if (!e)
        return;
    delete AS_TYPE(nativelib::Echo, e);
}

const char* ne_echo_get_msg(const ne_echo_t* e)
{
    return AS_CTYPE(nativelib::Echo, e)->get_msg();
}

void ne_echo_set_msg(ne_echo_t* e, const char* msg)
{
    AS_TYPE(nativelib::Echo, e)->set_msg(msg);
}

void ne_echo_echo(const ne_echo_t* e)
{
    AS_CTYPE(nativelib::Echo, e)->echo();
}
