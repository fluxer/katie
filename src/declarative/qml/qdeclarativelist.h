/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016 Ivailo Monev
**
** This file is part of the QtDeclarative module of the Katie Toolkit.
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

#ifndef QDECLARATIVELIST_H
#define QDECLARATIVELIST_H

#include <QtCore/qlist.h>
#include <QtCore/qvariant.h>


QT_BEGIN_NAMESPACE


class QObject;
struct QMetaObject;

#ifndef QDECLARATIVELISTPROPERTY
#define QDECLARATIVELISTPROPERTY
template<typename T>
class QDeclarativeListProperty {
public:
    typedef void (*AppendFunction)(QDeclarativeListProperty<T> *, T*);
    typedef int (*CountFunction)(QDeclarativeListProperty<T> *);
    typedef T *(*AtFunction)(QDeclarativeListProperty<T> *, int);
    typedef void (*ClearFunction)(QDeclarativeListProperty<T> *);

    QDeclarativeListProperty()
        : object(0), data(0), append(0), count(0), at(0), clear(0) {}
    QDeclarativeListProperty(QObject *o, QList<T *> &list)
        : object(o), data(&list), append(qlist_append), count(qlist_count), at(qlist_at),
          clear(qlist_clear) {}
    QDeclarativeListProperty(QObject *o, void *d, AppendFunction a, CountFunction c = 0, AtFunction t = 0,
                    ClearFunction r = 0)
        : object(o), data(d), append(a), count(c), at(t), clear(r) {}

    bool operator==(const QDeclarativeListProperty &o) const {
        return object == o.object &&
               data == o.data &&
               append == o.append &&
               count == o.count &&
               at == o.at &&
               clear == o.clear;
    }

    QObject *object;
    void *data;

    AppendFunction append;

    CountFunction count;
    AtFunction at;

    ClearFunction clear;

private:
    static void qlist_append(QDeclarativeListProperty *p, T *v) {
        reinterpret_cast<QList<T *> *>(p->data)->append(v);
    }
    static int qlist_count(QDeclarativeListProperty *p) {
        return reinterpret_cast<QList<T *> *>(p->data)->count();
    }
    static T *qlist_at(QDeclarativeListProperty *p, int idx) {
        return reinterpret_cast<QList<T *> *>(p->data)->at(idx);
    }
    static void qlist_clear(QDeclarativeListProperty *p) {
        return reinterpret_cast<QList<T *> *>(p->data)->clear();
    }
};
#endif

class QDeclarativeEngine;
class QDeclarativeListReferencePrivate;
class Q_DECLARATIVE_EXPORT QDeclarativeListReference
{
public:
    QDeclarativeListReference();
    QDeclarativeListReference(QObject *, const char *property, QDeclarativeEngine * = 0);
    QDeclarativeListReference(const QDeclarativeListReference &);
    QDeclarativeListReference &operator=(const QDeclarativeListReference &);
    ~QDeclarativeListReference();

    bool isValid() const;

    QObject *object() const;
    const QMetaObject *listElementType() const;

    bool canAppend() const;
    bool canAt() const;
    bool canClear() const;
    bool canCount() const;

    bool append(QObject *) const;
    QObject *at(int) const;
    bool clear() const;
    int count() const;

private:
    friend class QDeclarativeListReferencePrivate;
    QDeclarativeListReferencePrivate* d;
};

QT_END_NAMESPACE

Q_DECLARE_METATYPE(QDeclarativeListReference)


#endif // QDECLARATIVELIST_H
