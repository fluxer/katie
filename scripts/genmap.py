#!/usr/bin/python

import os, re
from collections import OrderedDict

components = {
    'Core': 'Q_CORE_EXPORT',
    'Gui': 'Q_GUI_EXPORT',
    'DBus': 'Q_DBUS_EXPORT',
    'Xml': 'Q_XML_EXPORT',
    'Test': 'Q_TEST_EXPORT',
}
classesmap = OrderedDict()

def exportscan(sdir, pattern, component):
    regex = re.compile('(?:class|struct) (?:%s) (\w+)' % pattern)
    for sroot, sdir, lfiles in os.walk(sdir):
        for sfile in lfiles:
            if not sfile.endswith('.h') or sfile.endswith('_p.h'):
                continue
            sfull = '%s/%s' % (sroot, sfile)
            with open(sfull, 'rb') as f:
                scontent = f.read()
            scontent = scontent.decode('utf-8')
            for match in regex.findall(scontent):
                if match in classesmap:
                    continue
                classesmap[match] = '%s/%s' % (component, sfile)

for component in components:
    exportscan('src/%s' % component.lower(), components[component], 'Qt%s' % component)

print('''static const struct ClassTblData {
    const QLatin1String klass;
    const QLatin1String header;
} ClassTbl[] = {''')

for klass in sorted(classesmap.keys()):
    print('    { QLatin1String("%s"), QLatin1String("%s") },' % (klass, classesmap[klass]))


print('''};
static const qint16 ClassTblSize = sizeof(ClassTbl) / sizeof(ClassTblData);''')
