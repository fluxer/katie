#!/usr/bin/python

import os, sys, subprocess
import xml.etree.ElementTree as ET

if len(sys.argv) < 3:
    print("usage: <path to Katie test> <path to Qt4 test>")
    sys.exit(1)

commonargs = ['-iterations', '100', '-tickcounter', '-xml']
katieargs = [sys.argv[1], '-o', '/tmp/katie-bench.xml']
qt4args = [sys.argv[2], '-o', '/tmp/qt4-bench.xml']
katieargs.extend(commonargs)
qt4args.extend(commonargs)

print('Running Katie benchmark..')
subprocess.check_call(katieargs)

print('Running Qt4 benchmark..')
subprocess.check_call(qt4args)

print('Parsing benchmark results..')
katiemap = {}
tree = ET.parse('/tmp/katie-bench.xml')
root = tree.getroot()
for testfunc in root:
    for testresult in testfunc:
        tag = testresult.get('tag')
        if not tag:
            continue
        value = testresult.get('value')
        katiemap[tag] = value

qt4map = {}
tree = ET.parse('/tmp/qt4-bench.xml')
root = tree.getroot()
for testfunc in root:
    for testresult in testfunc:
        tag = testresult.get('tag')
        if not tag:
            continue
        value = testresult.get('value')
        qt4map[tag] = value

print('Comparing benchmark results..')
tagmax = 0
for ktag in katiemap:
    if not ktag in qt4map:
        continue
    taglen = len(ktag)
    if taglen > tagmax:
        tagmax = taglen

infomax = (tagmax + 30)
infoalign = (tagmax - 2)
print('*' * infomax)
print(' Tag%s* Ticks Katie/Qt4 (Winner) *' % (' ' * infoalign))
print('*' * infomax)
ktotal = 0
qtotal = 0
for ktag in katiemap:
    if not ktag in qt4map:
        # print('    filtering Katie tag: %s' % tag)
        continue
    kvalue = katiemap[ktag]
    ikvalue = float(kvalue)
    qvalue = qt4map[ktag]
    iqvalue = float(qvalue)
    tagalign = tagmax - len(ktag) + 1
    winner = 'Katie'
    if ikvalue > iqvalue:
        winner = 'Qt4'
    ktotal += ikvalue
    qtotal += iqvalue
    print(' %s%s: %d/%d (%s)' % (ktag, ' ' * tagalign, ikvalue, iqvalue, winner))

totalalign = (tagmax - 3)
totalwinner = 'Katie'
if ktotal > qtotal:
    totalwinner = 'Qt4'
print('')
print('*' * infomax)
print('Total%s: %d/%d (%s)' % (' ' * totalalign, ktotal, qtotal, winner))
print('*' * infomax)
