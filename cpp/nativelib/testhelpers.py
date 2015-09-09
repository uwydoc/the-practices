#!/usr/bin/env python
# -*- coding: utf-8 -*-
#
import os
import subprocess
from cffi import FFI

here = os.path.abspath(os.path.dirname(__file__))
header = os.path.join(here, 'echo.h')
preprocessed_header = os.path.join(here, 'echo.i')

if not os.path.exists(preprocessed_header):
    subprocess.Popen([
        "cl", "/EP", "/P", "/DNATIVELIB_ECHO_API=",
        header], stdout=subprocess.PIPE, stderr=subprocess.PIPE).wait()

ffi = FFI()
with open(preprocessed_header) as f:
    cdefs = f.read().replace('\r\n', '\n')
    ffi.cdef(cdefs)
lib = ffi.dlopen(os.path.join(here, 'bin', 'Release', 'echo_shared.dll'))
