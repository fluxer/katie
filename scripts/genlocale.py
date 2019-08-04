#!/usr/bin/python3
#-*- coding: UTF-8 -*-

# Data is from https://unicode.org/Public/cldr/35/core.zip

import os, sys
import xml.etree.ElementTree as ET
import glob

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

def stringtoushort(fromstring):
    # NOTE: symbols (plus, minus, etc.) are assumed to be single character which is not true for
    # many of the locales, however the API for those does not handle them as strings thus the first
    # character only is used
    return ord(fromstring)

def listtochar(stringlist):
    result = '{ '
    for string in stringlist:
        result = '%s"%s\\0", ' % (result, string)
    result = '%s }' % result
    result = result.replace(',  }', ' }')
    return result

def stringtochar(string):
    if string:
        return '"%s\\0"' % string
    return 'Q_NULLPTR'

def stringtoescapedchar(string):
    if string:
        return '"%s\\0"' % string.replace('"', '\\"')
    return 'Q_NULLPTR'

def daytoenum(day):
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

def tomothslist(fromxmlelements, initialvalues):
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
    for key in sorted(frommap.keys()):
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
            stringtoushort(value['decimal']),
            stringtoushort(value['group']),
            stringtoushort(value['list']),
            stringtoushort(value['percent']),
            stringtoushort(value['zero']),
            stringtoushort(value['minus']),
            stringtoushort(value['plus']),
            stringtoushort(value['exponential']),
            value['currency_digits'],
            value['currency_rounding'],
            stringtoescapedchar(value['quotation_start']),
            stringtoescapedchar(value['quotation_end']),
            stringtoescapedchar(value['alternate_quotation_start']),
            stringtoescapedchar(value['alternate_quotation_end']),
            stringtochar(value['language_endonym']),
            stringtochar(value['country_endonym']),
            stringtochar(value['list_pattern_part_start']),
            stringtochar(value['list_pattern_part_mid']),
            stringtochar(value['list_pattern_part_end']),
            stringtochar(value['list_pattern_part_two']),
            stringtochar(value['short_date_format']),
            stringtochar(value['long_date_format']),
            stringtochar(value['short_time_format']),
            stringtochar(value['long_time_format']),
            stringtochar(value['am']),
            stringtochar(value['pm']),
            stringtochar(value['currency_symbol']),
            stringtochar(value['currency_format']),
            stringtochar(value['currency_negative_format']),
            stringtochar(value['currency_iso_code']),
            listtochar(value['currency_display_name']),
            listtochar(value['standalone_short_month_names']),
            listtochar(value['standalone_long_month_names']),
            listtochar(value['standalone_narrow_month_names']),
            listtochar(value['short_month_names']),
            listtochar(value['long_month_names']),
            listtochar(value['narrow_month_names']),
            listtochar(value['standalone_short_day_names']),
            listtochar(value['standalone_long_day_names']),
            listtochar(value['standalone_narrow_day_names']),
            listtochar(value['short_day_names']),
            listtochar(value['long_day_names']),
            listtochar(value['narrow_day_names']),
            key,
        )
    )

# main maps
languagemap = {}
countrymap = {}
scriptmap = {}
localemap = {}
# cross-reference maps
localescriptmap = {}
localefirstdaymap = {}
localeweekstartmap = {}
localeweekendmap = {}
localeiso4217map = {}
localecurrencymap = {}
localenumericmap = {}

# artificial values
languagemap['AnyLanguage'] = ['', 'Default']
languagemap['C'] = ['', 'C']
countrymap['AnyCountry'] = ['', 'Default']
scriptmap['AnyScript'] = ['', 'Default']

# language to script parsing
tree = ET.parse('common/supplemental/supplementalData.xml')
root = tree.getroot()
for supllanguage in root.findall('./languageData/language'):
    supllanguagetype = supllanguage.get('type')
    supllanguagescripts = supllanguage.get('scripts')
    localescriptmap[supllanguagetype] = supllanguagescripts

# language to first day parsing
for firstday in root.findall('./weekData/firstDay'):
    firstdayday = firstday.get('day')
    firstdayterritories = firstday.get('territories')
    localefirstdaymap[daytoenum(firstdayday)] = stripxmltext(firstdayterritories).split(' ')

# language to week start parsing
for weekstart in root.findall('./weekData/weekendStart'):
    weekstartday = weekstart.get('day')
    weekstartterritories = weekstart.get('territories')
    localeweekstartmap[daytoenum(weekstartday)] = stripxmltext(weekstartterritories).split(' ')

# language to week end parsing
for weekend in root.findall('./weekData/weekendStart'):
    weekendday = weekend.get('day')
    weekendterritories = weekend.get('territories')
    localeweekendmap[daytoenum(weekendday)] = stripxmltext(weekendterritories).split(' ')

# language to iso4217 parsing
for region in root.findall('./currencyData/region'):
    regioniso3166 = region.get('iso3166')
    latestcurrency = region.find('currency')
    lastestcurrencyiso4217 = latestcurrency.get('iso4217')
    localeiso4217map[regioniso3166] = lastestcurrencyiso4217

# language to currency parsing
for info in root.findall('./currencyData/fractions/info'):
    infoiso4217 = info.get('iso4217')
    infodigits = info.get('digits')
    inforounding = info.get('rounding')
    localecurrencymap[infoiso4217] = [infodigits, inforounding]

# language to numeric system parsing
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
    'long_date_format': 'EEEE, d MMMM yyyy',
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
localemap['Default'] = {}
mapcopy(localedefaults, localemap['Default'])
localemap['C'] = {}
mapcopy(localedefaults, localemap['C'])
localemap['C']['language'] = 'QLocale::Language::C'

# TODO: accept only "contributed" or "approved" values
for xml in glob.glob('common/main/*.xml'):
    if xml.endswith('/root.xml'):
        # only interested in specific languages
        continue

    lang = os.path.basename(xml)
    lang = lang.replace('.xml', '')

    localemap[lang] = {}
    mapcopy(localemap['Default'], localemap[lang])

    tree = ET.parse(xml)
    root = tree.getroot()

    # find the enums from mapped values
    language = root.find('./identity/language')
    langtype = None
    countrytype = None
    currencytype = None
    numertype = 'latn' # CLDR default

    # atleast language is required
    langtype = language.get('type')
    for key in languagemap.keys():
        if langtype == languagemap[key][0]:
            localemap[lang]['language'] = 'QLocale::Language::%s' % key
            break

    country = root.find('./identity/territory')
    if country is not None:
        countrytype = country.get('type')
        for key in countrymap.keys():
            if countrytype == countrymap[key][0]:
                localemap[lang]['country'] = 'QLocale::Country::%s' % key
                break
    else:
        # territory often is not specified, use language code as fallback
        countrytype = langtype.upper()

    # store for later reuse, data is partial so pick from what is mapped
    if countrytype in localeiso4217map.keys():
        currencytype = localeiso4217map[countrytype]

    defaultnumbersystem = root.find('./numbers/defaultNumberingSystem')
    if defaultnumbersystem is not None:
        numertype = defaultnumbersystem.text

    # find values from suplemental maps
    if langtype:
        for key in scriptmap.keys():
            if localescriptmap[langtype] == scriptmap[key][0]:
                localemap[lang]['script'] = 'QLocale::Script::%s' % key
                break

        for key in localefirstdaymap.keys():
            for countryvalue in localefirstdaymap[key]:
                if countryvalue == countrytype:
                    localemap[lang]['first_day_of_week'] = key
                    break

        for key in localeweekstartmap.keys():
            for countryvalue in localeweekstartmap[key]:
                if countryvalue == countrytype:
                    localemap[lang]['weekend_start'] = key
                    break

        for key in localeweekendmap.keys():
            for countryvalue in localeweekendmap[key]:
                if countryvalue == countrytype:
                    localemap[lang]['weekend_end'] = key
                    break

    # find from language data
    for symbol in root.findall('./numbers/symbols'):
        symbolnumbersystem = symbol.get('numberSystem')
        if not symbolnumbersystem == numertype:
            # should be the language numeric system
            continue

        decimal = symbol.find('./decimal')
        if decimal is not None and len(decimal.text) == 1:
            localemap[lang]['decimal'] = decimal.text

        group = symbol.find('./group')
        if group is not None and len(group.text) == 1:
            localemap[lang]['group'] = group.text

        listdelimiter = symbol.find('./list')
        if listdelimiter is not None and len(listdelimiter.text) == 1:
            localemap[lang]['list'] = listdelimiter.text

        percent = symbol.find('./percentSign')
        if percent is not None and len(percent.text) == 1:
            localemap[lang]['percent'] = percent.text

        # zero is from cross-reference numeric system map,
        # taking the first character works even for UTF-8 chars
        localemap[lang]['zero'] = localenumericmap[numertype][0]

        minus = symbol.find('./minusSign')
        if minus is not None and len(minus.text) == 1:
            localemap[lang]['minus'] = minus.text

        plus = symbol.find('./plusSign')
        if plus is not None and len(plus.text) == 1:
            localemap[lang]['plus'] = plus.text

        exponential = symbol.find('./exponential')
        if exponential is not None and len(exponential.text) == 1:
            localemap[lang]['exponential'] = exponential.text

    # digits/rounding data is specific so check if it is mapped
    if currencytype and currencytype in localecurrencymap.keys():
        localemap[lang]['currency_digits'] = localecurrencymap[currencytype][0]

        localemap[lang]['currency_rounding'] = localecurrencymap[currencytype][1]

    quotationstart = root.find('./delimiters/quotationStart')
    if quotationstart is not None:
        localemap[lang]['quotation_start'] = quotationstart.text

    quotationend = root.find('./delimiters/quotationEnd')
    if quotationend is not None:
        localemap[lang]['quotation_end'] = quotationend.text

    altquotationstart = root.find('./delimiters/alternateQuotationStart')
    if altquotationstart is not None:
        localemap[lang]['alternate_quotation_start'] = altquotationstart.text

    altquotationend = root.find('./delimiters/alternateQuotationEnd')
    if altquotationend is not None:
        localemap[lang]['alternate_quotation_end'] = altquotationend.text

    if langtype:
        for nativelang in root.findall('./localeDisplayNames/languages/language'):
            nativelangtype = nativelang.get('type')
            if nativelangtype == langtype:
                localemap[lang]['language_endonym'] = nativelang.text
                break

    if countrytype:
        for nativecountry in root.findall('./localeDisplayNames/territories/territory'):
            nativecountrytype = nativecountry.get('type')
            if nativecountrytype == countrytype:
                localemap[lang]['country_endonym'] = nativecountry.text
                break

    listpattern = root.find('./listPatterns/listPattern')
    if listpattern is not None:
        for listpatternpart in listpattern.findall('./listPatternPart'):
            listpatternparttype = listpatternpart.get('type')
            if listpatternparttype == 'start':
                localemap[lang]['list_pattern_part_start'] = tolistformat(listpatternpart.text)
            elif listpatternparttype == 'middle':
                localemap[lang]['list_pattern_part_mid'] = tolistformat(listpatternpart.text)
            elif listpatternparttype == 'end':
                localemap[lang]['list_pattern_part_end'] = tolistformat(listpatternpart.text)
            elif listpatternparttype == '2':
                localemap[lang]['list_pattern_part_two'] = tolistformat(listpatternpart.text)

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
                localemap[lang]['short_date_format'] = pattern.text
            elif dateformattype == 'long':
                pattern = dateformat.find('./dateFormat/pattern')
                localemap[lang]['long_date_format'] = pattern.text

        timeformat = calendar.find('./timeFormats/timeFormatLength')
        if timeformat is not None:
            timeformattype = timeformat.get('type')
            if timeformattype == 'short':
                pattern = timeformat.find('./timeFormat/pattern')
                localemap[lang]['short_time_format'] = pattern.text
            elif timeformattype == 'long':
                pattern = timeformat.find('./timeFormat/pattern')
                localemap[lang]['long_time_format'] = pattern.text

        for dayperiodwidth in calendar.findall('./dayPeriods/dayPeriodContext/dayPeriodWidth'):
            dayperiodwidthtype = dayperiodwidth.get('type')
            if not dayperiodwidthtype == 'wide':
                # only wide format is accepted
                continue
            for dayperiod in dayperiodwidth.findall('dayPeriod'):
                dayperiodtype = dayperiod.get('type')
                if dayperiodtype == 'am':
                    localemap[lang]['am'] = dayperiod.text
                elif dayperiodtype == 'pm':
                    localemap[lang]['pm'] = dayperiod.text

        # month/day names
        for monthcontext in calendar.findall('./months/monthContext'):
            monthcontexttype = monthcontext.get('type')
            if monthcontexttype == 'stand-alone':
                for monthwidth in monthcontext.findall('./monthWidth'):
                    monthwidthtype = monthwidth.get('type')
                    if monthwidthtype == 'wide':
                        months = monthwidth.findall('./month')
                        localemap[lang]['standalone_long_month_names'] = tomothslist(months, localemap[lang]['standalone_long_month_names'])
                    elif monthwidthtype == 'abbreviated':
                        months = monthwidth.findall('./month')
                        localemap[lang]['standalone_short_month_names'] = tomothslist(months, localemap[lang]['standalone_short_month_names'])
                    elif monthwidthtype == 'narrow':
                        months = monthwidth.findall('./month')
                        localemap[lang]['standalone_narrow_month_names'] = tomothslist(months, localemap[lang]['standalone_narrow_month_names'])
            elif monthcontexttype == 'format':
                for monthwidth in monthcontext.findall('./monthWidth'):
                    monthwidthtype = monthwidth.get('type')
                    if monthwidthtype == 'wide':
                        months = monthwidth.findall('./month')
                        localemap[lang]['long_month_names'] = tomothslist(months, localemap[lang]['long_month_names'])
                    elif monthwidthtype == 'abbreviated':
                        months = monthwidth.findall('./month')
                        localemap[lang]['short_month_names'] = tomothslist(months, localemap[lang]['short_month_names'])
                    elif monthwidthtype == 'narrow':
                        months = monthwidth.findall('./month')
                        localemap[lang]['narrow_month_names'] = tomothslist(months, localemap[lang]['narrow_month_names'])

        for daycontext in calendar.findall('./days/dayContext'):
            daycontexttype = daycontext.get('type')
            if daycontexttype == 'stand-alone':
                for daywidth in daycontext.findall('./dayWidth'):
                    daywidthtype = daywidth.get('type')
                    if daywidthtype == 'wide':
                        days = daywidth.findall('./day')
                        localemap[lang]['standalone_long_day_names'] = todayslist(days, localemap[lang]['standalone_long_day_names'])
                    elif daywidthtype == 'abbreviated':
                        days = daywidth.findall('./day')
                        localemap[lang]['standalone_short_day_names'] = todayslist(days, localemap[lang]['standalone_short_day_names'])
                    elif daywidthtype == 'narrow':
                        days = daywidth.findall('./day')
                        localemap[lang]['standalone_narrow_day_names'] = todayslist(days, localemap[lang]['standalone_narrow_day_names'])
            elif daycontexttype == 'format':
                for daywidth in daycontext.findall('./dayWidth'):
                    daywidthtype = daywidth.get('type')
                    if daywidthtype == 'wide':
                        days = daywidth.findall('./day')
                        localemap[lang]['long_month_names'] = todayslist(days, localemap[lang]['long_month_names'])
                    elif daywidthtype == 'abbreviated':
                        days = daywidth.findall('./day')
                        localemap[lang]['short_day_names'] = todayslist(days, localemap[lang]['short_day_names'])
                    elif daywidthtype == 'narrow':
                        days = daywidth.findall('./day')
                        localemap[lang]['narrow_day_names'] = todayslist(days, localemap[lang]['narrow_day_names'])

        # gregorian calendar was found, break
        break


    if currencytype:
        for elemcurrency in root.findall('./numbers/currencies/currency'):
            elemcurrencytype = elemcurrency.get('type')
            if elemcurrencytype == currencytype:
                symbol = elemcurrency.find('./symbol')
                if symbol is not None:
                    localemap[lang]['currency_symbol'] = symbol.text

                displaynamelist = ['', '', '', '', '', '', '']
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

                localemap[lang]['currency_display_name'] = displaynamelist
                # currency type was found, break
                break

        for currencyformat in root.findall('./numbers/currencyFormats'):
            currencyformatnumbersystem = currencyformat.get('numberSystem')
            if not currencyformatnumbersystem  == numertype:
                # should be the language numeric system
                continue
            nativecurrencyformat = currencyformat.find('currencyFormatLength/currencyFormat/pattern')
            if nativecurrencyformat is not None:
                formats = tocurrencyformat(nativecurrencyformat.text, localemap[lang])
                localemap[lang]['currency_format'] = formats[0]

                if len(formats) > 1:
                    localemap[lang]['currency_negative_format'] = formats[1]

            localemap[lang]['currency_iso_code'] = currencytype

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
print('static const qint16 localeTblSize = sizeof(localeTbl) / sizeof(QLocalePrivate);')
