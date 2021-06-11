/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016 Ivailo Monev
**
** This file is part of the QtCore module of the Katie Toolkit.
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

#ifndef QCOREAPPLICATION_P_H
#define QCOREAPPLICATION_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Katie API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include "QtCore/qcoreapplication.h"
#include "QtCore/qtranslator.h"
#include "QtCore/qsettings.h"
#include "qobject_p.h"

#include <bitset>

QT_BEGIN_NAMESPACE

typedef QList<QTranslator*> QTranslatorList;

class QAbstractEventDispatcher;

class Q_CORE_EXPORT QCoreApplicationPrivate : public QObjectPrivate
{
    Q_DECLARE_PUBLIC(QCoreApplication)

public:
    QCoreApplicationPrivate(int &aargc,  char **aargv);
    ~QCoreApplicationPrivate();

    bool sendThroughApplicationEventFilters(QObject *, QEvent *);
    bool sendThroughObjectEventFilters(QObject *, QEvent *);
    bool notify_helper(QObject *, QEvent *);

    virtual QAbstractEventDispatcher* createEventDispatcher();
    static void removePostedEvent(QEvent *);

    static bool checkInstance(const char *method);
    static void sendPostedEvents(QObject *receiver, int event_type, QThreadData *data);

#if !defined (QT_NO_DEBUG)
    void checkReceiverThread(QObject *receiver);
#endif
    const int &argc;
    char **const argv;

#ifndef QT_NO_TRANSLATION
    QTranslatorList translators;
#endif

    QCoreApplication::EventFilter eventFilter;

    bool in_exec;
    QString cachedApplicationDirPath;
    QString cachedApplicationFilePath;

    static bool is_app_running;
    static bool is_app_closing;

    static std::bitset<Qt::AA_AttributeCount> attribs;
    static QCoreApplication::Type app_type;
#ifndef QT_NO_SETTINGS
    static QSettings *staticConf();
#endif
};

QT_END_NAMESPACE

#endif // QCOREAPPLICATION_P_H
