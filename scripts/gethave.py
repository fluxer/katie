#!/usr/bin/python

import os, re

cwd = os.path.dirname(__file__)
regex = re.compile('(QT_HAVE_[\w]+)')
lmatches = []

for root, subdirs, files in os.walk('%s/../src' % cwd):
    for sfile in files:
        if sfile.endswith(('.cpp', '.h')):
            sfull = '%s/%s' % (root, sfile)
            with open(sfull, 'rb') as f:
                scontent = f.read()
            scontent = scontent.decode('utf-8')
            for smatch in regex.findall(scontent):
                smatch = smatch.strip('"\'')
                if not smatch in lmatches:
                    lmatches.append(smatch)

print('\n'.join(sorted(lmatches)))
