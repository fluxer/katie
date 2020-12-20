#!/usr/bin/python

# See https://github.com/fluxer/katie/wiki/Benchmarking-vs-Qt4

import os, sys, subprocess
import xml.etree.ElementTree as ET

def medianvalues(v1, v2):
    maxv = 1000000
    if v1 > maxv and v2 > maxv:
        v1 = v1 / 1024
        v2 = v2 / 1024
    return (v1, v2)

if len(sys.argv) < 3:
    print("usage: <path to Katie test> <path to Qt4 test>")
    sys.exit(1)

katietest = os.path.realpath(sys.argv[1])
katietestdir = os.path.dirname(katietest)
commonargs = ['-iterations', '10', '-tickcounter', '-xml']
katieargs = [katietest, '-o', '/tmp/katie-bench.xml']
qt4test = os.path.realpath(sys.argv[2])
qt4testdir = os.path.dirname(qt4test)
qt4args = [qt4test, '-o', '/tmp/qt4-bench.xml']
katieargs.extend(commonargs)
qt4args.extend(commonargs)

print('Running Katie benchmark..')
subprocess.check_call(katieargs, cwd=katietestdir)

print('Running Qt4 benchmark..')
subprocess.check_call(qt4args, cwd=qt4testdir)

print('Parsing benchmark results..')
katiemap = {}
tree = ET.parse('/tmp/katie-bench.xml')
root = tree.getroot()
for testfunc in root:
    name = testfunc.get('name')
    for testresult in testfunc:
        tag = testresult.get('tag')
        if not tag:
            tag = "notag"
        value = testresult.get('value')
        if not value:
            continue
        katiemap['%s_%s' % (name, tag)] = value

qt4map = {}
tree = ET.parse('/tmp/qt4-bench.xml')
root = tree.getroot()
for testfunc in root:
    name = testfunc.get('name')
    for testresult in testfunc:
        tag = testresult.get('tag')
        if not tag:
            tag = "notag"
        value = testresult.get('value')
        if not value:
            continue
        qt4map['%s_%s' % (name, tag)] = value

print('Comparing benchmark results..')
tagmax = 0
for ktag in katiemap:
    if not ktag in qt4map:
        continue
    taglen = len(ktag)
    if taglen > tagmax:
        tagmax = taglen

if tagmax == 0:
    print('No common benchmarks')
    sys.exit(2)

infomax = (tagmax + 30)
infoalign = (tagmax - 2)
print('*' * infomax)
print(' Tag%s* Ticks Katie/Qt4 (Winner) *' % (' ' * infoalign))
print('*' * infomax)
ktotal = 0
qtotal = 0
for ktag in sorted(katiemap.keys()):
    if not ktag in qt4map:
        continue
    kvalue = katiemap[ktag]
    ikvalue = float(kvalue)
    qvalue = qt4map[ktag]
    iqvalue = float(qvalue)
    tagalign = tagmax - len(ktag) + 1
    winner = 'Katie'
    ikvalue, iqvalue = medianvalues(ikvalue, iqvalue)
    if ikvalue > iqvalue:
        winner = 'Qt4'
    ktotal += ikvalue
    qtotal += iqvalue
    print(' %s%s: %d/%d (%s)' % (ktag, ' ' * tagalign, ikvalue, iqvalue, winner))

totalalign = (tagmax - 4)
totalwinner = 'Katie'
if ktotal > qtotal:
    totalwinner = 'Qt4'
print('')
print('*' * infomax)
print(' Total%s: %d/%d (%s)' % (' ' * totalalign, ktotal, qtotal, totalwinner))
print('*' * infomax)
