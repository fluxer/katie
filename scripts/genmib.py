#!/usr/bin/python

# See https://github.com/fluxer/katie/wiki/Unicode-support
# Data is from https://www.iana.org/assignments/character-sets/character-sets.xml

import xml.etree.ElementTree as ET
from collections import OrderedDict

mibmap = OrderedDict()

def fixalias(alias):
    # if alias contains space remove everything after it, spaces are not
    # allowed in encoding names, example of such case is Amiga-1251 alias
    alias = alias.replace('\n', ' ')
    return alias.split(' ')[0]

tree = ET.parse('character-sets.xml')
registry = tree.find('{http://www.iana.org/assignments}registry')
for record in registry:
    recordnames = []
    recordvalue = None

    for recordchild in record:
        if recordchild.tag == '{http://www.iana.org/assignments}name':
            recordnames.append(recordchild.text)
        elif recordchild.tag == '{http://www.iana.org/assignments}value':
            recordvalue = recordchild.text
        elif recordchild.tag == '{http://www.iana.org/assignments}alias':
            alias = recordchild.text
            if alias in recordnames:
                # alias to self, e.g. US-ASCII
                continue
            recordnames.append(fixalias(alias))

    mibmap[recordvalue] = recordnames

print('''static const struct MIBTblData {
    const qint16 mib;
    const char* name;
} MIBTbl[] = {''')

for mib in mibmap:
    for name in mibmap[mib]:
        print('    { %s, "%s\\0" },' % (mib, name))

print('''};
static const qint16 MIBTblSize = sizeof(MIBTbl) / sizeof(MIBTblData);\n''')
