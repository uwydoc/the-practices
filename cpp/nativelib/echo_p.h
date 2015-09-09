#ifndef NATIVELIB_ECHO_P_H_INCLUDED
#define NATIVELIB_ECHO_P_H_INCLUDED

namespace nativelib {

class Echo
{
public:
    Echo();
    Echo(const char* msg);
    ~Echo();

    const char* get_msg() const;
    void set_msg(const char* msg);
    void echo() const;

private:
    const char* msg_;
};

}

#endif // NATIVELIB_ECHO_P_H_INCLUDED
