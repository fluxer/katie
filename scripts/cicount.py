#!/usr/bin/python

import os, sys

cicountdir = "/home/appveyor/.ccache"
cicountfile = cicountdir + "/cicount"

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

if not os.path.isdir(cicountdir):
    os.makedirs(cicountdir)
cicount += 1
with open(cicountfile, 'w') as f:
    f.write(str(cicount))
