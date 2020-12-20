#!/bin/sh

rm -rf generated
mkdir generated
cd generated

make -f ../DerivedSources.make JavaScriptCore=.. V=1
cd ..
