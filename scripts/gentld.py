#!/usr/bin/python

# Data is from https://publicsuffix.org/list/public_suffix_list.dat

tldlist = []

with open('public_suffix_list.dat', 'rb') as f:
    content = f.read()
content = content.decode('utf-8')
for line in content.splitlines():
    if line.startswith('//') or not line:
        continue
    tldlist.append(line)

tldcount = len(tldlist)

print('''static const char* TLDTbl[%d] = {''' % tldcount)

for tld in tldlist:
    print('    "%s",' % tld)

print('''};
static const qint16 TLDTblSize = %d;''' % tldcount)
