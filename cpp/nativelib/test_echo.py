#!/usr/bin/env python
# -*- coding: utf-8 -*-
#
from __future__ import print_function, division
import time
from testhelpers import ffi, lib

def test_echo():
    print("nativelib echo:", lib.ne_get_version())
    e = lib.ne_echo_new(b"hello world")
    print("msg:", lib.ne_echo_get_msg(e))
    lib.ne_echo_set_msg(e, b"biubiu")
    lib.ne_echo_echo(e)
