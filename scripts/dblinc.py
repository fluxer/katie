#!/usr/bin/python

import os, re

regex = re.compile('(#include [<|"](?:.*)[>|"])')

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
        for match in regex.findall(cppcontent):
            if match in includes:
                print('multiple inclusions of: %s, in %s' % (match, cpp))
                continue
            includes.append(match)
