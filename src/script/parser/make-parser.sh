#!/bin/sh
#############################################################################
##
## Copyright (C) 2015 The Qt Company Ltd.
## Copyright (C) 2016 Ivailo Monev
##
## This file is the build configuration utility of the Katie Toolkit.
##
## $QT_BEGIN_LICENSE:LGPL$
##
## GNU Lesser General Public License Usage
## This file may be used under the terms of the GNU Lesser
## General Public License version 2.1 as published by the Free Software
## Foundation and appearing in the file LICENSE.LGPL included in the
## packaging of this file.  Please review the following information to
## ensure the GNU Lesser General Public License version 2.1 requirements
## will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
##
## $QT_END_LICENSE$
##
#############################################################################

me=$(dirname $0)
mkdir -p $me/out
(cd $me/out && ../../../../build/bin/qlalr --qt --no-lines ../qscript.g)

for f in $me/out/*.h $me/out/*.cpp; do
    n=$(basename $f)
    cp $f $n
done

git diff .

