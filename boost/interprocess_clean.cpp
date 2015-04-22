// interprocess_clean.cpp
//
#include <iostream>

#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/sync/named_mutex.hpp>
#include <boost/interprocess/sync/named_condition.hpp>

int main(int argc, char* argv[])
{
    if (argc < 2) {
        std::cout << "Usage: interprocess_clean <t:name> ..." << '\n';
        return 0;
    }

    using namespace boost::interprocess;

    for (int i = 1; i < argc; ++i) {
        std::string item(argv[i]);
        if(item.length() >= 3 && item[1] == ':') {
            char type = item[0];
            std::string name = item.substr(2);
            switch (type) {
                case 's':
                    if (shared_memory_object::remove(name.c_str()))
                        std::cout << "shared memory '" << name << "' removed" << '\n';
                    break;
                case 'm':
                    if (named_mutex::remove(name.c_str()))
                        std::cout << "named mutex '" << name << "' removed" << '\n';
                    break;
                case 'c':
                    if (named_condition::remove(name.c_str()))
                        std::cout << "named condition '" << name << "' removed" << '\n';
                    break;
                default:
                    std::cerr << "[error] unknown type '" << type << "'" << '\n';
                    break;
            }
        }
    }
}
