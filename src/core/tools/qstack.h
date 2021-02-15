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

#ifndef QSTACK_H
#define QSTACK_H

#include <QtCore/qvector.h>


QT_BEGIN_NAMESPACE


template<class T>
class Q_CORE_EXPORT QStack : public QVector<T>
{
public:
    inline QStack() : QVector<T>() {}
    inline ~QStack() {}
    inline void swap(QStack<T> &other) { QVector<T>::swap(other); } // prevent QVector<->QStack swaps
    inline void push(const T &t) { QVector<T>::append(t); }
    inline T pop();
    inline T &top() { return QVector<T>::last(); }; // for compatibility
    const T &top() const { return QVector<T>::last(); }; // for compatibility
};

template<class T>
inline T QStack<T>::pop()
{ Q_ASSERT(!this->isEmpty()); T t = this->data()[this->size() -1];
  this->resize(this->size()-1); return t; }

QT_END_NAMESPACE


#endif // QSTACK_H
