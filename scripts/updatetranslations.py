#!/usr/bin/python

import os, glob, subprocess

components = ('core', 'dbus', 'declarative', 'gui', 'network', 'plugins',
    'script', 'scripttools', 'sql', 'svg', 'test', 'uitools', 'xml')
cfiles = []
tfiles = []

def list_files(sdir):
    lfiles = []
    for root, subdirs, files in os.walk(sdir):
        for sfile in files:
            if sfile.endswith(('.cpp', '.h', '.js', '.qs', '.qml', '.ui')):
                lfiles.append('%s/%s' % (root, sfile))
    return lfiles

for c in components:
    cfiles.extend(list_files('src/%s' % c))

for t in glob.glob('src/tools/*'):
    tfiles.extend(list_files(t))

qtcommand = ['xgettext', '--from-code=UTF-8', '-o', 'translations/qt.pot',
    '-kQT_TR_NOOP:1', '-kQT_TR_NOOP_UTF8:1',
    '-kQT_TRANSLATE_NOOP:1c,2','-kQT_TRANSLATE_NOOP_UTF8:1c,2',
    '-ktranslate:1c,2', '-ktr:1', '-ktrUtf8:1']
qtcommand.extend(cfiles)
subprocess.check_call(qtcommand)

qttoolscommand = ['xgettext', '--from-code=UTF-8', '-o', 'translations/qt_tools.pot',
    '-kQT_TR_NOOP:1', '-kQT_TR_NOOP_UTF8:1',
    '-kQT_TRANSLATE_NOOP:1c,2','-kQT_TRANSLATE_NOOP_UTF8:1c,2',
    '-ktranslate:1c,2', '-ktr:1', '-ktrUtf8:1']
qttoolscommand.extend(tfiles)
subprocess.check_call(qttoolscommand)

for t in glob.glob('translations/*.po'):
    potfile = 'translations/qt.pot'
    if 'qt_tools' in t:
        potfile = 'translations/qt_tools.pot'
    subprocess.check_call(['msgmerge', '--verbose', '--update', t, potfile])
    subprocess.check_call(['msgattrib', '--no-fuzzy', '--no-obsolete', t, '-o', t])
