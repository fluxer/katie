#!/usr/bin/python2

import sys, os, re

headersdir = os.getcwd()
component = sys.argv[1]
keyword = sys.argv[2]

mapoutput = '%s/include/%s_map.h' % (headersdir, component)
mapdata = '#ifndef %s_MAP_H\n#define %s_MAP_H\n\n' % (component, component)

def exportscan(sdir):
    dirmap = ''
    for sroot, sdir, lfiles in os.walk(sdir):
        for sfile in lfiles:
            if not sfile.endswith('.h'):
                continue
            sfull = '%s/%s' % (sroot, sfile)
            with open(sfull, 'rb') as f:
                scontent = f.read()
            scontent = scontent.decode('utf-8')
            for match in re.findall('class (?:%s) (\w+)' % keyword, scontent):
                dirmap += 'QT_CLASS_LIB(%s, %s, %s)\n' % (match, component, sfile)
    return dirmap

mapdata += exportscan('%s/include/%s' % (headersdir, component))
mapdata += exportscan('%s/privateinclude/%s' % (headersdir, component))
mapdata += '\n#endif\n'

sys.stderr.write('-- Writing: %s\n' % os.path.basename(mapoutput))
with open(mapoutput, 'wb') as f:
    if sys.version_info[0] == 3:
        f.write(bytes(mapdata, 'utf-8'))
    else:
        f.write(mapdata)