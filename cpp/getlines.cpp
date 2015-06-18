// getlines.cpp
//
// `std::getline` replacement when handling many lines, more efficient and
// composible
//
#include <cassert>
#include <fstream>
#include <iostream>
#include <iterator>
#include <sstream>
#include <stdexcept>

class lines_iterator : public std::istream_iterator<std::string>
{
public:
    lines_iterator() : pstream(nullptr) {}
    lines_iterator(std::istream& stream) : pstream(&stream) {
        next();
    }
    const std::string& operator*() {
        return get();
    }
    const std::string* operator->() {
        return &get();
    }
    lines_iterator& operator++() {
        next();
        return *this;
    }
    lines_iterator operator++(int) {
        lines_iterator origin = *this;
        next();
        return origin;
    }
    bool operator==(const lines_iterator& that) const { return equal(that); }
    bool operator!=(const lines_iterator& that) const { return !(*this == that); }

private:
    const std::string& get() {
        if (!!pstream)
            return line;
        throw std::runtime_error("iterator end encountered");
    }
    void next() {
        if (!std::getline(*pstream, line))
            *this = lines_iterator();
    }
    bool equal(const lines_iterator& that) const {
        return (pstream == that.pstream);
    }

    istream_type* pstream;
    std::string line;
};

class lines_range
{
public:
    lines_range(std::istream& stream)
        : begin_(stream), end_() {}
    lines_iterator begin() { return begin_; }
    lines_iterator end() { return end_; }
private:
    lines_iterator begin_;
    lines_iterator end_;
};

lines_range getlines(std::istream& stream)
{
    return lines_range(stream);
}

int main(int argc, char* argv[])
{
    if (argc != 2) {
        std::cout << "Usage: getlines <filename>" << std::endl;
        return 0;
    }

    const char* filename = argv[1];
    std::ifstream fin(filename);
    assert(fin.is_open());
    for (const std::string& line : getlines(fin))
        std::cout << line << std::endl;
#if 0  // alternative
    for (auto it = lines_iterator(fin); it != lines_iterator(); ++it)
        std::cout << *it << std::endl;
#endif
    fin.close();
    // end iterator
    lines_iterator end;
    try {
        std::cout << "end iterator: " << *end << std::endl;
    } catch (std::runtime_error& e) {
        std::cerr << "[error] " << e.what() << std::endl;
    }

    return 0;
}
