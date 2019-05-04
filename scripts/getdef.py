#!/usr/bin/python

import os, re

cwd = os.path.dirname(__file__)
regex = re.compile('defined\((QT_NO_[^\)]+)|def (QT_NO_[\w]+)')
lmatches = []

for root, subdirs, files in os.walk('%s/../src' % cwd):
    for sfile in files:
        if sfile.endswith(('.cpp', '.h')):
            sfull = '%s/%s' % (root, sfile)
            with open(sfull, 'rb') as f:
                scontent = f.read()
            scontent = scontent.decode('utf-8')
            for smatch, smatch2 in regex.findall(scontent):
                if smatch and not smatch in lmatches:
                    lmatches.append(smatch)
                if smatch2 and not smatch2 in lmatches:
                    lmatches.append(smatch2)

print('\n'.join(sorted(lmatches)))
