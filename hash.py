#!/usr/bin/env python3

import sys


def hash(s, c):
    path_hash = 0
    for i in range(len(s)):
        path_hash += ord(s[i]) * (i + 1)
    path_hash += ord(c)
    path_hash %= 6
    return path_hash


print(hash(sys.argv[1], sys.argv[2]))
