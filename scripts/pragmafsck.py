#!/usr/bin/python

import os, re

regex = re.compile('(#ifndef (?:.*)_H\n#define .*$)', re.MULTILINE)

hppfiles = []
for root, dirs, files in os.walk(os.curdir):
    for f in files:
        if f.endswith(('.hpp', '.h')):
            hppfiles.append('%s/%s' % (root, f))

for hpp in hppfiles:
    hpp = os.path.realpath(hpp)
    with open(hpp, 'r') as f:
        cppcontent = f.read()
    if '#pragma once' in cppcontent:
        continue
    for match in regex.findall(cppcontent):
        with open(hpp, 'w') as f:
            print('adding pragma once to: %s' % hpp)
            cppcontent = cppcontent.replace(match, '#pragma once\n\n%s' % match)
            f.write(cppcontent)
