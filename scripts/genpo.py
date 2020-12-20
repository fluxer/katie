#!/usr/bin/python

from googletrans import Translator
import sys

if len(sys.argv) < 3:
    print('usage: <POT> <language>')
    sys.exit(1)

potfile = sys.argv[1]
language = sys.argv[2]

with open(potfile, 'r') as f:
    content = f.read()

translator = Translator()
newcontent = []
for line in content.splitlines():
    if line.startswith('msgid "'):
        msgid = line.replace('msgid "', '')
        msgid = msgid[:-1]
        if msgid:
            msgid = translator.translate(msgid, dest=language).text
        newcontent.append(line)
    elif line.startswith('msgstr "'):
        newcontent.append('msgstr "%s"' % msgid)
    else:
        newcontent.append(line)

print('\n'.join(newcontent))