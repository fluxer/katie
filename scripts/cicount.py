#!/usr/bin/python

import os, sys

cicountfile = "/home/appveyor/.ccache/cicount"

cicount = 0
try:
    with open(cicountfile, 'r') as f:
        cicontent = f.read()
        cicount = int(cicontent)
except:
    cicount = 10

if cicount >= 10:
    import randconfig
    cicount = 0

cicount += 1
with open(cicountfile, 'w') as f:
    f.write(str(cicount))
