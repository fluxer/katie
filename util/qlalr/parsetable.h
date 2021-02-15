/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016 Ivailo Monev
**
** This file is part of the utils of the Katie Toolkit.
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

#ifndef PARSETABLE_H
#define PARSETABLE_H

#include <QTextStream>

QT_BEGIN_NAMESPACE

class Automaton;

class ParseTable
{
public:
  ParseTable (QTextStream &out);

  void operator () (Automaton *a);

private:
  QTextStream &out;
};

QT_END_NAMESPACE

#endif // PARSETABLE_H
