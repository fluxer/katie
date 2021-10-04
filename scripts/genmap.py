#!/usr/bin/python

import os, re
from collections import OrderedDict

components = {
    'Core': 'Q_CORE_EXPORT',
    'Gui': 'Q_GUI_EXPORT',
    'DBus': 'Q_DBUS_EXPORT',
    'Declarative': 'Q_DECLARATIVE_EXPORT',
    'Designer': 'Q_DESIGNER_EXPORT|QDESIGNER_COMPONENTS_EXPORT|QT_FORMEDITOR_EXPORT|QT_PROPERTYEDITOR_EXPORT|QT_SIGNALSLOTEDITOR_EXPORT|QT_OBJECTINSPECTOR_EXPORT|QT_WIDGETBOX_EXPORT|QT_BUDDYEDITOR_EXPORT|QT_TABORDEREDITOR_EXPORT|QT_TASKMENU_EXPORT',
    'Network': 'Q_NETWORK_EXPORT',
    'Svg': 'Q_SVG_EXPORT',
    'Xml': 'Q_XML_EXPORT',
    'Script': 'Q_SCRIPT_EXPORT',
    'ScriptTools': 'Q_SCRIPTTOOLS_EXPORT',
    'Test': 'Q_TEST_EXPORT',
    'UiTools': 'Q_UITOOLS_EXPORT',
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
