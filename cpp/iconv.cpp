// iconv.cpp
//
#include <iconv.h>
#include <errno.h>
#include <string.h>

#include <iostream>
#include <fstream>

#include <boost/filesystem.hpp>

int main(int argc, char* argv[])
{
    if (argc != 3) {
        std::cout << "Usage: iconv from_encoding to_encoding" << '\n';
        exit(0);
    }
    const char* fromcode = argv[1];
    const char* tocode = argv[2];
    iconv_t cd = iconv_open(tocode, fromcode);
    if (cd == (iconv_t)-1) {
        std::cout << "[error] conversion from \"" << fromcode << "\""
                  << " to \"" << tocode << "\" not supported" << '\n';
        exit(1);
    }

    std::ifstream fin("iconv.txt");
    std::string path;
    std::getline(fin, path);
    fin.close();

    char inbuf[1024] = { 0 };
    strcpy(inbuf, path.c_str());
    size_t inbuflen = path.length();
    namespace fs = boost::filesystem;
    std::cout << "[original] " << inbuf <<", exists: "
              << fs::exists(fs::path(inbuf)) << '\n';
    char outbuf[1024] = { 0 };
    size_t outbuflen = sizeof(outbuf) - 1;
    char* inbufptr = (char*)inbuf;
    char* outbufptr = (char*)outbuf;
    size_t ret = iconv(cd, (char**)&inbufptr, &inbuflen, (char**)&outbufptr, &outbuflen);
    if (errno == 0) {
        std::cout << "[converted] " << outbuf << ", exists: "
                  << fs::exists(fs::path(outbuf)) << '\n';
    }
    iconv_close(cd);
}
