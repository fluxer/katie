#!/usr/bin/python

import sys
from collections import OrderedDict

def normalizespace(sstring):
    result = sstring.replace('\t', ' ')
    for r in range(10):
        result = result.replace('  ', ' ')
    return result.strip()

def removespace(sstring):
    result = normalizespace(sstring)
    result = result.replace(' ', '')
    return result.strip()

rgbmap = OrderedDict()
with open(sys.argv[1], 'rb') as f:
    for sline in f.readlines():
        sline = sline.decode('utf-8')
        sline = normalizespace(sline)
        llineparts = sline.split(' ')
        if len(llineparts) != 4:
            continue
        scolor = removespace(llineparts[3].lower())
        rgbmap[scolor] = {
            'r': llineparts[0],
            'g': llineparts[1],
            'b': llineparts[2],
        }

print('''static const struct X11RGBTblData {
    const QRgb value;
    const char *name;
} X11RGBTbl[] = {''')

for rgb in sorted(rgbmap.keys()):
    print('    { qRgb(%s, %s, %s), "%s" },' % (rgbmap[rgb]['r'], rgbmap[rgb]['g'], rgbmap[rgb]['b'], rgb))

print('''};
static const qint16 X11RGBTblSize = sizeof(X11RGBTbl) / sizeof(X11RGBTblData);''')
