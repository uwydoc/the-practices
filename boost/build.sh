#!/bin/bash

[ -z "$1" ] && echo "Usage: build.sh file [other_file...]" && exit 0

set -e

CC=g++
BOOST_ROOT=/d/Downloads/boost-build-1_55_0
BOOST_LIBRARYDIR=/d/Downloads/boost-build-1_55_0/mingw/stage/lib

# compile
ls $@ | while read filename; do
    outfile=${filename%.*};
    rm -f runtime/$outfile;
    $CC $CFLAGS $CXXFLAGS -I "$BOOST_ROOT" -L "$BOOST_LIBRARYDIR" -o runtime/$outfile $filename $LDFLAGS || break;
done

# run
ls $@ | while read filename; do
    outfile=${filename%.*};
    echo "== $outfile ==";
    ./runtime/$outfile;
    echo "";
done
