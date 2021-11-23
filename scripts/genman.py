#!/usr/bin/python

import sys, os, subprocess

cwd = os.path.dirname(__file__)

pod2man = None
for path in os.environ.get('PATH', '/bin:/usr/bin').split(':'):
    sfile = '%s/pod2man' % path
    if os.access(sfile, os.X_OK):
        pod2man = sfile
        break

if not pod2man:
    sys.stderr.write('Perl is not installed\n')
    sys.exit(1)

sys.stdout.write('Generating man pages...\n')
for root, sdir, lfiles in os.walk('%s/../src/tools' % cwd):
    for sfile in lfiles:
        if sfile.endswith('.pod'):
            ifile = '%s/%s' % (root, sfile)
            ofile = ifile.replace('.pod', '.1')
            subprocess.check_call(
                (pod2man,
                '--release=Katie 4.11.1',
                '--center=Katie Manual',
                '--section=1',
                '--utf8',
                ifile,
                ofile)
            )
