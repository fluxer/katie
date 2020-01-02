#!/usr/bin/python

import os

licenses = ['BSD', 'FDL', 'LGPL', 'LGPL3', 'LGPL21']

lfiles = []
for root, subdirs, files in os.walk(os.curdir):
    for sfile in files:
        if sfile.endswith(('.cpp', '.h', '.js', '.qs', '.qml', '.ui', '.g')):
            lfiles.append('%s/%s' % (root, sfile))

def readlicense(sfile, replacestars):
    sheader = ''
    with open(sfile, 'r') as f:
        shouldappend = False
        for sline in f.readlines():
            if 'QT_BEGIN_LICENSE' in sline:
                shouldappend = True
            elif 'QT_END_LICENSE' in sline:
                sheader = '%s%s' % (sheader, sline)
                shouldappend = False
            if shouldappend:
                sheader = '%s%s' % (sheader, sline)
        if replacestars:
            sheader = sheader.replace('**', '--')
    return sheader

for sfile in lfiles:
    with open(sfile, 'r') as f:
        scontent = f.read()
    for license in licenses:
        if ('$QT_BEGIN_LICENSE:%s$' % license) in scontent:
            snewheader = readlicense('%s/header.%s' % (os.curdir, license), sfile.endswith('.g'))
            soldheader = readlicense(sfile, False)
            snewcontent = scontent.replace(soldheader, snewheader)
            snewcontent = snewcontent.replace('2016-2019', '2016-2020')
            if not snewcontent == scontent:
                with open(sfile, 'w') as f:
                    f.write(snewcontent)
            
                
