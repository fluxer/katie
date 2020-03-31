#!/usr/bin/python3
#-*- coding: UTF-8 -*-

# Data is from https://unicode.org/Public/cldr/36.1/core.zip

import sys, re
import xml.etree.ElementTree as ET

printenumsandexit = ('--printenums' in sys.argv)
printdocsandexit = ('--printdocs' in sys.argv)

def normalizestring(fromstring):
    result = fromstring.replace(' ', '')
    result = result.replace('-', '')
    result = result.replace(',', '')
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

def tochar(fromstring):
    if fromstring:
        return '"%s\\0"' % fromstring
    return 'Q_NULLPTR'

def tolanguageenum(fromstring):
    for key in languagemap.keys():
        if fromstring == languagemap[key]['code']:
            return 'QLocale::Language::%s' % key
    # print('Unknown language: %s' % fromstring)
    # sys.exit(1)

def toscriptenum(fromstring):
    for key in scriptmap.keys():
        if fromstring == scriptmap[key]['code']:
            return 'QLocale::Script::%s' % key
    # print('Unknown script: %s' % fromstring)
    # sys.exit(1)

def tocountryenum(fromstring):
    for key in countrymap.keys():
        if fromstring == countrymap[key]['code']:
            return 'QLocale::Country::%s' % key
    # print('Unknown country: %s' % fromstring)
    # sys.exit(1)

# printenum prints mapped values that have unique code only, the rest are set to the enum of the
# first occurence. the reason for doing so is because table lookups for figuring out language,
# script and country required for constructing QLocale from string (named locales) relies on the
# fact that there is only one code for each, if that is not the case constructing copy of locale
# from its name will not copy it correctly. printtable skips duplicate code entries entirely
def printenum(frommap, prefix):
    keyscount = 0
    aliascodes = []
    seencodes = []

    print('    enum %s {' % prefix)
    # print Default and C first
    for key in frommap.keys():
        if not key in ('Any%s' % prefix, 'C'):
            continue
        print('        %s = %d,' % (key, keyscount))
        keyscount += 1

    # now everything except those, save last key for later
    lastkey = ''
    for key in sorted(frommap.keys()):
        if key in ('Any%s' % prefix, 'C'):
            continue
        code = frommap[key]['code']
        if code in seencodes:
            aliascodes.append(key)
            continue
        seencodes.append(code)
        print('        %s = %d,' % (key, keyscount))
        lastkey = key
        keyscount += 1

    # now aliases
    print('')
    for alias in sorted(aliascodes):
        aliascode = frommap[alias]['code']
        aliasenum = None
        for key in sorted(frommap.keys()):
            code = frommap[key]['code']
            if aliascode == code:
                aliasenum == key
                break
        print('        %s = %s,' % (alias, key))

    # print last key
    print('\n        Last%s = %s' % (prefix, lastkey))

    print('    };\n')
def printdoc(frommap, prefix):
    print('// %s' % prefix)
    for key in sorted(frommap.keys()):
        if key in ('Any%s' % prefix, 'C'):
            continue
        code = frommap[key]['code']
        name = frommap[key]['name']
        print('    \\value %s' % key)
    print('')

def printtable(frommap, prefix):
    lowerprefix = prefix.lower()
    seencodes = []

    print('''static const struct %sTblData {
    const QLocale::%s %s;
    const char* name;
    const char* code;
} %sTbl[] = {''' % (lowerprefix, prefix, lowerprefix, lowerprefix))

    # print Default and C first
    for key in frommap.keys():
        if not key in ('Any%s' % prefix, 'C'):
            continue
        code = frommap[key]['code']
        name = frommap[key]['name']
        print('    { QLocale::%s::%s, %s, %s },' % (prefix, key, tochar(name), tochar(code)))

    # now everything except those but only unique code values
    for key in sorted(frommap.keys()):
        if key in ('Any%s' % prefix, 'C'):
            continue
        code = frommap[key]['code']
        if code in seencodes:
            continue
        seencodes.append(code)
        name = frommap[key]['name']
        print('    { QLocale::%s::%s, %s, %s },' % (prefix, key, tochar(name), tochar(code)))

    print('};')
    print('static const qint16 %sTblSize = sizeof(%sTbl) / sizeof(%sTblData);\n' % (lowerprefix, lowerprefix, lowerprefix))

def printaliastable(frommap, prefix):
    print('''static const struct %sAliasTblData {
    const QLatin1String original;
    const QLatin1String substitute;
} %sAliasTbl[] = {''' % (prefix, prefix))

    for key in sorted(frommap):
        # territories and scripts entries can contain multiple replacements, add one for each
        splitvalue = frommap[key].split(' ')
        for value in splitvalue:
            print('    { QLatin1String("%s"), QLatin1String("%s") },' % (key, value))

    print('};')
    print('static const qint16 %sAliasTblSize = sizeof(%sAliasTbl) / sizeof(%sAliasTblData);\n' % (prefix, prefix, prefix))

# main maps
languagemap = {}
countrymap = {}
scriptmap = {}
likelysubtagsmap = {}
languagealiasmap = {}
countryaliasmap = {}
scriptaliasmap = {}
# regular expressions
localeregex = re.compile('([^_|-|\.|@]+)+')

# artificial entries
languagemap['AnyLanguage'] = {
    'code': '',
    'name': 'Default',
}
languagemap['C'] = {
    'code': 'C',
    'name': 'C',
}
countrymap['AnyCountry'] = {
    'code': '',
    'name': 'Default',
}
scriptmap['AnyScript'] = {
    'code': '',
    'name': 'Default',
}

# language parsing
tree = ET.parse('common/main/en.xml')
root = tree.getroot()
for language in root.findall('./localeDisplayNames/languages/language'):
    languagetype = language.get('type')
    normallanguage = normalizestring(language.text)
    if normallanguage in ('Nauru', 'Tokelau', 'Tuvalu'):
        # country and language are the same, suffix to solve enum clashes
        normallanguage = '%sLanguage' % normallanguage
    languagemap[normallanguage] = {
        'code': languagetype,
        'name': language.text,
    }

if printenumsandexit:
    printenum(languagemap, 'Language')
elif printdocsandexit:
    printdoc(languagemap, 'Language')
else:
    printtable(languagemap, 'Language')

# country parsing
for country in root.findall('./localeDisplayNames/territories/territory'):
    countrytype = country.get('type')
    normalcountry = normalizestring(country.text)
    countrymap[normalcountry] = {
        'code': countrytype,
        'name': country.text,
    }

if printenumsandexit:
    printenum(countrymap, 'Country')
elif printdocsandexit:
    printdoc(countrymap, 'Country')
else:
    printtable(countrymap, 'Country')

# scripts parsing
for script in root.findall('./localeDisplayNames/scripts/script'):
    scripttype = script.get('type')
    normalscript = normalizestring(script.text)
    if not normalscript.endswith('Script'):
        # suffix script if needed
        normalscript = '%sScript' % normalscript
    if normalscript in ('UnknownScript', 'CommonScript'):
        # only interested in specific scripts
        continue
    scriptmap[normalscript] = {
        'code': scripttype,
        'name': script.text,
    }

if printenumsandexit:
    printenum(scriptmap, 'Script')
    sys.exit(0)
elif printdocsandexit:
    printdoc(scriptmap, 'Script')
    sys.exit(0)
else:
    printtable(scriptmap, 'Script')

# likely subtags parsing
# TODO: variant support
tree = ET.parse('common/supplemental/likelySubtags.xml')
root = tree.getroot()
for likelysubtag in root.findall('./likelySubtags/likelySubtag'):
    likelysubtagfrom = likelysubtag.get('from')
    likelysubtagto = likelysubtag.get('to')
    # split code into language, script and country to make it possible to match against regardless
    # of separators and remap to enums so that it is possible to substitute both named and enumed
    # locale searches at the cost of not covering all named cases
    likelysubtagfromsplit = localeregex.findall(likelysubtagfrom)
    likelysubtagfromsplitlen = len(likelysubtagfromsplit)
    likelysubtagtosplit = localeregex.findall(likelysubtagto)
    likelysubtagtosplitlen = len(likelysubtagtosplit)
    likelyfromlanguage = None
    likelyfromscript = None
    likelyfromcountry = None
    if likelysubtagfromsplitlen == 1:
        likelyfromlanguage = tolanguageenum(likelysubtagfromsplit[0])
        likelyfromscript = 'QLocale::Script::AnyScript'
        likelyfromcountry = 'QLocale::Country::AnyCountry'
    elif likelysubtagfromsplitlen == 2:
        likelyfromlanguage = tolanguageenum(likelysubtagfromsplit[0])
        likelyfromscript = 'QLocale::Script::AnyScript'
        likelyfromcountry = tocountryenum(likelysubtagfromsplit[1])
    elif likelysubtagfromsplitlen == 3:
        likelyfromlanguage = tolanguageenum(likelysubtagfromsplit[0])
        likelyfromscript = toscriptenum(likelysubtagfromsplit[1])
        likelyfromcountry = tocountryenum(likelysubtagfromsplit[2])
    elif likelysubtagfromsplitlen > 3 or likelysubtagtosplitlen > 3:
        # the regular expression is intentionally greedy, if there are more than 3 group matches
        # then it is likely a variant and that is not supported case yet
        print(likelysubtagfrom, likelysubtagfromsplit)
        print(likelysubtagto, likelysubtagtosplit)
        sys.exit(1)
    likelytolanguage = tolanguageenum(likelysubtagtosplit[0])
    likelytoscript = toscriptenum(likelysubtagtosplit[1])
    likelytocountry = tocountryenum(likelysubtagtosplit[2])

    if not likelyfromlanguage or not likelyfromscript or not likelyfromcountry \
        or not likelytolanguage or not likelytoscript or not likelytocountry:
        # if there are no enums for the codes skip the entry
        continue

    likelysubtagsmap[likelysubtagfrom] = {
        'fromlanguage' : likelyfromlanguage,
        'fromscript' : likelyfromscript,
        'fromcountry' : likelyfromcountry,
        'tolanguage' : likelytolanguage,
        'toscript' : likelytoscript,
        'tocountry' : likelytocountry,
    }
print('''static const struct subtagAliasTblData {
    const QLocale::Language fromlanguage;
    const QLocale::Script fromscript;
    const QLocale::Country fromcountry;
    const QLocale::Language tolanguage;
    const QLocale::Script toscript;
    const QLocale::Country tocountry;
} subtagAliasTbl[] = {''')

for key in sorted(likelysubtagsmap):
    value = likelysubtagsmap[key]
    print('''    {
        %s, %s, %s,
        %s, %s, %s
    },''' % (
            value['fromlanguage'],
            value['fromscript'],
            value['fromcountry'],
            value['tolanguage'],
            value['toscript'],
            value['tocountry'],
        )
    )

print('};')
print('static const qint16 subtagAliasTblSize = sizeof(subtagAliasTbl) / sizeof(subtagAliasTblData);\n')

# language alias parsing
tree = ET.parse('common/supplemental/supplementalMetadata.xml')
root = tree.getroot()
for languagealias in root.findall('./metadata/alias/languageAlias'):
    languagealiastype = languagealias.get('type')
    languagealiasreplacement = languagealias.get('replacement')
    if '_' in languagealiastype or '-' in languagealiastype \
        or '_' in languagealiasreplacement or '-' in languagealiasreplacement:
        # if either the original or the substitute is BCP47 code (language and script/country
        # included) skip it because QLocalePrivate::codeToLanguage() should be dealing with
        # language codes only
        continue
    languagealiasmap[languagealiastype] = languagealiasreplacement

printaliastable(languagealiasmap, 'language')

# country alias parsing
for territoryalias in root.findall('./metadata/alias/territoryAlias'):
    territoryaliastype = territoryalias.get('type')
    territoryaliasreplacement = territoryalias.get('replacement')
    countryaliasmap[territoryaliastype] = territoryaliasreplacement

printaliastable(countryaliasmap, 'country')

# script alias parsing
for scriptalias in root.findall('./metadata/alias/scriptAlias'):
    scriptaliastype = scriptalias.get('type')
    scriptaliasreplacement = scriptalias.get('replacement')
    scriptaliasmap[scriptaliastype] = scriptaliasreplacement

printaliastable(scriptaliasmap, 'script')
