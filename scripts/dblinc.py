#!/usr/bin/python

import sys, os, re

cppfiles = []
for root, dirs, files in os.walk(os.curdir):
    for f in files:
        if f.endswith(('.cpp', '.cc', '.hpp', '.h')):
            cppfiles.append('%s/%s' % (root, f))

for cpp in cppfiles:
    cpp = os.path.realpath(cpp)
    cppbase = os.path.basename(cpp)
    with open(cpp, 'r') as f:
        cppcontent = f.read()
        includes = []
        for match in re.findall('(#include [<|"](?:.*)[>|"])', cppcontent):
            if match in includes:
                print('multiple inclusions of: %s, in %s' % (match, cpp))
                continue
            includes.append(match)
