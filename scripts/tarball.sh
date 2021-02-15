#!/bin/sh

set -e

cwd="$(readlink -f $(dirname $0))"

tarball="Katie-4.10.0-Source"

git archive HEAD --format=tar --prefix="$tarball/" | xz > "$cwd/../$tarball.tar.xz"
