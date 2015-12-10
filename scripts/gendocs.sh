#!/bin/bash

set -e

cwd="$(dirname $0)"

if ! type -p doxygen;then
    echo "Doxygen is not installed"
    exit 1
fi

rm -rf apidocs/*
echo "Generating API docs for Katie..."
doxygen ${cwd}/../doxygen.conf