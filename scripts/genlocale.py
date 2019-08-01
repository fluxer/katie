#!/usr/bin/python

import os, sys
import xml.etree.ElementTree as ET

# TODO: conflicting languages: Nauru, Tokelau and Tuvalu

def stripstring(xmlstring):
    xmlstring = xmlstring.replace('\n', '')
    xmlstring = xmlstring.replace('\t', '')
    return xmlstring.strip()

def compactstring(xmlstring):
    # TODO: xmlstring = xmlstring.replace("\xe2", '')
    xmlstring = xmlstring.replace(' ', '')
    xmlstring = xmlstring.replace('-', '')
    xmlstring = xmlstring.replace('&', 'And')
    xmlstring = xmlstring.replace('(', '')
    xmlstring = xmlstring.replace(')', '')
    xmlstring = xmlstring.replace('St.', 'St')
    xmlstring = xmlstring.replace('U.S.', 'UnitedStates')
    return xmlstring

if 'language' in sys.argv:
    print('''static const struct languageTblData {
    const char* name;
    const char* code;
    const QLocale::Language language;
} languageTbl[] = {''')

    print('    { "Default\\0", "  \\0", QLocale::Language::AnyLanguage },')
    print('    { "C\\0", "  \\0", QLocale::Language::C },')

    tree = ET.parse('common/main/en.xml')
    root = tree.getroot()
    languagecount = 2
    for language in root.findall('./localeDisplayNames/languages/language'):
        languagetype = language.get('type')
        compactlanguage = compactstring(language.text)
        # print('        %s = %d,' % (compactlanguage, languagecount))
        if len(languagetype) == 2:
            print('    { "%s\\0", "%s\\0", QLocale::%s },' % (language.text, languagetype, compactlanguage))
        else:
            print('    { "%s\\0", "%s", QLocale::%s },' % (language.text, languagetype, compactlanguage))
        languagecount += 1

    print('};')
    print('static const qint16 languageTblSize = sizeof(languageTbl) / sizeof(languageTblData);')

elif 'country' in sys.argv:
    print('''static const struct countryTblData {
    const char* name;
    const char* code;
    const QLocale::Country country;
} countryTbl[] = {''')

    print('    { "Default\\0", "  \\0", QLocale::Country::AnyCountry },')

    tree = ET.parse('common/main/en.xml')
    root = tree.getroot()
    countrycount = 1
    for country in root.findall('./localeDisplayNames/territories/territory'):
        countrytype = country.get('type')
        compactcountry = compactstring(country.text)
        if compactcountry == 'World':
            continue
        # print('        %s = %d,' % (compactcountry, countrycount))
        print('    { "%s\\0", "%s\\0", QLocale::%s },' % (country.text, countrytype, compactcountry))
        countrycount += 1

    print('};')
    print('static const qint16 countryTblSize = sizeof(countryTbl) / sizeof(countryTblData);')

elif 'script' in sys.argv:
    print('''static const struct scriptsTblData {
    const char* name;
    const char* code;
    const QLocale::Script script;
} scriptsTbl[] = {''')

    print('    { "Default\\0", "\\0\\0\\0\\0", QLocale::Script::AnyScript },')

    tree = ET.parse('common/main/en.xml')
    root = tree.getroot()
    scriptcount = 1
    for script in root.findall('./localeDisplayNames/scripts/script'):
        scripttype = script.get('type')
        compactscript = compactstring(script.text)
        if not compactscript.endswith('Script'):
            compactscript = '%sScript' % compactscript
        # print('        %s = %d,' % (compactscript, scriptcount))
        print('    { "%s\\0", "%s", QLocale::%s },' % (script.text, scripttype, compactscript))
        scriptcount += 1

    print('};')
    print('static const qint16 scriptsTblSize = sizeof(scriptsTbl) / sizeof(scriptsTblData);')

else:
    print('''usage: <language|country|script>

Data is from https://unicode.org/Public/cldr/35/core.zip''')
    sys.exit(1)
