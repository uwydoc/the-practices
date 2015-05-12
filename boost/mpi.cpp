// mpi.cpp
//
#include <iostream>
#include <string>

#include <boost/mpi.hpp>
#include <boost/serialization/string.hpp>

namespace test {

class animal
{
public:
    animal() {}
    animal(const std::string& name, int legs)
        : name_(name), legs_(legs) {}

    const std::string& name() const { return name_; }
    int legs() const { return legs_; }
private:
    std::string name_;
    int legs_;
};

template<typename Archive>
void serialize(Archive& ar, animal& a, const unsigned int version)
{
    if (version != 0)
        return;

    if (Archive::is_saving::value) {
        ///@note the following 2 lines of code failed to compile, because
        ///      the interface is defined as follows:
        ///      [code] interface_oarchive::operator<<(T& t) [/code]
        ///      non-const reference is required.
        ///
        ///      i think Boost.Serialization should provide the const reference
        ///      version instead
        //ar << a.name();
        //ar << a.legs();
        std::string name = a.name();
        int legs = a.legs();
        ar & name;
        ar & legs;
    } else if (Archive::is_loading::value) {
        std::string name;
        int legs;
        ar & name;
        ar & legs;
        a = animal(name, legs);
    }
}

std::ostream& operator<<(std::ostream& os, const animal& a)
{
    return (os << "animal(" << a.name() << "," << a.legs() << ")");
}

enum {
    k_tag_msg = 16
    , k_tag_animal
};

}

int main(int argc, char* argv[])
{
    namespace mpi = boost::mpi;

    mpi::environment env(argc, argv);
    mpi::communicator world_comm;
    std::cout << "[info]" << env.processor_name() << ", "
              << world_comm.rank() << ", " << world_comm.size() << '\n';
    if (world_comm.rank() == 0) {
        std::string msg;
        mpi::status status = world_comm.recv(mpi::any_source, test::k_tag_msg, msg);
        std::cout << "source: " << status.source() << ", msg: " << msg << '\n';
        test::animal a;
        status = world_comm.recv(mpi::any_source, test::k_tag_animal, a);
        std::cout << "source: " << status.source() << ", animal: " << a << '\n';
    } else {
        world_comm.send(0, test::k_tag_msg, std::string("hello world"));
        world_comm.send(0, test::k_tag_animal, test::animal("bird", 2));
    }
}
