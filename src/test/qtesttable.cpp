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

#include "qtesttable_p.h"
#include "qtestdata.h"
#include "qtestassert.h"

#include <string.h>

QT_BEGIN_NAMESPACE

class QTestTablePrivate
{
public:
    struct ElementList
    {
        ElementList(): elementName(0), elementType(0), next(0) {}
        const char *elementName;
        int elementType;
        ElementList *next;
    };

    struct DataList
    {
        DataList(): data(0), next(0) {}
        QTestData *data;
        DataList *next;
    };

    QTestTablePrivate(): list(0), dataList(0) {}
    ~QTestTablePrivate();

    ElementList *list;
    DataList *dataList;

    void append(int elemType, const char *elemName);
    void append(QTestData *data);
    ElementList *elementAt(int index) const;
    QTestData *dataAt(int index) const;

    static QTestTable *currentTestTable;
    static QTestTable *gTable;
};

QTestTable *QTestTablePrivate::currentTestTable = 0;
QTestTable *QTestTablePrivate::gTable = 0;

QTestTablePrivate::ElementList *QTestTablePrivate::elementAt(int index) const
{
    ElementList *iter = list;
    for (int i = 0; i < index; ++i) {
        if (!iter)
            return 0;
        iter = iter->next;
    }
    return iter;
}

QTestData *QTestTablePrivate::dataAt(int index) const
{
    DataList *iter = dataList;
    for (int i = 0; i < index; ++i) {
        if (!iter)
            return 0;
        iter = iter->next;
    }
    return iter ? iter->data : 0;
}

QTestTablePrivate::~QTestTablePrivate()
{
    DataList *dit = dataList;
    while (dit) {
        DataList *next = dit->next;
        delete dit->data;
        delete dit;
        dit = next;
    }
    ElementList *iter = list;
    while (iter) {
        ElementList *next = iter->next;
        delete iter;
        iter = next;
    }
}

void QTestTablePrivate::append(int elemType, const char *elemName)
{
    ElementList *item = new ElementList;
    item->elementName = elemName;
    item->elementType = elemType;
    if (!list) {
        list = item;
        return;
    }
    ElementList *last = list;
    while (last->next != 0)
        last = last->next;
    last->next = item;
}

void QTestTablePrivate::append(QTestData *data)
{
    DataList *item = new DataList;
    item->data = data;
    if (!dataList) {
        dataList = item;
        return;
    }
    DataList *last = dataList;
    while (last->next != 0)
        last = last->next;
    last->next = item;
}

void QTestTable::addColumn(int type, const char *name)
{
    QTEST_ASSERT(type);
    QTEST_ASSERT(name);

    d->append(type, name);
}

int QTestTable::elementCount() const
{
    QTestTablePrivate::ElementList *item = d->list;
    int count = 0;
    while (item) {
        ++count;
        item = item->next;
    }
    return count;
}


int QTestTable::dataCount() const
{
    QTestTablePrivate::DataList *item = d->dataList;
    int count = 0;
    while (item) {
        ++count;
        item = item->next;
    }
    return count;
}

bool QTestTable::isEmpty() const
{
    return !d->list;
}

QTestData *QTestTable::newData(const char *tag)
{
    QTestData *dt = new QTestData(tag, this);
    d->append(dt);
    return dt;
}

QTestTable::QTestTable()
{
    d = new QTestTablePrivate;
    QTestTablePrivate::currentTestTable = this;
}

QTestTable::~QTestTable()
{
    QTestTablePrivate::currentTestTable = 0;
    delete d;
}

int QTestTable::elementTypeId(int index) const
{
    QTestTablePrivate::ElementList *item = d->elementAt(index);
    if (!item)
        return -1;
    return item->elementType;
}

const char *QTestTable::dataTag(int index) const
{
    QTestTablePrivate::ElementList *item = d->elementAt(index);
    if (!item)
        return 0;
    return item->elementName;
}

QTestData *QTestTable::testData(int index) const
{
    return d->dataAt(index);
}

int QTestTable::indexOf(const char *elementName) const
{
    QTEST_ASSERT(elementName);

    QTestTablePrivate::ElementList *item = d->list;
    int i = 0;
    while (item) {
        if (strcmp(elementName, item->elementName) == 0)
            return i;
        item = item->next;
        ++i;
    }
    return -1;
}

QTestTable *QTestTable::globalTestTable()
{
    if (!QTestTablePrivate::gTable)
        QTestTablePrivate::gTable = new QTestTable();
    return QTestTablePrivate::gTable;
}

void QTestTable::clearGlobalTestTable()
{
    delete QTestTablePrivate::gTable;
    QTestTablePrivate::gTable = 0;
}

QTestTable *QTestTable::currentTestTable()
{
    return QTestTablePrivate::currentTestTable;
}

QT_END_NAMESPACE
