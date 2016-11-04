#!/usr/bin/python

import os, re

cwd = os.path.dirname(__file__)
regex = re.compile('getenv\("([^\)]+)')
lmatches = []

for root, subdirs, files in os.walk('%s/../src' % cwd):
    for sfile in files:
        if sfile.endswith(('.cpp', '.h')):
            sfull = '%s/%s' % (root, sfile)
            with open(sfull, 'r') as f:
                for smatch in regex.findall(f.read()):
                    smatch = smatch.strip('"\'')
                    if not smatch in lmatches:
                        lmatches.append(smatch)

print('\n'.join(sorted(lmatches)))