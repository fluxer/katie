#!/usr/bin/python

import os, glob, subprocess, shlex

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
    if 'tools' in t:
        files = ' '.join(tfiles)
    else:
        files = ' '.join(cfiles)
    command = 'minsize/bin/lupdate -locations relative -no-ui-lines -no-obsolete %s -ts %s' % (files, t)
    command = shlex.split(command)
    pipe = subprocess.Popen(command, stderr=subprocess.PIPE)
    pipe.wait()
    if pipe.returncode != 0:
        raise(Exception(pipe.communicate()[1].strip()))
