#!/usr/bin/env python3


def hash(s, c):
    h = 0
    for i in range(len(s)):
        h += ord(s[i]) + (i * 1)
    h += ord(c)
    print(h)
    h %= 2

    return h


print(hash("q0", "0"))
print(hash("q1", "0"))
