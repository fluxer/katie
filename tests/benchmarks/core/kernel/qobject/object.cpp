/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016 Ivailo Monev
**
** This file is part of the test suite of the Katie Toolkit.
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
#include "object.h"

void Object::emitSignal0()
{ emit signal0(); }
void Object::emitSignal1()
{ emit signal1(); }


void Object::slot0()
{ }
void Object::slot1()
{ }
void Object::slot2()
{ }
void Object::slot3()
{ }
void Object::slot4()
{ }
void Object::slot5()
{ }
void Object::slot6()
{ }
void Object::slot7()
{ }
void Object::slot8()
{ }
void Object::slot9()
{ }

#include "moc_object.h"
