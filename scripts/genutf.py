#!/usr/bin/python

# See https://github.com/fluxer/katie/wiki/Unicode-support

import sys

def readlines(fromfile):
    lines = []
    with open(fromfile, 'r') as f:
        content = f.read()
    for line in content.splitlines():
        if line.startswith('#') or not line:
            continue
        commentsplit = line.split('#')
        lines.append(commentsplit[0])
    return lines

def mapinsert(tomap, key, value):
    if not key in tomap.keys():
        tomap[key] = [value]
    else:
        tomap[key].append(value)

def mapdecideinsert(toswitchmap, torangemap, value, key):
    if '..' in key:
        rangesplit = key.split('..')
        rangemin = rangesplit[0]
        rangemax = rangesplit[1]
        # use switch statement if the range is 1
        irangemax = int(rangemax, 16)
        if (int(rangemin, 16) + 1) == irangemax or (int(rangemin, 16) - 1) == irangemax:
            mapinsert(toswitchmap, value, rangemin)
            mapinsert(toswitchmap, value, rangemax)
        else:
            mapinsert(torangemap, value, key)
    else:
        mapinsert(toswitchmap, value, key)

def printswitch(frommap):
    print('    switch (ucs4) {')
    for key in frommap:
        for case in frommap[key]:
            print('        case 0x%s:' % case)
        print('            return %s;' % key)
    print('    }')

def printifrange(frommap):
    for key in frommap:
        for r in frommap[key]:
            rangesplit = r.split('..')
            rangemin = rangesplit[0]
            rangemax = rangesplit[1]
            print('    if (ucs4 <= 0x%s && ucs4 >= 0x%s)' % (rangemax, rangemin))
            print('        return %s;' % key)

switchmap = {}
rangemap = {}

if 'grapheme' in sys.argv:
    # only some are supported by harfbuzz
    supported = [
        'CR',
        'LF',
        'Control',
        'Extend',
        'L',
        'V',
        'T',
        'LV',
        'LVT',
    ]
    for line in readlines('auxiliary/GraphemeBreakProperty.txt'):
        tablesplit = line.split(';')
        codepoint = tablesplit[0].strip()
        value = tablesplit[1].strip()
        if not value in supported:
            continue
        value = 'QUnicodeTables::GraphemeBreak_%s' % value.replace('_', '')
        mapdecideinsert(switchmap, rangemap, value, codepoint)
    printswitch(switchmap)
    printifrange(rangemap)
elif 'word' in sys.argv:
    # only some are supported by harfbuzz
    supported = [
        'Format',
        'Katakana',
        'ALetter',
        'MidLetter',
        'MidNum',
        'Numeric',
        'ExtendNumLet',
    ]
    for line in readlines('auxiliary/WordBreakProperty.txt'):
        tablesplit = line.split(';')
        codepoint = tablesplit[0].strip()
        value = tablesplit[1].strip()
        if not value in supported:
            continue
        value = 'QUnicodeTables::WordBreak_%s' % value.replace('_', '')
        mapdecideinsert(switchmap, rangemap, value, codepoint)
    printswitch(switchmap)
    printifrange(rangemap)
elif 'sentence' in sys.argv:
    # only some are supported by harfbuzz
    supported = [
        'Sep',
        'Format',
        'Sp',
        'Lower',
        'Upper',
        'OLetter',
        'Numeric',
        'ATerm',
        'STerm',
        'Close',
    ]
    for line in readlines('auxiliary/SentenceBreakProperty.txt'):
        tablesplit = line.split(';')
        codepoint = tablesplit[0].strip()
        value = tablesplit[1].strip()
        if not value in supported:
            continue
        value = 'QUnicodeTables::SentenceBreak_%s' % value.replace('_', '')
        mapdecideinsert(switchmap, rangemap, value, codepoint)
    printswitch(switchmap)
    printifrange(rangemap)
elif 'line' in sys.argv:
    # only some are supported by harfbuzz
    supported = [
        'OP',
        'CL',
        'QU',
        'GL',
        'NS',
        'EX',
        'SY',
        'IS',
        'PR',
        'PO',
        'NU',
        'AL',
        'ID',
        'IN',
        'HY',
        'BA',
        'BB',
        'B2',
        'ZW',
        'CM',
        'WJ',
        'H2',
        'H3',
        'JL',
        'JV',
        'JT',
        'SA',
        'SG',
        'SP',
        'CR',
        'LF',
        'BK'
    ]
    for line in readlines('LineBreak.txt'):
        tablesplit = line.split(';')
        codepoint = tablesplit[0].strip()
        value = tablesplit[1].strip()
        if not value in supported:
            continue
        value = 'QUnicodeTables::LineBreak_%s' % value.replace('_', '')
        mapdecideinsert(switchmap, rangemap, value, codepoint)
    printswitch(switchmap)
    printifrange(rangemap)
elif 'script' in sys.argv:
    for line in readlines('Scripts.txt'):
        tablesplit = line.split(';')
        codepoint = tablesplit[0].strip()
        value = tablesplit[1].strip()
        if value == 'Common':
            # default is Common
            continue
        value = 'QUnicodeTables::%s' % value.replace('_', '')
        mapdecideinsert(switchmap, rangemap, value, codepoint)
    printswitch(switchmap)
    printifrange(rangemap)
elif 'special' in sys.argv:
    scriptsmap = {}
    for line in readlines('Scripts.txt'):
        tablesplit = line.split(';')
        codepoint = tablesplit[0].strip()
        value = tablesplit[1].strip()
        if value in ('Inherited', 'Common'):
            # both are treated differently
            codepoint = '0'
        if value in scriptsmap.keys():
            # only one per script
            continue
        if '..' in codepoint:
            rangesplit = codepoint.split('..')
            codepoint = rangesplit[0]
        scriptsmap[value] = codepoint

    print('    0x%s, // Common' % scriptsmap['Common'])
    for value in sorted(scriptsmap.keys()):
        if (value == 'Common'):
            continue
        print('    0x%s, // %s' % (scriptsmap[value], value.replace('_', '')))
else:
    print('''usage: <grapheme|word|sentence|line|script|special>

Data is from https://unicode.org/Public/14.0.0/ucd/UCD.zip''')
    sys.exit(1)
