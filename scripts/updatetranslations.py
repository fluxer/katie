#!/usr/bin/python

import os, glob, subprocess, shlex

components = ['core', 'declarative', 'gui', 'multimedia', 'network', 'opengl', 'plugins', 'script', 'scripttools', 'sql', 'svg', 'xml', 'xmlpatterns']
tools = ['lrelease', 'uic', 'moc', 'lupdate', 'idc', 'qcollectiongenerator', 'rcc', 'lconvert', 'qhelpgenerator', 'qdbus']
cfiles = []
tfiles = []

def list_files(sdir):
    slist = []
    for root, subdirs, files in os.walk(sdir):
        for sfile in files:
            slist.append('%s/%s' % (root, sfile))
    return slist

for c in components:
    for f in list_files('src/%s' % c):
        if f.endswith(('.cpp', '.h', '.js')):
            cfiles.append(f)

for t in tools:
    for f in list_files('src/%s' % c):
        if f.endswith(('.cpp', '.js')):
            tfiles.append(f)

for t in glob.glob('translations/qt*.ts'):
    if 'tools' in t:
        command = 'build/bin/lupdate -locations relative -no-ui-lines -no-obsolete %s -ts %s' % (' '.join(tfiles), t)
        command = shlex.split(command)
        pipe = subprocess.Popen(command, stderr=subprocess.PIPE)
        pipe.wait()
        if pipe.returncode != 0:
            raise(Exception(pipe.communicate()[1].strip()))
    else:
        command = 'build/bin/lupdate -locations relative -no-ui-lines -no-obsolete %s -ts %s' % (' '.join(cfiles), t)
        command = shlex.split(command)
        pipe = subprocess.Popen(command, stderr=subprocess.PIPE)
        pipe.wait()
        if pipe.returncode != 0:
            raise(Exception(pipe.communicate()[1].strip()))
