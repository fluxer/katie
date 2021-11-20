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

if 'combining' in sys.argv:
    for line in readlines('extracted/DerivedCombiningClass.txt'):
        tablesplit = line.split(';')
        codepoint = tablesplit[0].strip()
        value = tablesplit[1].strip()
        if value == '0':
            # not ordered is default
            continue
        elif value == '202':
            value = 'QUnicodeTables::Combining_AttachedBelow'
        elif value == '214':
            value = 'QUnicodeTables::Combining_AttachedAbove'
        elif value == '216':
            value = 'QUnicodeTables::Combining_AttachedAboveRight'
        elif value == '218':
            value = 'QUnicodeTables::Combining_BelowLeft'
        elif value == '220':
            value = 'QUnicodeTables::Combining_Below'
        elif value == '222':
            value = 'QUnicodeTables::Combining_BelowRight'
        elif value == '224':
            value = 'QUnicodeTables::Combining_Left'
        elif value == '226':
            value = 'QUnicodeTables::Combining_Right'
        elif value == '228':
            value = 'QUnicodeTables::Combining_AboveLeft'
        elif value == '230':
            value = 'QUnicodeTables::Combining_Above'
        elif value == '232':
            value = 'QUnicodeTables::Combining_AboveRight'
        elif value == '233':
            value = 'QUnicodeTables::Combining_DoubleBelow'
        elif value == '234':
            value = 'QUnicodeTables::Combining_DoubleAbove'
        elif value == '240':
            value = 'QUnicodeTables::Combining_IotaSubscript'
        elif value in ['200', '204', '208', '210', '212']:
            print('Unhandled deprecated combining type: %s' % value)
            sys.exit(2)
        else:
            # only some are supported by harfbuzz
            continue
        mapdecideinsert(switchmap, rangemap, value, codepoint)
    printswitch(switchmap)
    printifrange(rangemap)
elif 'grapheme' in sys.argv:
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
    scriptslist = []
    for line in readlines('Scripts.txt'):
        tablesplit = line.split(';')
        codepoint = tablesplit[0].strip()
        value = tablesplit[1].strip()
        if value in ('Inherited', 'Common'):
            # both are treated differently
            codepoint = '0'
        if value in scriptslist:
            # only one per script
            continue
        elif '..' in codepoint:
            rangesplit = codepoint.split('..')
            rangemin = rangesplit[0]
            print('    0x%s, // %s' % (rangemin, value.replace('_', '')))
        else:
            print('    0x%s, // %s' % (codepoint, value.replace('_', '')))
        scriptslist.append(value)
else:
    print('''usage: <combining|grapheme|word|sentence|line|script|special>

Data is from https://unicode.org/Public/14.0.0/ucd/UCD.zip''')
    sys.exit(1)
