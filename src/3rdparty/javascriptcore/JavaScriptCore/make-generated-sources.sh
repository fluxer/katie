#!/bin/sh

export SRCROOT=$PWD
export WebCore=$PWD
export CREATE_HASH_TABLE="$SRCROOT/create_hash_table"

rm -rf generated
mkdir generated
cd generated

make -f ../DerivedSources.make JavaScriptCore=.. BUILT_PRODUCTS_DIR=..
cd ..
