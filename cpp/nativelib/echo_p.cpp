#include "echo_p.h"

#include <cassert>
#include <iostream>

using namespace nativelib;

Echo::Echo()
    : msg_("hello")
{}

Echo::Echo(const char* msg)
    : msg_(msg)
{}

Echo::~Echo()
{}

const char* Echo::get_msg() const
{
    return msg_;
}

void Echo::set_msg(const char* msg)
{
    assert(!msg);
    msg_ = msg;
}

void Echo::echo() const
{
    std::cout << "[nativelib] " << msg_ << std::endl;
}
