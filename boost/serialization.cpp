// serialization.cpp
//
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include <boost/array.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/serialization/array.hpp>
#include <boost/serialization/export.hpp>
#include <boost/serialization/string.hpp>
#include <boost/serialization/scoped_ptr.hpp>

namespace test {

class animal
{
public:
    animal() {}
    animal(int legs, const std::string& name)
        : legs_(legs), name_(name)
    {}

    int legs() const { return legs_;  }
    const std::string& name() const { return name_; }
private:
    friend class boost::serialization::access;

    template<typename Archive>
    friend void serialize(Archive& ar, animal& a, const unsigned int version);

    int legs_;
    std::string name_;
};

template<typename Archive>
void serialize(Archive& ar, animal& a, const unsigned int version)
{
    ar & a.legs_;
    if (version > 0)
        ar & a.name_;
}

class bird : public animal
{
public:
    bird() : animal() {}
    bird(int legs, const std::string& name, bool can_fly)
        : animal(legs, name), can_fly_(can_fly)
    {}

    bool can_fly() const { return can_fly_; }
    void set_can_fly(bool can_fly) { can_fly_ = can_fly; }
private:
    bool can_fly_;
};

template<typename Archive>
void serialize(Archive& ar, bird& b, const unsigned int version)
{
    ar & boost::serialization::base_object<animal>(b);
    bool can_fly;
    ar & can_fly;
    b.set_can_fly(can_fly);
}

std::ostream& operator<<(std::ostream& os, const animal& a)
{
    return (os << "animal(" << a.legs() << "," << a.name() << ")");
}
std::ostream& operator<<(std::ostream& os, const bird& b)
{
    return (os << "bird(" << b.legs() << "," << b.name()
               << "," << std::boolalpha << b.can_fly()
               << ")");
}

}

BOOST_CLASS_VERSION(test::animal, 1)
/// Statically register derived class, to allow base pointer pointing to
/// derived instance be serialized
/// Could also use `archive::register_type<T>()` to do dynamic register on the
/// specified archive
BOOST_CLASS_EXPORT(test::bird)

int main(int argc, char* argv[])
{
    namespace archive = boost::archive;

    const char archive_filename[] = "serialization.txt";
    do {  // save
        // default, pointer, reference, smart pointer
        std::ofstream ofs(archive_filename);
        archive::text_oarchive aout(ofs);
        test::animal a1(0, "snake");
        test::animal* a2_ptr = new test::animal(2, "ape");
        test::animal a3(4, "mouse");
        test::animal& a3_ref = a3;
        boost::scoped_ptr<test::animal> a4_sptr(new test::animal(8, "octopus"));
        aout << a1 << a2_ptr << a3_ref << a4_sptr;
        delete a2_ptr;

        // derived class
        test::bird b1(2, "penguin", false);
        test::animal* b2_ptr = new test::bird(2, "sparrow", true);
        aout << b1 << b2_ptr;
        delete b2_ptr;

        // wrapper and optimization
        boost::array<int, 3> arr1 = {3, 5, 7};
        boost::array<int, 3> arr2 = {2, 4, 6};
        aout << arr1 << boost::serialization::make_array(arr2.data(), arr2.size());
    } while (0);

    do {  // restore
        std::ifstream ifs(archive_filename);
        archive::text_iarchive ain(ifs);
        test::animal a1;
        test::animal* a2_ptr;
        test::animal a3;
        test::animal& a3_ref = a3;
        boost::scoped_ptr<test::animal> a4_sptr;
        ain >> a1 >> a2_ptr >> a3_ref >> a4_sptr;
        std::cout << a1 << '\n'
                  << *a2_ptr << '\n'
                  << a3 << '\n'
                  << *a4_sptr << '\n';
        delete a2_ptr;

        test::bird b1;
        test::animal* b2_ptr;
        ain >> b1 >> b2_ptr;
        std::cout << b1 << '\n'
                  << *reinterpret_cast<test::bird*>(b2_ptr) << '\n';
        delete b2_ptr;

        boost::array<int, 3> arr1;
        boost::array<int, 3> arr2;
        ain >> arr1 >> boost::serialization::make_array(arr2.begin(), arr2.size());
        std::cout << "arr1: " << arr1[0] << ' ' << arr1[1] << ' ' << arr1[2] << '\n'
                  << "arr2: " << arr2[0] << ' ' << arr2[1] << ' ' << arr2[2] << '\n';
    } while (0);
}
