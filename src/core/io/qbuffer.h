/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016-2021 Ivailo Monev
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
** As a special exception, The Qt Company gives you certain additional
** rights. These rights are described in The Qt Company LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef QBUFFER_H
#define QBUFFER_H

#include <QtCore/qiodevice.h>


QT_BEGIN_NAMESPACE

class QObject;
class QBufferPrivate;

class Q_CORE_EXPORT QBuffer : public QIODevice
{
#ifndef QT_NO_QOBJECT
    Q_OBJECT
#endif

public:
#ifndef QT_NO_QOBJECT
     explicit QBuffer(QObject *parent = Q_NULLPTR);
     QBuffer(QByteArray *buf, QObject *parent = Q_NULLPTR);
#else
     QBuffer();
     explicit QBuffer(QByteArray *buf);
#endif
    ~QBuffer();

    QByteArray &buffer();
    const QByteArray &buffer() const;
    void setBuffer(QByteArray *a);

    void setData(const QByteArray &data);
    inline void setData(const char *data, const int len)
        { setData(QByteArray::fromRawData(data, len)); }
    const QByteArray &data() const;

    bool open(OpenMode openMode);

    qint64 size() const;
    bool seek(qint64 off);
    bool canReadLine() const;

protected:
#ifndef QT_NO_QOBJECT
    void connectNotify(const char *signal);
    void disconnectNotify(const char *signal);
#endif
    qint64 readData(char *data, qint64 maxlen);
    qint64 writeData(const char *data, qint64 len);

private:
    Q_DECLARE_PRIVATE(QBuffer)
    Q_DISABLE_COPY(QBuffer)

    Q_PRIVATE_SLOT(d_func(), void _q_emitSignals())
};

QT_END_NAMESPACE


#endif // QBUFFER_H
