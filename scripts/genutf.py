#!/usr/bin/python

import os, sys

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

if 'digit' in sys.argv:
    for line in readlines('extracted/DerivedNumericValues.txt'):
        tablesplit = line.split(';')
        codepoint = tablesplit[0].strip()
        value = tablesplit[3].strip()
        if '/' in value:
            # TODO: handle fraction
            continue
        mapdecideinsert(switchmap, rangemap, value, codepoint)
    printswitch(switchmap)
    printifrange(rangemap)
elif 'joining' in sys.argv:
    joiningmap = {}
    for line in readlines('extracted/DerivedJoiningType.txt'):
        tablesplit = line.split(';')
        codepoint = tablesplit[0].strip()
        value = tablesplit[1].strip()
        if value == 'C':
            value = 'QChar::Causing'
        elif value == 'D':
            value = 'QChar::Dual'
        elif value == 'R':
            value = 'QChar::Right'
        elif value == 'L':
            value = 'QChar::Left'
        elif value == 'T':
            value = 'QChar::Transparent'
        else:
            print('Unhandled joining type: %s' % value)
            sys.exit(2)
        mapdecideinsert(switchmap, rangemap, value, codepoint)
    printswitch(switchmap)
    printifrange(rangemap)
elif 'combining' in sys.argv:
    for line in readlines('extracted/DerivedCombiningClass.txt'):
        tablesplit = line.split(';')
        codepoint = tablesplit[0].strip()
        value = tablesplit[1].strip()
        if value == '0':
            # not ordered is default
            continue
        elif value == '1':
            value = 'QUnicodeTables::Combining_Overlay'
        elif value == '7':
            value = 'QUnicodeTables::Combining_Nukta'
        elif value == '8':
            value = 'QUnicodeTables::Combining_Kana_Voicing'
        elif value == '9':
            value = 'QUnicodeTables::Combining_Virama'
        elif value == '10':
            value = 'QUnicodeTables::Combining_CCC10'
        elif value == '11':
            value = 'QUnicodeTables::Combining_CCC11'
        elif value == '12':
            value = 'QUnicodeTables::Combining_CCC12'
        elif value == '13':
            value = 'QUnicodeTables::Combining_CCC13'
        elif value == '14':
            value = 'QUnicodeTables::Combining_CCC14'
        elif value == '15':
            value = 'QUnicodeTables::Combining_CCC15'
        elif value == '16':
            value = 'QUnicodeTables::Combining_CCC16'
        elif value == '17':
            value = 'QUnicodeTables::Combining_CCC17'
        elif value == '18':
            value = 'QUnicodeTables::Combining_CCC18'
        elif value == '19':
            value = 'QUnicodeTables::Combining_CCC19'
        elif value == '20':
            value = 'QUnicodeTables::Combining_CCC20'
        elif value == '21':
            value = 'QUnicodeTables::Combining_CCC21'
        elif value == '22':
            value = 'QUnicodeTables::Combining_CCC22'
        elif value == '23':
            value = 'QUnicodeTables::Combining_CCC23'
        elif value == '24':
            value = 'QUnicodeTables::Combining_CCC24'
        elif value == '25':
            value = 'QUnicodeTables::Combining_CCC25'
        elif value == '26':
            value = 'QUnicodeTables::Combining_CCC26'
        elif value == '27':
            value = 'QUnicodeTables::Combining_CCC27'
        elif value == '28':
            value = 'QUnicodeTables::Combining_CCC28'
        elif value == '29':
            value = 'QUnicodeTables::Combining_CCC29'
        elif value == '30':
            value = 'QUnicodeTables::Combining_CCC30'
        elif value == '31':
            value = 'QUnicodeTables::Combining_CCC31'
        elif value == '32':
            value = 'QUnicodeTables::Combining_CCC32'
        elif value == '33':
            value = 'QUnicodeTables::Combining_CCC33'
        elif value == '34':
            value = 'QUnicodeTables::Combining_CCC34'
        elif value == '35':
            value = 'QUnicodeTables::Combining_CCC35'
        elif value == '36':
            value = 'QUnicodeTables::Combining_CCC36'
        elif value == '84':
            value = 'QUnicodeTables::Combining_CCC84'
        elif value == '91':
            value = 'QUnicodeTables::Combining_CCC91'
        elif value == '103':
            value = 'QUnicodeTables::Combining_CCC103'
        elif value == '107':
            value = 'QUnicodeTables::Combining_CCC107'
        elif value == '118':
            value = 'QUnicodeTables::Combining_CCC118'
        elif value == '122':
            value = 'QUnicodeTables::Combining_CCC122'
        elif value == '129':
            value = 'QUnicodeTables::Combining_CCC129'
        elif value == '130':
            value = 'QUnicodeTables::Combining_CCC130'
        elif value == '132':
            value = 'QUnicodeTables::Combining_CCC132'
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
        else:
            print('Unhandled combining type: %s' % value)
            sys.exit(2)
        mapdecideinsert(switchmap, rangemap, value, codepoint)
    printswitch(switchmap)
    printifrange(rangemap)
elif 'mirror' in sys.argv:
    for line in readlines('BidiMirroring.txt'):
        tablesplit = line.split(';')
        codepoint = tablesplit[0].strip()
        value = tablesplit[1].strip()
        mapdecideinsert(switchmap, rangemap, '0x%s' % value, codepoint)
    printswitch(switchmap)
    printifrange(rangemap)
elif 'version' in sys.argv:
    for line in readlines('DerivedAge.txt'):
        tablesplit = line.split(';')
        codepoint = tablesplit[0].strip()
        value = tablesplit[1].strip()
        if value == '1.1':
            value = 'QChar::Unicode_1_1'
        elif value == '2.0':
            value = 'QChar::Unicode_2_0'
        elif value == '2.1':
            value = 'QChar::Unicode_2_1'
        elif value == '3.0':
            value = 'QChar::Unicode_3_0'
        elif value == '3.1':
            value = 'QChar::Unicode_3_1'
        elif value == '3.2':
            value = 'QChar::Unicode_3_2'
        elif value == '4.0':
            value = 'QChar::Unicode_4_0'
        elif value == '4.1':
            value = 'QChar::Unicode_4_1'
        elif value == '5.0':
            value = 'QChar::Unicode_5_0'
        elif value == '5.1':
            value = 'QChar::Unicode_5_1'
        elif value == '5.2':
            value = 'QChar::Unicode_5_2'
        elif value == '6.0':
            value = 'QChar::Unicode_6_0'
        elif value == '6.1':
            value = 'QChar::Unicode_6_1'
        elif value == '6.2':
            value = 'QChar::Unicode_6_2'
        elif value == '6.3':
            value = 'QChar::Unicode_6_3'
        elif value == '7.0':
            value = 'QChar::Unicode_7_0'
        elif value == '8.0':
            value = 'QChar::Unicode_8_0'
        elif value == '9.0':
            value = 'QChar::Unicode_9_0'
        elif value == '10.0':
            value = 'QChar::Unicode_10_0'
        elif value == '11.0':
            value = 'QChar::Unicode_11_0'
        elif value == '12.0':
            value = 'QChar::Unicode_12_0'
        elif value == '12.1':
            value = 'QChar::Unicode_12_1'
        else:
            print('Unhandled version type: %s' % value)
            sys.exit(2)
        mapdecideinsert(switchmap, rangemap, value, codepoint)
    printswitch(switchmap)
    printifrange(rangemap)
elif 'grapheme' in sys.argv:
    # only some are supported by harfbuzz
    supported = [
        'CR',
        'LF'
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
        'Katakana'
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
        'Sp'
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
        'QU'
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
    print('usage: <digit|joining|combining|mirror|version|grapheme|word|sentence|line|script|special>')
    sys.exit(1)
