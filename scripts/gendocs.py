#!/usr/bin/python

# https://www.doxygen.nl/manual/commands.html

import sys, os, shutil, subprocess

cwd = os.path.dirname(__file__)

doxygen = None
for path in os.environ.get('PATH', '/bin:/usr/bin').split(':'):
    sfile = '%s/doxygen' % path
    if os.access(sfile, os.X_OK):
        doxygen = sfile
        break

if not doxygen:
    sys.stderr.write('Doxygen is not installed\n')
    sys.exit(1)

sys.stdout.write('Generating API docs for Katie...\n')
apidir = '%s/../apidocs' % cwd
if os.path.isdir(apidir):
    shutil.rmtree(apidir)
subprocess.check_call((doxygen, '%s/../doxygen.conf' % cwd))

