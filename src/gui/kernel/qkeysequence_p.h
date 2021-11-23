/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016 Ivailo Monev
**
** This file is part of the QtGui module of the Katie Toolkit.
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

#ifndef QKEYSEQUENCE_P_H
#define QKEYSEQUENCE_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Katie API.  It exists for the convenience
// of qapplication_*.cpp, qwidget*.cpp and qfiledialog.cpp.  This header
// file may change from version to version without notice, or even be removed.
//
// We mean it.
//

#include "qkeysequence.h"

QT_BEGIN_NAMESPACE

#ifndef QT_NO_SHORTCUT
struct Q_AUTOTEST_EXPORT QKeyBinding
{
    const QKeySequence::StandardKey standardKey;
    const uint shortcut;
};

class Q_AUTOTEST_EXPORT QKeySequencePrivate
{
public:
    static QString encodeString(int key, QKeySequence::SequenceFormat format);
    static int decodeString(const QString &keyStr, QKeySequence::SequenceFormat format);

    static const QKeyBinding keyBindings[];
    static const short numberOfKeyBindings;

};
#endif // QT_NO_SHORTCUT

QT_END_NAMESPACE

#endif //QKEYSEQUENCE_P_H
