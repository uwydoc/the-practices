// try_catch_as_function_body.cpp
//
// Use try...catch block as function body, especially in constructors, to catch
// exception thrown by the initialization list
//
#include <iostream>

int do_throw()
{
    throw 0;
    return 1;
}

struct foo
{
    int i;
    foo() try: i(do_throw()) {
        std::cout << "foo constructed" << std::endl;
    } catch (int e) {
        std::cout << "except " << e << std::endl;
    }
};

int main(int argc, char* argv[])
{
    try {
        foo f;
    } catch (int e) {
        std::cout << "except thrown out of constructor" << std::endl;
    }
}
