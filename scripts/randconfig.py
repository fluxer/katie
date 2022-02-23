#!/usr/bin/python

import random

configfile = "src/core/global/qconfig.h.cmake"

with open(configfile, 'r') as f:
    configcontent = f.read()

isconfig = False
configlines = []
for sline in configcontent.splitlines():
    if sline == '// Misc':
        isconfig = True
        continue
    elif isconfig and not sline:
        isconfig = False
    if isconfig:
        configlines.append(sline)

toreplace = configlines[random.randrange(0, len(configlines))]
replacement = toreplace.replace('#cmakedefine', '#define')
configcontent = configcontent.replace(toreplace, replacement)

with open(configfile, 'w') as f:
    f.write(configcontent)
    
