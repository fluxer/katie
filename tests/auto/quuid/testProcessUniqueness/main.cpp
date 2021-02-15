/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016 Ivailo Monev
**
** This file is part of the test suite of the Katie Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
**
** GNU Lesser General Public License Usage
** This file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include <stdio.h>
#include <QUuid>

// This is a testcase for QTBUG-11213
int main(int argc, char **argv)
{
    Q_UNUSED(argc)
    Q_UNUSED(argv)
    
    // First, break QUuid.
    qrand();

    // Now print a few uuids.
    printf("%s", qPrintable(QUuid::createUuid().toString()));
    printf("%s", qPrintable(QUuid::createUuid().toString()));
    printf("%s", qPrintable(QUuid::createUuid().toString()));

    // Done
    return 0;
}

