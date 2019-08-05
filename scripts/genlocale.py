#!/usr/bin/python3
#-*- coding: UTF-8 -*-

# Data is from https://unicode.org/Public/cldr/35/core.zip

import os, sys, glob, re
import xml.etree.ElementTree as ET

def mapcopy(frommap, tomap):
    for key in frommap.keys():
        tomap[key] = frommap[key]

def listcopy(fromlist, tolist):
    for entry in fromlist:
        tolist.append(entry)

def stripxmltext(fromxmltext):
    result = fromxmltext.replace('\n', '')
    result = result.replace('\t', '')
    # 3-passes of double-space removal seems to be enough for all cases
    for p in range(3):
        result = result.replace('  ', ' ')
    return result.strip()

def touint(fromstring):
    # NOTE: symbols (plus, minus, etc.) are assumed to be single character which is not true for
    # many of the locales, however the API for those does not handle them as strings thus the first
    # character only is used
    return ord(fromstring)

def tochararray(fromstringlist):
    result = '{ '
    for string in fromstringlist:
        result = '%s"%s\\0", ' % (result, string)
    result = '%s }' % result
    result = result.replace(',  }', ' }')
    return result

def tochar(fromstring):
    if fromstring:
        return '"%s\\0"' % fromstring
    return 'Q_NULLPTR'

def toescapedchar(fromstring):
    if fromstring:
        return '"%s\\0"' % fromstring.replace('"', '\\"')
    return 'Q_NULLPTR'

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
        for r in range(20):
            fmt = fmt.replace('##', "#")
        fmt = fmt.replace('#', "%1")
        fmt = fmt.replace(u'\xa4', "%2")
        fmt = fmt.replace('-', frommap['minus'])
        fmt = fmt.replace('+', frommap['plus'])
        result.append(fmt)
    return result

def todatetimeformat(fromformat):
    unsupportedtags = [
        'g',
        'u',
        'q',
        'l',
        'w',
        'f',
        'g',
        'e',
        'c',
        'a',
        'k',
        'j',
        'v',
    ]
    possibleoccurences = [
        '%s, ',
        '%s.',
        '%s-',
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
    return result

def todayslist(fromxmlelements, initialvalues):
    result = []
    listcopy(initialvalues, result)
    for day in fromxmlelements:
        daytype = day.get('type')
        if daytype == 'sun':
            result[0] = day.text
        elif daytype == 'mon':
            result[1] = day.text
        elif daytype == 'tue':
            result[2] = day.text
        elif daytype == 'wed':
            result[3] = day.text
        elif daytype == 'thu':
            result[4] = day.text
        elif daytype == 'fri':
            result[5] = day.text
        elif daytype == 'sat':
            result[6] = day.text
    return result

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
    print('static const qint16 %sTblSize = sizeof(%sTbl) / sizeof(%sTblData);\n' % (lowerprefix, lowerprefix, lowerprefix))

def printlocaledata(frommap, key):
    value = frommap[key]
    print('''    {
        %s, %s, %s,
        // week
        %s, %s, %s,
        // symbols
        %s, %s, %s, %s, %s, %s, %s, %s, %s, %s,
        // quotation
        %s, %s, %s, %s,
        // endonym
        %s, %s,
        // list pattern
        %s, %s, %s, %s,
        // date/time format
        %s, %s, %s, %s,
        // am/pm
        %s, %s,
        // currency
        %s, %s, %s, %s,
        %s,
        // month names
        %s,
        %s,
        %s,
        %s,
        %s,
        %s,
        // day names
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
            value['currency_rounding'],
            touint(value['zero']),
            toescapedchar(value['quotation_start']),
            toescapedchar(value['quotation_end']),
            toescapedchar(value['alternate_quotation_start']),
            toescapedchar(value['alternate_quotation_end']),
            tochar(value['language_endonym']),
            tochar(value['country_endonym']),
            tochar(value['list_pattern_part_start']),
            tochar(value['list_pattern_part_mid']),
            tochar(value['list_pattern_part_end']),
            tochar(value['list_pattern_part_two']),
            tochar(todatetimeformat(value['short_date_format'])),
            tochar(todatetimeformat(value['long_date_format'])),
            tochar(value['short_time_format']),
            tochar(value['long_time_format']),
            tochar(value['am']),
            tochar(value['pm']),
            tochar(value['currency_symbol']),
            tochar(value['currency_format']),
            tochar(value['currency_negative_format']),
            tochar(value['currency_iso_code']),
            tochararray(value['currency_display_name']),
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

# main maps
languagemap = {}
countrymap = {}
scriptmap = {}
localemap = {}
# main lists
imperiallist = []
# cross-reference maps
localescriptmap = {}
localefirstdaymap = {}
localeweekendstartmap = {}
localeweekendendmap = {}
localeiso4217map = {}
localecurrencymap = {}
localenumericmap = {}

# artificial entries
languagemap['AnyLanguage'] = ['', 'Default']
languagemap['C'] = ['', 'C']
countrymap['AnyCountry'] = ['', 'Default']
scriptmap['AnyScript'] = ['', 'Default']

# locale to script parsing
tree = ET.parse('common/supplemental/supplementalData.xml')
root = tree.getroot()
for suppllanguage in root.findall('./languageData/language'):
    suppllanguagetype = suppllanguage.get('type')
    suppllanguagescripts = suppllanguage.get('scripts')
    localescriptmap[suppllanguagetype] = suppllanguagescripts

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
    latestcurrency = region.find('currency')
    lastestcurrencyiso4217 = latestcurrency.get('iso4217')
    localeiso4217map[regioniso3166] = lastestcurrencyiso4217

# locale to currency parsing
for info in root.findall('./currencyData/fractions/info'):
    infoiso4217 = info.get('iso4217')
    infodigits = info.get('digits')
    inforounding = info.get('rounding')
    localecurrencymap[infoiso4217] = [infodigits, inforounding]

# locale to numeric system parsing
tree = ET.parse('common/supplemental/numberingSystems.xml')
root = tree.getroot()
for numeric in root.findall('./numberingSystems/numberingSystem'):
    numericid = numeric.get('id')
    numericdigits = numeric.get('digits')
    if numericdigits:
        # either digits or rules is set
        localenumericmap[numericid] = stripxmltext(numericdigits)

# language parsing
tree = ET.parse('common/main/en.xml')
root = tree.getroot()
for language in root.findall('./localeDisplayNames/languages/language'):
    languagetype = language.get('type')
    normallanguage = normalizestring(language.text)
    if normallanguage in ('Nauru', 'Tokelau', 'Tuvalu'):
        # countries and language are the same, suffix to solve enum clashes
        normallanguage = '%sLanguage' % normallanguage
    languagemap[normallanguage] = [languagetype, language.text]

# printenum(languagemap, 'Language')
printtable(languagemap, 'Language')

# country parsing
tree = ET.parse('common/main/en.xml')
root = tree.getroot()
for country in root.findall('./localeDisplayNames/territories/territory'):
    countrytype = country.get('type')
    normalcountry = normalizestring(country.text)
    if normalcountry == 'World':
        # only interested in specific countries
        continue
    countrymap[normalcountry] = [countrytype, country.text]

# printenum(countrymap, 'Country')
printtable(countrymap, 'Country')

# scripts parsing
tree = ET.parse('common/main/en.xml')
root = tree.getroot()
for script in root.findall('./localeDisplayNames/scripts/script'):
    scripttype = script.get('type')
    normalscript = normalizestring(script.text)
    if not normalscript.endswith('Script'):
        # suffix script if needed
        normalscript = '%sScript' % normalscript
    if normalscript in ('UnknownScript', 'CommonScript'):
        # only interested in specific scripts
        continue
    scriptmap[normalscript] = [scripttype, script.text]

# printenum(scriptmap, 'Script')
printtable(scriptmap, 'Script')

# locales parsing
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
    'currency_rounding': '1', # not used, default in CLDR is 0
    # strings
    'quotation_start': '"',
    'quotation_end': '"',
    'alternate_quotation_start': '"',
    'alternate_quotation_end': '"',
    'language_endonym': '',
    'country_endonym': '',
    'list_pattern_part_start': "%1, %2",
    'list_pattern_part_mid': "%1, %2",
    'list_pattern_part_end': "%1, %2",
    'list_pattern_part_two': "%1, %2",
    'short_date_format': 'd MMM yyyy',
    'long_date_format': 'd MMMM yyyy',
    'short_time_format': 'HH:mm:ss',
    'long_time_format': 'HH:mm:ss z',
    'am': 'AM',
    'pm': 'PM',
    'currency_symbol': '',
    'currency_format': '%1%2',
    'currency_negative_format': '',
    'currency_iso_code': '',
    # arrays
    'currency_display_name': ['', '', '', '', '', '', ''], # only the first entry is used
    'standalone_short_month_names': ['Jan', 'Feb', 'Mar', 'Apr', 'May', 'Jun', 'Jul', 'Aug', 'Sep', 'Oct', 'Nov', 'Dec'],
    'standalone_long_month_names': ['January', 'February', 'March', 'April', 'May', 'June', 'July', 'August', 'September', 'October', 'November', 'December'],
    'standalone_narrow_month_names': ['J', 'F', 'M', 'A', 'M', 'J', 'J', 'A', 'S', 'O', 'N', 'D'],
    'short_month_names': ['Jan', 'Feb', 'Mar', 'Apr', 'May', 'Jun', 'Jul', 'Aug', 'Sep', 'Oct', 'Nov', 'Dec'],
    'long_month_names':  ['January', 'February', 'March', 'April', 'May', 'June', 'July', 'August', 'September', 'October', 'November', 'December'],
    'narrow_month_names': ['1', '2', '3', '4', '5', '6', '7', '8', '9', '10', '11', '12'],
    'standalone_short_day_names': ['Sun', 'Mon', 'Tue', 'Wed', 'Thu', 'Fri', 'Sat'],
    'standalone_long_day_names': ['Sunday', 'Monday', 'Tuesday', 'Wednesday', 'Thursday', 'Friday', 'Saturday'],
    'standalone_narrow_day_names': ['S', 'M', 'T', 'W', 'T', 'F', 'S'],
    'short_day_names': ['Sun', 'Mon', 'Tue', 'Wed', 'Thu', 'Fri', 'Sat'],
    'long_day_names': ['Sunday', 'Monday', 'Tuesday', 'Wednesday', 'Thursday', 'Friday', 'Saturday'],
    'narrow_day_names': ['7', '1', '2', '3', '4', '5', '6'],
}
# artificial entries
localemap['Default'] = {}
mapcopy(localedefaults, localemap['Default'])
localemap['C'] = {}
mapcopy(localedefaults, localemap['C'])
localemap['C']['language'] = 'QLocale::Language::C'

# TODO: accept only "contributed" or "approved" values
for xml in sorted(glob.glob('common/main/*.xml')):
    if xml.endswith('/root.xml'):
        # only interested in specific locales
        continue

    locale = os.path.basename(xml)
    locale = locale.replace('.xml', '')

    localemap[locale] = {}
    mapcopy(localemap['Default'], localemap[locale])

    tree = ET.parse(xml)
    root = tree.getroot()

    language = root.find('./identity/language')
    langtype = language.get('type')
    countrytype = None
    currencytype = None
    numbertype = 'latn' # CLDR default

    # set defaults from main locale if territory is specified
    country = root.find('./identity/territory')
    if country is not None:
        mapcopy(localemap[langtype], localemap[locale])

    # find the enums from mapped values
    for key in languagemap.keys():
        if langtype == languagemap[key][0]:
            localemap[locale]['language'] = 'QLocale::Language::%s' % key
            break

    if country is not None:
        countrytype = country.get('type')
        for key in countrymap.keys():
            if countrytype == countrymap[key][0]:
                localemap[locale]['country'] = 'QLocale::Country::%s' % key
                break
    else:
        # territory often is not specified, use language code as fallback
        countrytype = langtype.upper()

    # store for later, data is partial so pick from what is mapped
    if countrytype in localeiso4217map.keys():
        currencytype = localeiso4217map[countrytype]

    defaultnumbersystem = root.find('./numbers/defaultNumberingSystem')
    if defaultnumbersystem is not None:
        numbertype = defaultnumbersystem.text

    # find values from supplemental maps
    if langtype:
        for key in scriptmap.keys():
            if localescriptmap[langtype] == scriptmap[key][0]:
                localemap[locale]['script'] = 'QLocale::Script::%s' % key
                break

        for key in localefirstdaymap.keys():
            for countryvalue in localefirstdaymap[key]:
                if countryvalue == countrytype:
                    localemap[locale]['first_day_of_week'] = key
                    break

        for key in localeweekendstartmap.keys():
            for countryvalue in localeweekendstartmap[key]:
                if countryvalue == countrytype:
                    localemap[locale]['weekend_start'] = key
                    break

        for key in localeweekendendmap.keys():
            for countryvalue in localeweekendendmap[key]:
                if countryvalue == countrytype:
                    localemap[locale]['weekend_end'] = key
                    break

    # find from locale data
    for symbol in root.findall('./numbers/symbols'):
        symbolnumbersystem = symbol.get('numberSystem')
        if not symbolnumbersystem == numbertype:
            # should be the locale numeric system
            continue

        decimal = symbol.find('./decimal')
        if decimal is not None and len(decimal.text) == 1:
            localemap[locale]['decimal'] = decimal.text

        group = symbol.find('./group')
        if group is not None and len(group.text) == 1:
            localemap[locale]['group'] = group.text

        listdelimiter = symbol.find('./list')
        if listdelimiter is not None and len(listdelimiter.text) == 1:
            localemap[locale]['list'] = listdelimiter.text

        percent = symbol.find('./percentSign')
        if percent is not None and len(percent.text) == 1:
            localemap[locale]['percent'] = percent.text

        minus = symbol.find('./minusSign')
        if minus is not None and len(minus.text) == 1:
            localemap[locale]['minus'] = minus.text

        plus = symbol.find('./plusSign')
        if plus is not None and len(plus.text) == 1:
            localemap[locale]['plus'] = plus.text

        exponential = symbol.find('./exponential')
        if exponential is not None and len(exponential.text) == 1:
            localemap[locale]['exponential'] = exponential.text

        # zero is from cross-reference numeric system map,
        # taking the first character works even for UTF-8 chars
        localemap[locale]['zero'] = localenumericmap[numbertype][0]

    # digits/rounding data is specific so check if it is mapped
    if currencytype and currencytype in localecurrencymap.keys():
        localemap[locale]['currency_digits'] = localecurrencymap[currencytype][0]

        localemap[locale]['currency_rounding'] = localecurrencymap[currencytype][1]

    quotationstart = root.find('./delimiters/quotationStart')
    if quotationstart is not None:
        localemap[locale]['quotation_start'] = quotationstart.text

    quotationend = root.find('./delimiters/quotationEnd')
    if quotationend is not None:
        localemap[locale]['quotation_end'] = quotationend.text

    altquotationstart = root.find('./delimiters/alternateQuotationStart')
    if altquotationstart is not None:
        localemap[locale]['alternate_quotation_start'] = altquotationstart.text

    altquotationend = root.find('./delimiters/alternateQuotationEnd')
    if altquotationend is not None:
        localemap[locale]['alternate_quotation_end'] = altquotationend.text

    if langtype:
        for nativelang in root.findall('./localeDisplayNames/languages/language'):
            nativelangtype = nativelang.get('type')
            if nativelangtype == langtype:
                localemap[locale]['language_endonym'] = nativelang.text
                break

    if countrytype:
        for nativecountry in root.findall('./localeDisplayNames/territories/territory'):
            nativecountrytype = nativecountry.get('type')
            if nativecountrytype == countrytype:
                localemap[locale]['country_endonym'] = nativecountry.text
                break

    listpattern = root.find('./listPatterns/listPattern')
    if listpattern is not None:
        for listpatternpart in listpattern.findall('./listPatternPart'):
            listpatternparttype = listpatternpart.get('type')
            if listpatternparttype == 'start':
                localemap[locale]['list_pattern_part_start'] = tolistformat(listpatternpart.text)
            elif listpatternparttype == 'middle':
                localemap[locale]['list_pattern_part_mid'] = tolistformat(listpatternpart.text)
            elif listpatternparttype == 'end':
                localemap[locale]['list_pattern_part_end'] = tolistformat(listpatternpart.text)
            elif listpatternparttype == '2':
                localemap[locale]['list_pattern_part_two'] = tolistformat(listpatternpart.text)

    for calendar in root.findall('./dates/calendars/calendar'):
        calendartype = calendar.get('type')
        if not calendartype == 'gregorian':
            # all values should be from be from gregorian calendar
            continue
        dateformat = calendar.find('./dateFormats/dateFormatLength')
        if dateformat is not None:
            dateformattype = dateformat.get('type')
            if dateformattype == 'short':
                pattern = dateformat.find('./dateFormat/pattern')
                localemap[locale]['short_date_format'] = todatetimeformat(pattern.text)
            elif dateformattype == 'long':
                pattern = dateformat.find('./dateFormat/pattern')
                localemap[locale]['long_date_format'] = todatetimeformat(pattern.text)

        timeformat = calendar.find('./timeFormats/timeFormatLength')
        if timeformat is not None:
            timeformattype = timeformat.get('type')
            if timeformattype == 'short':
                pattern = timeformat.find('./timeFormat/pattern')
                localemap[locale]['short_time_format'] = todatetimeformat(pattern.text)
            elif timeformattype == 'long':
                pattern = timeformat.find('./timeFormat/pattern')
                localemap[locale]['long_time_format'] = todatetimeformat(pattern.text)

        for dayperiodwidth in calendar.findall('./dayPeriods/dayPeriodContext/dayPeriodWidth'):
            dayperiodwidthtype = dayperiodwidth.get('type')
            if not dayperiodwidthtype == 'wide':
                # only wide format is accepted
                continue
            for dayperiod in dayperiodwidth.findall('dayPeriod'):
                dayperiodtype = dayperiod.get('type')
                if dayperiodtype == 'am':
                    localemap[locale]['am'] = dayperiod.text
                elif dayperiodtype == 'pm':
                    localemap[locale]['pm'] = dayperiod.text

        # month/day names
        for monthcontext in calendar.findall('./months/monthContext'):
            monthcontexttype = monthcontext.get('type')
            if monthcontexttype == 'stand-alone':
                for monthwidth in monthcontext.findall('./monthWidth'):
                    monthwidthtype = monthwidth.get('type')
                    if monthwidthtype == 'wide':
                        months = monthwidth.findall('./month')
                        localemap[locale]['standalone_long_month_names'] = tomonthslist(months, localemap[locale]['standalone_long_month_names'])
                    elif monthwidthtype == 'abbreviated':
                        months = monthwidth.findall('./month')
                        localemap[locale]['standalone_short_month_names'] = tomonthslist(months, localemap[locale]['standalone_short_month_names'])
                    elif monthwidthtype == 'narrow':
                        months = monthwidth.findall('./month')
                        localemap[locale]['standalone_narrow_month_names'] = tomonthslist(months, localemap[locale]['standalone_narrow_month_names'])
            elif monthcontexttype == 'format':
                for monthwidth in monthcontext.findall('./monthWidth'):
                    monthwidthtype = monthwidth.get('type')
                    if monthwidthtype == 'wide':
                        months = monthwidth.findall('./month')
                        localemap[locale]['long_month_names'] = tomonthslist(months, localemap[locale]['long_month_names'])
                    elif monthwidthtype == 'abbreviated':
                        months = monthwidth.findall('./month')
                        localemap[locale]['short_month_names'] = tomonthslist(months, localemap[locale]['short_month_names'])
                    elif monthwidthtype == 'narrow':
                        months = monthwidth.findall('./month')
                        localemap[locale]['narrow_month_names'] = tomonthslist(months, localemap[locale]['narrow_month_names'])

        for daycontext in calendar.findall('./days/dayContext'):
            daycontexttype = daycontext.get('type')
            if daycontexttype == 'stand-alone':
                for daywidth in daycontext.findall('./dayWidth'):
                    daywidthtype = daywidth.get('type')
                    if daywidthtype == 'wide':
                        days = daywidth.findall('./day')
                        localemap[locale]['standalone_long_day_names'] = todayslist(days, localemap[locale]['standalone_long_day_names'])
                    elif daywidthtype == 'abbreviated':
                        days = daywidth.findall('./day')
                        localemap[locale]['standalone_short_day_names'] = todayslist(days, localemap[locale]['standalone_short_day_names'])
                    elif daywidthtype == 'narrow':
                        days = daywidth.findall('./day')
                        localemap[locale]['standalone_narrow_day_names'] = todayslist(days, localemap[locale]['standalone_narrow_day_names'])
            elif daycontexttype == 'format':
                for daywidth in daycontext.findall('./dayWidth'):
                    daywidthtype = daywidth.get('type')
                    if daywidthtype == 'wide':
                        days = daywidth.findall('./day')
                        localemap[locale]['long_day_names'] = todayslist(days, localemap[locale]['long_day_names'])
                    elif daywidthtype == 'abbreviated':
                        days = daywidth.findall('./day')
                        localemap[locale]['short_day_names'] = todayslist(days, localemap[locale]['short_day_names'])
                    elif daywidthtype == 'narrow':
                        days = daywidth.findall('./day')
                        localemap[locale]['narrow_day_names'] = todayslist(days, localemap[locale]['narrow_day_names'])

        # gregorian calendar was found, break
        break


    if currencytype:
        for elemcurrency in root.findall('./numbers/currencies/currency'):
            elemcurrencytype = elemcurrency.get('type')
            if elemcurrencytype == currencytype:
                symbol = elemcurrency.find('./symbol')
                if symbol is not None:
                    localemap[locale]['currency_symbol'] = symbol.text

                displaynamelist = []
                listcopy(localemap[locale]['currency_display_name'], displaynamelist)
                for displayname in elemcurrency.findall('./displayName'):
                    displaynamecount = displayname.get('count')
                    # TODO: 0 and 1 are aliases?
                    if not displaynamecount:
                        displaynamelist[0] = displayname.text
                    elif displaynamecount == 'zero':
                        displaynamelist[1] = displayname.text
                    elif displaynamecount == 'one':
                        displaynamelist[2] = displayname.text
                    elif displaynamecount == 'two':
                        displaynamelist[3] = displayname.text
                    elif displaynamecount == 'few':
                        displaynamelist[4] = displayname.text
                    elif displaynamecount == 'many':
                        displaynamelist[5] = displayname.text
                    elif displaynamecount == 'other':
                        displaynamelist[6] = displayname.text

                localemap[locale]['currency_display_name'] = displaynamelist
                # currency type was found, break
                break

        for currencyformat in root.findall('./numbers/currencyFormats'):
            currencyformatnumbersystem = currencyformat.get('numberSystem')
            if not currencyformatnumbersystem  == numbertype:
                # should be the locale numeric system
                continue
            nativecurrencyformat = currencyformat.find('currencyFormatLength/currencyFormat/pattern')
            if nativecurrencyformat is not None:
                formats = tocurrencyformat(nativecurrencyformat.text, localemap[locale])
                localemap[locale]['currency_format'] = formats[0]

                # negative format is optional
                if len(formats) > 1:
                    localemap[locale]['currency_negative_format'] = formats[1]

            localemap[locale]['currency_iso_code'] = currencytype

    # month/day names are set during calendar parsing

print('''static const QLocalePrivate localeTbl[] = {''')

# print Default first, C second
printlocaledata(localemap, 'Default')
printlocaledata(localemap, 'C')

# now everything except those
for key in sorted(localemap.keys()):
    if key in ('Default', 'C'):
        continue
    printlocaledata(localemap, key)

print('};')
print('static const qint16 localeTblSize = sizeof(localeTbl) / sizeof(QLocalePrivate);\n')

# imperial parsing
tree = ET.parse('common/supplemental/supplementalData.xml')
root = tree.getroot()
for measurementsystem in root.findall('./measurementData/measurementSystem'):
    measurementsystemtype = measurementsystem.get('type')
    if measurementsystemtype in ('UK', 'US'):
        territories = measurementsystem.get('territories')
        for territory in territories.split(' '):
            countryenum = None
            for key in countrymap.keys():
                countrycode = countrymap[key][0]
                if countrycode == territory:
                    countryenum = key
                    break
            imperiallist.append(countryenum)

print('''static const QLocale::Country imperialTbl[] = {''')

for string in sorted(imperiallist):
    print('    QLocale::Country::%s,' % string)

print('};')
print('static const qint16 imperialTblSize = sizeof(imperialTbl);')