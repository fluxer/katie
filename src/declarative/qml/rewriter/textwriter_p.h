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

#ifndef TEXTWRITER_H
#define TEXTWRITER_H

#include <QtCore/QString>
#include <QtCore/QList>
#include <QtGui/QTextCursor>

QT_BEGIN_NAMESPACE

namespace QDeclarativeJS {

class TextWriter
{
	struct Replace {
		int pos;
		int length;
		QString replacement;
	};

	QList<Replace> replaceList;

	bool hasOverlap(int pos, int length) const;

public:
	TextWriter();

	void replace(int pos, int length, const QString &replacement);

	void write(QString *s);
};

} // end of namespace QDeclarativeJS

QT_END_NAMESPACE

#endif // TEXTWRITER_H
