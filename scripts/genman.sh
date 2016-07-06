#!/bin/bash

set -e

cwd="$(dirname $0)"

if ! type -p pod2man;then
    echo "Perl is not installed"
    exit 1
fi

echo "Generating man pages for Katie..."
for pfile in $(find $cwd/../src/tools -name '*.pod');do
    ofile="${pfile//.pod/.1}"
    pod2man --release="Katie 4.9.0" \
            --center="Katie Manual" \
            --section='1' --utf8 $pfile > $ofile
done