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

for t in glob.glob('translations/qt*.ts'):
    command = ['minsize/bin/lupdate', '-locations', 'relative', '-no-ui-lines', '-no-obsolete']
    if not 'tools' in t:
        command.extend(tfiles)
    else:
        command.extend(cfiles)
    command.extend(['-ts', t])
    subprocess.check_call(command)
