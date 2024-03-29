/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016 Ivailo Monev
**
** This file is part of the QtTest module of the Katie Toolkit.
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

#ifndef QTESTCORELIST_H
#define QTESTCORELIST_H

#include <QtCore/qglobal.h>


QT_BEGIN_NAMESPACE

template <class T>
class Q_TEST_EXPORT QTestCoreList
{
public:
    QTestCoreList();
    virtual ~QTestCoreList();

    void addToList(T **list);
    T *nextElement();
    T *previousElement();
    int count(T *list);
    int count();

private:
    T *next;
    T *prev;
};

template <class T>
QTestCoreList<T>::QTestCoreList()
    : next(0),
    prev(0)
{
}

template <class T>
QTestCoreList<T>::~QTestCoreList()
{
    if (prev) {
        prev->next = 0;
    }
    delete prev;

    if (next) {
        next->prev = 0;
    }
    delete next;
}

template <class T>
void QTestCoreList<T>::addToList(T **list)
{
    if (next)
        next->addToList(list);
    else {
        next = *list;
        if (next)
            next->prev = static_cast<T*>(this);
    }

    *list = static_cast<T*>(this);
}

template <class T>
T *QTestCoreList<T>::nextElement()
{
    return next;
}

template <class T>
T *QTestCoreList<T>::previousElement()
{
    return prev;
}

template <class T>
int QTestCoreList<T>::count()
{
    int numOfElements = 0;
    T *it = next;

    while(it){
        ++numOfElements;
        it = it->nextElement();
    }

    return numOfElements;
}

QT_END_NAMESPACE


#endif
