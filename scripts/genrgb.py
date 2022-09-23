#!/usr/bin/python

# Data is from /usr/share/X11/rgb.txt

import sys
from collections import OrderedDict

def normalizespace(sstring):
    result = sstring.replace(' \t', ' ')
    result = result.replace('\t ', ' ')
    result = result.replace('\t\t', '\t')
    result = result.replace('\t', ' ')
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
        if sline.startswith('!'):
            continue
        sline = normalizespace(sline)
        llineparts = sline.split(' ')
        if len(llineparts) < 4:
            continue
        scolor = ' '.join(llineparts[3:])
        scolor = removespace(scolor.lower())
        rgbmap[scolor] = {
            'r': llineparts[0],
            'g': llineparts[1],
            'b': llineparts[2],
        }

print('''static const struct X11RGBTblData {
    const char *name;
    const QRgb value;
} X11RGBTbl[] = {''')

for rgb in sorted(rgbmap.keys()):
    print('    { "%s", qRgb(%s, %s, %s) },' % (rgb, rgbmap[rgb]['r'], rgbmap[rgb]['g'], rgbmap[rgb]['b']))

print('''};
static const qint16 X11RGBTblSize = sizeof(X11RGBTbl) / sizeof(X11RGBTblData);''')
