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

#ifndef QBSPTREE_P_H
#define QBSPTREE_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Katie API.  It exists for the convenience
// of other Qt classes.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include "qvector.h"
#include "qrect.h"

QT_BEGIN_NAMESPACE

class QBspTree
{
public:

    struct Node
    {
        enum Type { None = 0, VerticalPlane = 1, HorizontalPlane = 2, Both = 3 };
        inline Node() : pos(0), type(None) {}
        int pos;
        Type type;
    };
    typedef Node::Type NodeType;

    struct Data
    {
        Data(void *p) : ptr(p) {}
        Data(int n) : i(n) {}
        union {
            void *ptr;
            int i;
        };
    };
    typedef QBspTree::Data QBspTreeData;
    typedef void callback(QVector<int> &leaf, const QRect &area, uint visited, QBspTreeData data);

    QBspTree();

    void create(int n);
    void destroy();

    inline void init(const QRect &area, NodeType type) { init(area, depth, type, 0); }

    void climbTree(const QRect &rect, callback *function, QBspTreeData data);

    inline int leafCount() const { return leaves.count(); }
    inline QVector<int> &leaf(int i) { return leaves[i]; }
    inline void insertLeaf(const QRect &r, int i) { climbTree(r, &insert, i, 0); }
    inline void removeLeaf(const QRect &r, int i) { climbTree(r, &remove, i, 0); }

protected:
    void init(const QRect &area, int depth, NodeType type, int index);
    void climbTree(const QRect &rect, callback *function, QBspTreeData data, int index);

    inline int parentIndex(int i) const { return (i & 1) ? ((i - 1) / 2) : ((i - 2) / 2); }
    inline int firstChildIndex(int i) const { return ((i * 2) + 1); }

    static void insert(QVector<int> &leaf, const QRect &area, uint visited, QBspTreeData data);
    static void remove(QVector<int> &leaf, const QRect &area, uint visited, QBspTreeData data);

private:
    uint depth;
    uint visited;
    QVector<Node> nodes;
    QVector< QVector<int> > leaves; // the leaves are just indices into the items
};

QT_END_NAMESPACE

#endif // QBSPTREE_P_H
