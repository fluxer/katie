#!/usr/bin/python3
#-*- coding: UTF-8 -*-

# See https://github.com/fluxer/katie/wiki/Unicode-support
# Data is from https://unicode.org/Public/cldr/40/core.zip

import os, sys, glob, re
import xml.etree.ElementTree as ET

printenumsandexit = ('--printenums' in sys.argv)
printdocsandexit = ('--printdocs' in sys.argv)

def mapcopy(frommap, tomap):
    for key in frommap.keys():
        tomap[key] = frommap[key]

def mapmerge(frommap, tomap, defaultmap):
    for key in frommap.keys():
        if frommap[key] == defaultmap[key]:
            continue
        tomap[key] = frommap[key]

def listcopy(fromlist, tolist):
    for entry in fromlist:
        tolist.append(entry)

def stripxmltext(fromxmltext):
    result = fromxmltext.replace('\n', '')
    result = result.replace('\t', '')
    # 3-passes of double-space removal seems to be enough for all cases
    for r in range(3):
        result = result.replace('  ', ' ')
    return result.strip()

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
    result = result.replace(u'ā', 'a')
    return result

def xmlmerge(fromxml, fromxml2):
    tree = ET.parse(fromxml)
    root = tree.getroot()
    tree2 = ET.parse(fromxml2)
    root2 = tree2.getroot()
    for element in root:
        root2.insert(0, element)
    return root2

def touint(fromstring):
    # NOTE: symbols (plus, minus, etc.) are assumed to be single character which is not true for
    # many of the locales, however the API for those does not handle them as strings thus the first
    # character only is used
    return ord(fromstring)

def tochar(fromstring):
    if fromstring:
        return '"%s\\0"' % fromstring
    return 'nullptr'

def tochararray(fromstringlist):
    result = '{ '
    for string in fromstringlist:
        result = '%s%s, ' % (result, tochar(string))
    result = '%s }' % result
    result = result.replace(',  }', ' }')
    return result

def todayenum(day):
    if day == 'mon':
        return 'Qt::Monday'
    elif day == 'tue':
        return 'Qt::Tuesday'
    elif day == 'wed':
        return 'Qt::Wednesday'
    elif day == 'thu':
        return 'Qt::Thursday'
    elif day == 'fri':
        return 'Qt::Friday'
    elif day == 'sat':
        return 'Qt::Saturday'
    elif day == 'sun':
        return 'Qt::Sunday'
    print('Unknown day: %s' % day)
    sys.exit(1)

def tolistformat(fromformat):
    result = fromformat.replace('{0}', '%1')
    result = result.replace('{1}', '%2')
    result = result.replace('{2}', '%3')
    return result

def tocurrencyformat(fromformat, frommap):
    result = []
    # currency format can optionally have negative form separated by ';'
    for fmt in fromformat.split(';'):
        fmt = fmt.replace('0', '#')
        fmt = fmt.replace(',', '#')
        fmt = fmt.replace('.', '#')
        for r in range(10):
            fmt = fmt.replace('##', "#")
        fmt = fmt.replace('#', "%1")
        fmt = fmt.replace(u'\xa4', "%2")
        fmt = fmt.replace('-', frommap['minus'])
        fmt = fmt.replace('+', frommap['plus'])
        result.append(fmt)
    return result

def todatetimeformat(fromformat):
    # valid are y, m, M, d, h, H, s, a, A, z and t
    unsupportedtags = [
        'g',
        'u',
        'q',
        'l',
        'w',
        'f',
        'j',
    ]
    replacementtags = {
        'MMMMM' : 'MMM', # narrow month name
        'LLLLL' : 'MMM', # stand-alone narrow month name
        'E' : 'ddd', 'EE' : 'ddd', 'EEE' : 'ddd', 'EEEEE' : 'ddd', 'EEEE' : 'dddd', # day of week
        'e' : 'ddd', 'ee' : 'ddd', 'eee' : 'ddd', 'eeeee' : 'ddd', 'eeee' : 'dddd', # local day of week
        'c' : 'ddd', 'cc' : 'ddd', 'ccc' : 'ddd', 'ccccc' : 'ddd', 'cccc' : 'dddd', # stand-alone local day of week
        'K' : 'h', # Hour 0-11
        'k' : 'H', # Hour 1-24
        'z' : 'Z', 'zz' : 'Z', 'zzz' : 'Z', 'zzzz' : 'Z', # timezone
        'Z' : 'Z', 'ZZ' : 'Z', 'ZZZ' : 'Z', 'ZZZZ' : 'Z', # timezone
        'v' : 'Z', 'vv' : 'Z', 'vvv' : 'Z', 'vvvv' : 'Z', # timezone
        'V' : 'Z', 'VV' : 'Z', 'VVV' : 'Z', 'VVVV' : 'Z',  # timezone
        'L' : 'M', # stand-alone month names. not supported
    }
    replacementregex = {
        r'y' : 'yyyy',       # four-digit year without leading zeroes
        r'yyy{3,}' : 'yyyy', # more that three digits hence convert to four-digit year
        r'S{1,}' : '',       # fractional seconds. not supported.
        r'A{1,}' : '',       # milliseconds in day. not supported.
        r'a' : 'AP',         # AM/PM
    }
    possibleoccurences = [
        '%s, ',
        ', %s',
        '%s.',
        '.%s',
        '%s-',
        '-%s',
        '(%s)',
        "('%s')",
        '%s ',
        ' %s',
        '%s',
    ]
    result = fromformat
    for tag in unsupportedtags:
        uppertag = tag.upper()
        for occurence in possibleoccurences:
            result = result.replace(occurence % (tag * 4), '')
            result = result.replace(occurence % (tag * 3), '')
            result = result.replace(occurence % (tag * 2), '')
            result = result.replace(occurence % tag, '')
            result = result.replace(occurence % (uppertag * 4), '')
            result = result.replace(occurence % (uppertag * 3), '')
            result = result.replace(occurence % (uppertag * 2), '')
            result = result.replace(occurence % uppertag, '')
    for key in replacementregex.keys():
        result = re.sub(key, replacementregex[key], result)
    for key in replacementtags.keys():
        result = result.replace(key, replacementtags[key])
    return result

def tomonthslist(fromxmlelements, initialvalues):
    result = []
    listcopy(initialvalues, result)
    for month in fromxmlelements:
        monthtype = month.get('type')
        if monthtype == '1':
            result[0] = month.text
        elif monthtype == '2':
            result[1] = month.text
        elif monthtype == '3':
            result[2] = month.text
        elif monthtype == '4':
            result[3] = month.text
        elif monthtype == '5':
            result[4] = month.text
        elif monthtype == '6':
            result[5] = month.text
        elif monthtype == '7':
            result[6] = month.text
        elif monthtype == '8':
            result[7] = month.text
        elif monthtype == '9':
            result[8] = month.text
        elif monthtype == '10':
            result[9] = month.text
        elif monthtype == '11':
            result[10] = month.text
        elif monthtype == '12':
            result[11] = month.text
        else:
            print('Unknown month: %s' % monthtype)
            sys.exit(1)
    return result

def todayslist(fromxmlelements, initialvalues):
    result = []
    listcopy(initialvalues, result)
    for day in fromxmlelements:
        daytype = day.get('type')
        if daytype == 'mon':
            result[0] = day.text
        elif daytype == 'tue':
            result[1] = day.text
        elif daytype == 'wed':
            result[2] = day.text
        elif daytype == 'thu':
            result[3] = day.text
        elif daytype == 'fri':
            result[4] = day.text
        elif daytype == 'sat':
            result[5] = day.text
        elif daytype == 'sun':
            result[6] = day.text
        else:
            print('Unknown day: %s' % daytype)
            sys.exit(1)
    return result

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

def printlocaledata(frommap, key):
    value = frommap[key]
    # skip table entries without country (non-territory), unless it is artificial, this is done to
    # preserve the assumption in QLocalePrivate::findLocale that "AnyCountry" means "find me a
    # language, no matter what country it is spoken in" if "AnyCountry" is passed to it as argument
    # and also shrinks the table
    if value['country'] == 'QLocale::Country::AnyCountry' and not key == 'C':
        return
    print('''    {
        %s, %s, %s,
        %s, %s, %s,
        %s, %s, %s, %s, %s, %s, %s, %s, %s,
        %s, %s, %s, %s,
        %s, %s,
        %s, %s, %s, %s,
        %s, %s, %s, %s,
        %s, %s,
        %s, %s, %s, %s, %s,
        %s,
        %s,
        %s,
        %s,
        %s,
        %s,
        %s,
        %s,
        %s,
        %s,
        %s,
        %s
    }, // %s''' % (
            value['language'],
            value['script'],
            value['country'],
            value['first_day_of_week'],
            value['weekend_start'],
            value['weekend_end'],
            touint(value['decimal']),
            touint(value['group']),
            touint(value['list']),
            touint(value['percent']),
            touint(value['minus']),
            touint(value['plus']),
            touint(value['exponential']),
            value['currency_digits'],
            touint(value['zero']),
            touint(value['quotation_start']),
            touint(value['quotation_end']),
            touint(value['alternate_quotation_start']),
            touint(value['alternate_quotation_end']),
            tochar(value['language_endonym']),
            tochar(value['country_endonym']),
            tochar(value['list_pattern_part_start']),
            tochar(value['list_pattern_part_mid']),
            tochar(value['list_pattern_part_end']),
            tochar(value['list_pattern_part_two']),
            tochar(value['short_date_format']),
            tochar(value['long_date_format']),
            tochar(value['short_time_format']),
            tochar(value['long_time_format']),
            tochar(value['am']),
            tochar(value['pm']),
            tochar(value['currency_symbol']),
            tochar(value['currency_format']),
            tochar(value['currency_negative_format']),
            tochar(value['currency_iso_code']),
            tochar(value['currency_display_name']),
            tochararray(value['standalone_short_month_names']),
            tochararray(value['standalone_long_month_names']),
            tochararray(value['standalone_narrow_month_names']),
            tochararray(value['short_month_names']),
            tochararray(value['long_month_names']),
            tochararray(value['narrow_month_names']),
            tochararray(value['standalone_short_day_names']),
            tochararray(value['standalone_long_day_names']),
            tochararray(value['standalone_narrow_day_names']),
            tochararray(value['short_day_names']),
            tochararray(value['long_day_names']),
            tochararray(value['narrow_day_names']),
            key,
        )
    )

def printaliastable(frommap, prefix):
    print('''static const struct %sAliasTblData {
    const char* original;
    const char* substitute;
} %sAliasTbl[] = {''' % (prefix, prefix))

    for key in sorted(frommap):
        # territories and scripts entries can contain multiple replacements, add one for each
        splitvalue = frommap[key].split(' ')
        for value in splitvalue:
            print('    { "%s", "%s" },' % (key, value))

    print('};')
    print('static const qint16 %sAliasTblSize = sizeof(%sAliasTbl) / sizeof(%sAliasTblData);\n' % (prefix, prefix, prefix))

# main maps
languagemap = {}
countrymap = {}
scriptmap = {}
localemap = {}
likelysubtagsmap = {}
languagealiasmap = {}
countryaliasmap = {}
scriptaliasmap = {}
# cross-reference maps
localeparentmap = {}
localeparentvaluesmap = {}
localescriptmap = {}
localefirstdaymap = {}
localeweekendstartmap = {}
localeweekendendmap = {}
localeiso4217map = {}
localecurrencymap = {}
localenumberingmap = {}
# regular expressions
localeregex = re.compile('([^_|\-|\.|@]+)+')

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

# locale to parent parsing
tree = ET.parse('common/supplemental/supplementalData.xml')
root = tree.getroot()
for parentlocale in root.findall('./parentLocales/parentLocale'):
    parentlocaleparent = parentlocale.get('parent')
    parentlocalelocales = parentlocale.get('locales')
    localeparentmap[parentlocaleparent] = parentlocalelocales.split(' ')

# locale to script parsing
# only languages with one primary script are mapped because if there are multiple it should be
# specified in the locale data, see:
# https://sites.google.com/site/cldr/development/updating-codes/update-language-script-info/language-script-description
# secondary scripts are not taken into account at all
for suppllanguage in root.findall('./languageData/language'):
    suppllanguagetype = suppllanguage.get('type')
    suppllanguagescripts = suppllanguage.get('scripts')
    suppllanguagealt = suppllanguage.get('alt')
    if not suppllanguagescripts or suppllanguagealt == 'secondary':
        # alternative entry, skip it
        continue
    suppllanguagescriptslist = suppllanguagescripts.split(' ')
    if not len(suppllanguagescriptslist) == 1:
        # skip entries without definitive primary script
        continue
    suppllanguageterritories = suppllanguage.get('territories')
    if not suppllanguageterritories:
        # territories is optional, if not specified use artifical value to map all languages of
        # that type to the script
        suppllanguageterritories = 'AnyTerritory'
    localescriptmap[suppllanguagetype] = {
        'script': suppllanguagescripts,
        'territories': suppllanguageterritories.split(' '),
    }

# locale to first day parsing
for firstday in root.findall('./weekData/firstDay'):
    firstdayday = firstday.get('day')
    firstdayterritories = firstday.get('territories')
    localefirstdaymap[todayenum(firstdayday)] = stripxmltext(firstdayterritories).split(' ')

# locale to weekend start parsing
for weekstart in root.findall('./weekData/weekendStart'):
    weekstartday = weekstart.get('day')
    weekstartterritories = weekstart.get('territories')
    localeweekendstartmap[todayenum(weekstartday)] = stripxmltext(weekstartterritories).split(' ')

# locale to weekend end parsing
for weekend in root.findall('./weekData/weekendEnd'):
    weekendday = weekend.get('day')
    weekendterritories = weekend.get('territories')
    localeweekendendmap[todayenum(weekendday)] = stripxmltext(weekendterritories).split(' ')

# locale to iso4217 parsing
for region in root.findall('./currencyData/region'):
    regioniso3166 = region.get('iso3166')
    # data includes past currencies too, pick the current currency which is first
    currency = region.find('currency')
    currencyiso4217 = currency.get('iso4217')
    localeiso4217map[regioniso3166] = currencyiso4217

# locale to currency parsing
for info in root.findall('./currencyData/fractions/info'):
    infoiso4217 = info.get('iso4217')
    infodigits = info.get('digits')
    localecurrencymap[infoiso4217] = infodigits

# locale to numbering system parsing
tree = ET.parse('common/supplemental/numberingSystems.xml')
root = tree.getroot()
for numberingsystem  in root.findall('./numberingSystems/numberingSystem'):
    numberingsystemid = numberingsystem.get('id')
    numberingsystemdigits = numberingsystem.get('digits')
    if numberingsystemdigits:
        # either digits or rules is set
        localenumberingmap[numberingsystemid] = stripxmltext(numberingsystemdigits)

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

# these defaults are used as parent locales fallback, C uses them as actual values because root
# contains UTF-8 characters and for compatibility. for the rest defaults are set from root
localedefaults = {
    # enums
    'language': 'QLocale::Language::AnyLanguage',
    'script': 'QLocale::Script::AnyScript',
    'country': 'QLocale::Country::AnyCountry',
    'first_day_of_week': 'Qt::Monday',
    'weekend_start': 'Qt::Saturday',
    'weekend_end': 'Qt::Sunday',
    # characters
    'decimal': '.',
    'group': ',',
    'list': ';',
    'percent': '%',
    'zero': '0',
    'minus': '-',
    'plus': '+',
    'exponential': 'e', # default in CLDR is E
    'currency_digits': '2',
    'quotation_start': '"', # default in CLDR is “
    'quotation_end': '"', # default in CLDR is ”
    'alternate_quotation_start': "'", # default in CLDR is ‘
    'alternate_quotation_end': "'", # default in CLDR is ’
    # strings
    'language_endonym': '',
    'country_endonym': '',
    'list_pattern_part_start': "%1, %2",
    'list_pattern_part_mid': "%1, %2",
    'list_pattern_part_end': "%1, %2",
    'list_pattern_part_two': "%1, %2",
    'short_date_format': 'd MMM yyyy', # default in CLDR is y-MM-dd
    'long_date_format': 'd MMMM yyyy',
    'short_time_format': 'HH:mm:ss', # default in CLDR is HH:mm
    'long_time_format': 'HH:mm:ss z',
    'am': 'AM',
    'pm': 'PM',
    'currency_symbol': '',
    'currency_format': '%1%2',
    'currency_negative_format': '',
    'currency_iso_code': '',
    'currency_display_name': '',
    # arrays
    'standalone_short_month_names': ['Jan', 'Feb', 'Mar', 'Apr', 'May', 'Jun', 'Jul', 'Aug', 'Sep', 'Oct', 'Nov', 'Dec'],
    'standalone_long_month_names': ['January', 'February', 'March', 'April', 'May', 'June', 'July', 'August', 'September', 'October', 'November', 'December'],
    'standalone_narrow_month_names': ['J', 'F', 'M', 'A', 'M', 'J', 'J', 'A', 'S', 'O', 'N', 'D'],
    'short_month_names': ['Jan', 'Feb', 'Mar', 'Apr', 'May', 'Jun', 'Jul', 'Aug', 'Sep', 'Oct', 'Nov', 'Dec'],
    'long_month_names':  ['January', 'February', 'March', 'April', 'May', 'June', 'July', 'August', 'September', 'October', 'November', 'December'],
    'narrow_month_names': ['1', '2', '3', '4', '5', '6', '7', '8', '9', '10', '11', '12'],
    'standalone_short_day_names': ['Mon', 'Tue', 'Wed', 'Thu', 'Fri', 'Sat', 'Sun'],
    'standalone_long_day_names': ['Monday', 'Tuesday', 'Wednesday', 'Thursday', 'Friday', 'Saturday', 'Sunday'],
    'standalone_narrow_day_names': ['M', 'T', 'W', 'T', 'F', 'S', 'S'],
    'short_day_names': ['Mon', 'Tue', 'Wed', 'Thu', 'Fri', 'Sat', 'Sun'],
    'long_day_names': ['Monday', 'Tuesday', 'Wednesday', 'Thursday', 'Friday', 'Saturday', 'Sunday'],
    'narrow_day_names': ['1', '2', '3', '4', '5', '6', '7'],
}
# artificial entries
localemap['C'] = {}
mapcopy(localedefaults, localemap['C'])
localemap['C']['language'] = 'QLocale::Language::C'

# locales parsing
# TODO: accept only "contributed" or "approved" values
def readlocale(fromxml, tomap, isparent):
    locale = os.path.basename(fromxml)
    locale = locale.replace('.xml', '')

    if '_' in fromxml:
        # merge parent locales (non-territory) into the current one so that data can be read
        # from them. they do not have territory and currency data is based on territory so
        # cross-reference is not possible without doing so
        localeparent = locale.split('_')[0]
        xmlparent = fromxml.replace(locale, localeparent)
        root = xmlmerge(fromxml, xmlparent)
    else:
        tree = ET.parse(fromxml)
        root = tree.getroot()

    variant = root.find('./identity/variant')
    if variant is not None:
        # TODO: variants are not supported by QLocale
        return

    language = root.find('./identity/language')
    langtype = language.get('type')
    country = root.find('./identity/territory')
    countrytype = None
    currencytype = None
    scripttype = None
    numbertype = 'latn' # CLDR default

    tomap[locale] = {}
    if isparent:
        mapcopy(localedefaults, tomap[locale])
    else:
        mapcopy(localeparentvaluesmap['root'], tomap[locale])

    # set defaults from parent locale if territory is specified
    if country is not None:
        for parent in localeparentmap.keys():
            if locale in localeparentmap[parent]:
                mapcopy(localeparentvaluesmap[parent], tomap[locale])
    # then from main locale (non-territory) filling the blanks that even parent locales do not fill
    if not isparent:
        mapmerge(localemap[langtype], tomap[locale], localedefaults)

    # find the enums from mapped values
    tomap[locale]['language'] = tolanguageenum(langtype)

    if not isparent and country is not None:
        countrytype = country.get('type')
        tomap[locale]['country'] = tocountryenum(countrytype)
    else:
        # territory often is not specified, use language code as fallback
        countrytype = langtype.upper()

    # script is specified either in the locale or supplemental data
    script = root.find('./identity/script')
    if script is not None:
        scripttype = script.get('type')
    elif not isparent:
        # scripts map is partial, pick from what is mapped
        if langtype in localescriptmap.keys():
            scriptterritories = localescriptmap[langtype]['territories']
            if 'AnyTerritory' in scriptterritories \
                or countrytype in scriptterritories:
                scripttype = localescriptmap[langtype]['script']

    # store for later
    if countrytype in localeiso4217map.keys():
        currencytype = localeiso4217map[countrytype]

    defaultnumbersystem = root.find('./numbers/defaultNumberingSystem')
    if defaultnumbersystem is not None:
        numbertype = defaultnumbersystem.text

    # find values from supplemental maps
    if not isparent and scripttype:
        tomap[locale]['script'] = toscriptenum(scripttype)

    for key in localefirstdaymap.keys():
        for countryvalue in localefirstdaymap[key]:
            if countryvalue == countrytype:
                tomap[locale]['first_day_of_week'] = key
                break

    for key in localeweekendstartmap.keys():
        for countryvalue in localeweekendstartmap[key]:
            if countryvalue == countrytype:
                tomap[locale]['weekend_start'] = key
                break

    for key in localeweekendendmap.keys():
        for countryvalue in localeweekendendmap[key]:
            if countryvalue == countrytype:
                tomap[locale]['weekend_end'] = key
                break

    # find from locale data
    for symbol in root.findall('./numbers/symbols'):
        symbolnumbersystem = symbol.get('numberSystem')
        if not symbolnumbersystem == numbertype:
            # should be the locale numeric system
            continue

        decimal = symbol.find('./decimal')
        if decimal is not None and len(decimal.text) == 1:
            tomap[locale]['decimal'] = decimal.text

        group = symbol.find('./group')
        if group is not None and len(group.text) == 1:
            tomap[locale]['group'] = group.text

        listdelimiter = symbol.find('./list')
        if listdelimiter is not None and len(listdelimiter.text) == 1:
            tomap[locale]['list'] = listdelimiter.text

        percent = symbol.find('./percentSign')
        if percent is not None and len(percent.text) == 1:
            tomap[locale]['percent'] = percent.text

        minus = symbol.find('./minusSign')
        if minus is not None and len(minus.text) == 1:
            tomap[locale]['minus'] = minus.text

        plus = symbol.find('./plusSign')
        if plus is not None and len(plus.text) == 1:
            tomap[locale]['plus'] = plus.text

        exponential = symbol.find('./exponential')
        if exponential is not None and len(exponential.text) == 1:
            tomap[locale]['exponential'] = exponential.text

        # zero is from cross-reference numeric system map,
        # taking the first character works even for UTF-8 chars
        tomap[locale]['zero'] = localenumberingmap[numbertype][0]

        # locale numeric system was found, break
        break

    # digits/rounding data is specific so check if it is mapped
    if currencytype and currencytype in localecurrencymap.keys():
        tomap[locale]['currency_digits'] = localecurrencymap[currencytype]

    quotationstart = root.find('./delimiters/quotationStart')
    if quotationstart is not None:
        tomap[locale]['quotation_start'] = quotationstart.text

    quotationend = root.find('./delimiters/quotationEnd')
    if quotationend is not None:
        tomap[locale]['quotation_end'] = quotationend.text

    altquotationstart = root.find('./delimiters/alternateQuotationStart')
    if altquotationstart is not None:
        tomap[locale]['alternate_quotation_start'] = altquotationstart.text

    altquotationend = root.find('./delimiters/alternateQuotationEnd')
    if altquotationend is not None:
        tomap[locale]['alternate_quotation_end'] = altquotationend.text

    for nativelang in root.findall('./localeDisplayNames/languages/language'):
        nativelangtype = nativelang.get('type')
        if nativelangtype == langtype:
            tomap[locale]['language_endonym'] = nativelang.text
            break

    if countrytype:
        for nativecountry in root.findall('./localeDisplayNames/territories/territory'):
            nativecountrytype = nativecountry.get('type')
            if nativecountrytype == countrytype:
                tomap[locale]['country_endonym'] = nativecountry.text
                break

    listpattern = root.find('./listPatterns/listPattern')
    if listpattern is not None:
        for listpatternpart in listpattern.findall('./listPatternPart'):
            listpatternparttype = listpatternpart.get('type')
            if listpatternparttype == 'start':
                tomap[locale]['list_pattern_part_start'] = tolistformat(listpatternpart.text)
            elif listpatternparttype == 'middle':
                tomap[locale]['list_pattern_part_mid'] = tolistformat(listpatternpart.text)
            elif listpatternparttype == 'end':
                tomap[locale]['list_pattern_part_end'] = tolistformat(listpatternpart.text)
            elif listpatternparttype == '2':
                tomap[locale]['list_pattern_part_two'] = tolistformat(listpatternpart.text)

    for calendar in root.findall('./dates/calendars/calendar'):
        calendartype = calendar.get('type')
        if not calendartype == 'gregorian':
            # all values should be from gregorian calendar
            continue
        for dateformat in calendar.findall('./dateFormats/dateFormatLength'):
            dateformattype = dateformat.get('type')
            if dateformattype == 'short':
                pattern = dateformat.find('./dateFormat/pattern')
                tomap[locale]['short_date_format'] = todatetimeformat(pattern.text)
            elif dateformattype == 'long':
                pattern = dateformat.find('./dateFormat/pattern')
                tomap[locale]['long_date_format'] = todatetimeformat(pattern.text)

        for timeformat in calendar.findall('./timeFormats/timeFormatLength'):
            timeformattype = timeformat.get('type')
            if timeformattype == 'short':
                pattern = timeformat.find('./timeFormat/pattern')
                tomap[locale]['short_time_format'] = todatetimeformat(pattern.text)
            elif timeformattype == 'long':
                pattern = timeformat.find('./timeFormat/pattern')
                tomap[locale]['long_time_format'] = todatetimeformat(pattern.text)

        for dayperiodwidth in calendar.findall('./dayPeriods/dayPeriodContext/dayPeriodWidth'):
            dayperiodwidthtype = dayperiodwidth.get('type')
            if not dayperiodwidthtype == 'wide':
                # all values should be in wide format
                continue
            for dayperiod in dayperiodwidth.findall('dayPeriod'):
                dayperiodtype = dayperiod.get('type')
                if dayperiodtype == 'am':
                    tomap[locale]['am'] = dayperiod.text
                elif dayperiodtype == 'pm':
                    tomap[locale]['pm'] = dayperiod.text

        # month/day names
        for monthcontext in calendar.findall('./months/monthContext'):
            monthcontexttype = monthcontext.get('type')
            if monthcontexttype == 'stand-alone':
                for monthwidth in monthcontext.findall('./monthWidth'):
                    monthwidthtype = monthwidth.get('type')
                    if monthwidthtype == 'wide':
                        months = monthwidth.findall('./month')
                        tomap[locale]['standalone_long_month_names'] = tomonthslist(months, tomap[locale]['standalone_long_month_names'])
                    elif monthwidthtype == 'abbreviated':
                        months = monthwidth.findall('./month')
                        tomap[locale]['standalone_short_month_names'] = tomonthslist(months, tomap[locale]['standalone_short_month_names'])
                    elif monthwidthtype == 'narrow':
                        months = monthwidth.findall('./month')
                        tomap[locale]['standalone_narrow_month_names'] = tomonthslist(months, tomap[locale]['standalone_narrow_month_names'])
            elif monthcontexttype == 'format':
                for monthwidth in monthcontext.findall('./monthWidth'):
                    monthwidthtype = monthwidth.get('type')
                    if monthwidthtype == 'wide':
                        months = monthwidth.findall('./month')
                        tomap[locale]['long_month_names'] = tomonthslist(months, tomap[locale]['long_month_names'])
                    elif monthwidthtype == 'abbreviated':
                        months = monthwidth.findall('./month')
                        tomap[locale]['short_month_names'] = tomonthslist(months, tomap[locale]['short_month_names'])
                    elif monthwidthtype == 'narrow':
                        months = monthwidth.findall('./month')
                        tomap[locale]['narrow_month_names'] = tomonthslist(months, tomap[locale]['narrow_month_names'])

        for daycontext in calendar.findall('./days/dayContext'):
            daycontexttype = daycontext.get('type')
            if daycontexttype == 'stand-alone':
                for daywidth in daycontext.findall('./dayWidth'):
                    daywidthtype = daywidth.get('type')
                    if daywidthtype == 'wide':
                        days = daywidth.findall('./day')
                        tomap[locale]['standalone_long_day_names'] = todayslist(days, tomap[locale]['standalone_long_day_names'])
                    elif daywidthtype == 'abbreviated':
                        days = daywidth.findall('./day')
                        tomap[locale]['standalone_short_day_names'] = todayslist(days, tomap[locale]['standalone_short_day_names'])
                    elif daywidthtype == 'narrow':
                        days = daywidth.findall('./day')
                        tomap[locale]['standalone_narrow_day_names'] = todayslist(days, tomap[locale]['standalone_narrow_day_names'])
            elif daycontexttype == 'format':
                for daywidth in daycontext.findall('./dayWidth'):
                    daywidthtype = daywidth.get('type')
                    if daywidthtype == 'wide':
                        days = daywidth.findall('./day')
                        tomap[locale]['long_day_names'] = todayslist(days, tomap[locale]['long_day_names'])
                    elif daywidthtype == 'abbreviated':
                        days = daywidth.findall('./day')
                        tomap[locale]['short_day_names'] = todayslist(days, tomap[locale]['short_day_names'])
                    elif daywidthtype == 'narrow':
                        days = daywidth.findall('./day')
                        tomap[locale]['narrow_day_names'] = todayslist(days, tomap[locale]['narrow_day_names'])

        # gregorian calendar was found, break
        break


    if currencytype:
        for elemcurrency in root.findall('./numbers/currencies/currency'):
            elemcurrencytype = elemcurrency.get('type')
            if elemcurrencytype == currencytype:
                symbol = elemcurrency.find('./symbol')
                if symbol is not None:
                    tomap[locale]['currency_symbol'] = symbol.text

                for displayname in elemcurrency.findall('./displayName'):
                    displaynamecount = displayname.get('count')
                    if not displaynamecount:
                        # only interested in default display name
                        tomap[locale]['currency_display_name'] = displayname.text
                        break

                # currency type was found, break
                break

        for currencyformat in root.findall('./numbers/currencyFormats'):
            currencyformatnumbersystem = currencyformat.get('numberSystem')
            if not currencyformatnumbersystem  == numbertype:
                # should be the locale numeric system
                continue
            nativecurrencyformat = currencyformat.find('currencyFormatLength/currencyFormat/pattern')
            if nativecurrencyformat is not None:
                formats = tocurrencyformat(nativecurrencyformat.text, tomap[locale])
                tomap[locale]['currency_format'] = formats[0]

                # negative format is optional
                if len(formats) > 1:
                    tomap[locale]['currency_negative_format'] = formats[1]

            # currency format was found, break
            break

        tomap[locale]['currency_iso_code'] = currencytype

    # month/day names are set during calendar parsing

# read parent locales first
for xml in glob.glob('common/main/*.xml'):
    xmlbase = os.path.basename(xml)
    xmlbase = xmlbase.replace('.xml', '')
    if not xmlbase in localeparentmap.keys():
        continue
    readlocale(xml, localeparentvaluesmap, True)

# now everything including those
for xml in sorted(glob.glob('common/main/*.xml')):
    if xml.endswith('/root.xml'):
        # root is not actual locale
        continue
    readlocale(xml, localemap, False)

print('''static const QLocalePrivate localeTbl[] = {''')

# print C first
printlocaledata(localemap, 'C')

# now everything except that
for key in sorted(localemap.keys()):
    if key == 'C':
        continue
    printlocaledata(localemap, key)

print('};')
print('static const qint16 localeTblSize = sizeof(localeTbl) / sizeof(QLocalePrivate);\n')

# likely subtags parsing
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
