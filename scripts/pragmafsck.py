#!/usr/bin/python

import sys, os, re

regex = re.compile('(#ifndef (.*)_H\n#define .*$)', re.MULTILINE)

cppfiles = []
for root, dirs, files in os.walk(os.curdir):
    for f in files:
        if f.endswith(('.hpp', '.h')):
            cppfiles.append('%s/%s' % (root, f))

for cpp in cppfiles:
    cpp = os.path.realpath(cpp)
    with open(cpp, 'r') as f:
        cppcontent = f.read()
    if '#pragma once' in cppcontent:
        continue
    for match, guard in regex.findall(cppcontent):
        with open(cpp, 'w') as f:
            print('adding pragma once to: %s' % cpp)
            cppcontent = cppcontent.replace(match, '#pragma once\n\n%s' % match)
            f.write(cppcontent)
