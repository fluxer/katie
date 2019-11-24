#!/usr/bin/python

# Data is from https://www.iana.org/assignments/character-sets/character-sets.xml

import xml.etree.ElementTree as ET
from collections import OrderedDict

mibmap = OrderedDict()

tree = ET.parse('character-sets.xml')
registry = tree.findall('{http://www.iana.org/assignments}registry')[0]
for record in registry.getchildren():
    recordnames = []
    recordvalue = None

    for recordchild in record.getchildren():
        if recordchild.tag == '{http://www.iana.org/assignments}name':
            recordnames.append(recordchild.text)
        elif recordchild.tag == '{http://www.iana.org/assignments}value':
            recordvalue = recordchild.text
        elif recordchild.tag == '{http://www.iana.org/assignments}alias':
            recordnames.append(recordchild.text)

    mibmap[recordvalue] = recordnames

print('''static const struct MIBTblData {
    const int mib;
    const char* name;
} MIBTbl[] = {''')

for mib in mibmap:
    for name in mibmap[mib]:
        print('    { %s, "%s\\0" },' % (mib, name))

print('''};
static const qint16 MIBTblSize = sizeof(MIBTbl) / sizeof(MIBTblData);\n''')
