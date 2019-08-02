#!/usr/bin/python
#-*- coding: UTF-8 -*-

import os, sys
import xml.etree.ElementTree as ET

def stripstring(fromxmlstring):
    result = fromxmlstring.replace('\n', '')
    result = result.replace('\t', '')
    return result.strip()

def normalizestring(fromstring):
    result = fromstring.replace(' ', '')
    result = result.replace('-', '')
    result = result.replace("'", '')
    result = result.replace('&', 'And')
    result = result.replace('(', '')
    result = result.replace(')', '')
    result = result.replace('St.', 'St')
    result = result.replace('U.S.', 'UnitedStates')
    # UTF-8 chars
    result = result.replace(u'ʼ', '')
    result = result.replace(u'’', '')
    result = result.replace(u'ü', 'u')
    result = result.replace(u'å', 'a')
    result = result.replace(u'ç', 'c')
    result = result.replace(u'õ', 'o')
    result = result.replace(u'Å', 'A')
    result = result.replace(u'ô', 'o')
    result = result.replace(u'ã', 'a')
    result = result.replace(u'é', 'e')
    result = result.replace(u'í', 'i')
    return result

def printenum(frommap, prefix):
    keyscount = 0

    # print Default and C first
    for key in sorted(frommap.keys()):
        if not key in ('Any%s' % prefix, 'C'):
            continue
        print('        %s = %d,' % (key, keyscount))
        keyscount += 1

    # now everything except those
    lastkey = ''
    for key in sorted(frommap.keys()):
        if key in ('Any%s' % prefix, 'C'):
            continue
        print('        %s = %d,' % (key, keyscount))
        lastkey = key
        keyscount += 1

    # print last key
    print('\n        Last%s = %s' % (prefix, lastkey))

def printtable(frommap, prefix):
    lowerprefix = prefix.lower()
    print('''static const struct %sTblData {
    const char* name;
    const char* code;
    const QLocale::%s %s;
} %sTbl[] = {''' % (lowerprefix, prefix, lowerprefix, lowerprefix))

    # print Default and C first
    for key in frommap.keys():
        if not key in ('Any%s' % prefix, 'C'):
            continue
        firstvalue = frommap[key][0]
        secondvalue = frommap[key][1]
        print('    { "%s\\0", "%s\\0", QLocale::%s::%s },' % (secondvalue, firstvalue, prefix, key))

    # now everything except those
    for key in sorted(frommap.keys()):
        if key in ('Any%s' % prefix, 'C'):
            continue
        firstvalue = frommap[key][0]
        secondvalue = frommap[key][1]
        print('    { "%s\\0", "%s\\0", QLocale::%s::%s },' % (secondvalue, firstvalue, prefix, key))

    print('};')
    print('static const qint16 %sTblSize = sizeof(%sTbl) / sizeof(%sTblData);' % (lowerprefix, lowerprefix, lowerprefix))

printmap = {}

if 'language' in sys.argv:
    # artificial values
    printmap['AnyLanguage'] = ['', 'Default']
    printmap['C'] = ['', 'C']

    tree = ET.parse('common/main/en.xml')
    root = tree.getroot()
    for language in root.findall('./localeDisplayNames/languages/language'):
        languagetype = language.get('type')
        normallanguage = normalizestring(language.text)
        if normallanguage in ('Nauru', 'Tokelau', 'Tuvalu'):
            # countries and language are the same, suffix to solve enum clashes
            normallanguage = '%sLanguage' % normallanguage
        printmap[normallanguage] = [languagetype, language.text]

    # printenum(printmap, 'Language')
    printtable(printmap, 'Language')

elif 'country' in sys.argv:
    # artificial values
    printmap['AnyCountry'] = ['', 'Default']

    tree = ET.parse('common/main/en.xml')
    root = tree.getroot()
    for country in root.findall('./localeDisplayNames/territories/territory'):
        countrytype = country.get('type')
        normalcountry = normalizestring(country.text)
        if normalcountry == 'World':
            # only interested in specific countries
            continue
        printmap[normalcountry] = [countrytype, country.text]

    # printenum(printmap, 'Country')
    printtable(printmap, 'Country')

elif 'script' in sys.argv:
    # artificial values
    printmap['AnyScript'] = ['', 'Default']

    tree = ET.parse('common/main/en.xml')
    root = tree.getroot()
    scriptcount = 1
    for script in root.findall('./localeDisplayNames/scripts/script'):
        scripttype = script.get('type')
        normalscript = normalizestring(script.text)
        if not normalscript.endswith('Script'):
            # suffix script if needed
            normalscript = '%sScript' % normalscript
        if normalscript in ('UnknownScript', 'CommonScript'):
            # only interested in specific scripts
            continue
        printmap[normalscript] = [scripttype, script.text]

    # printenum(printmap, 'Script')
    printtable(printmap, 'Script')

else:
    print('''usage: <language|country|script>

Data is from https://unicode.org/Public/cldr/35/core.zip''')
    sys.exit(1)
