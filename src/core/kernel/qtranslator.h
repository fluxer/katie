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

#ifndef QTRANSLATOR_H
#define QTRANSLATOR_H

#include <QtCore/qobject.h>


QT_BEGIN_NAMESPACE


#ifndef QT_NO_TRANSLATION

class QTranslatorPrivate;

class Q_CORE_EXPORT QTranslator : public QObject
{
    Q_OBJECT
public:
    explicit QTranslator(QObject *parent = nullptr);
    ~QTranslator();

    virtual QString translate(const char *context, const char *sourceText) const;

    virtual bool isEmpty() const;

    bool load(const QString &domain);


private:
    Q_DISABLE_COPY(QTranslator)
    Q_DECLARE_PRIVATE(QTranslator)
};

#endif // QT_NO_TRANSLATION

QT_END_NAMESPACE


#endif // QTRANSLATOR_H
