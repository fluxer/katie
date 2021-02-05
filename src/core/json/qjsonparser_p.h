/****************************************************************************
**
** Copyright (c) 2012-2015 Barbara Geller
** Copyright (c) 2012-2015 Ansel Sermersheim
** Copyright (c) 2012-2014 Digia Plc and/or its subsidiary(-ies).
** Copyright (c) 2008-2012 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef QJSONPARSER_P_H
#define QJSONPARSER_P_H

#include "qjsondocument.h"
#include "qvarlengtharray.h"

QT_BEGIN_NAMESPACE

namespace QJsonPrivate {

class Parser
{
 public:
   Parser(const char *json, int length);

   QJsonDocument parse(QJsonParseError *error);

   class ParsedObject
   {
    public:
      ParsedObject(Parser *p, int pos) : parser(p), objectPosition(pos) {}
      void insert(uint offset);

      Parser *parser;
      int objectPosition;
      QVarLengthArray<uint, 64> offsets;

      inline QJsonPrivate::Entry *entryAt(int i) const {
         return reinterpret_cast<QJsonPrivate::Entry *>(parser->data + objectPosition + offsets[i]);
      }
   };


 private:
   inline void eatBOM();
   inline bool eatSpace();
   inline char nextToken();

   bool parseObject();
   bool parseArray();
   bool parseMember(int baseOffset);
   bool parseString(bool *latin1);
   bool parseValue(QJsonPrivate::Value *val, int baseOffset);
   bool parseNumber(QJsonPrivate::Value *val, int baseOffset);
   const char *head;
   const char *json;
   const char *end;

   char *data;
   int dataLength;
   int current;
   int nestingLevel;
   QJsonParseError::ParseError lastError;

   inline int reserveSpace(int space) {
      if (current + space >= dataLength) {
         dataLength = 2 * dataLength + space;
         data = (char *)realloc(data, dataLength);
      }
      int pos = current;
      current += space;
      return pos;
   }
};

}

QT_END_NAMESPACE

#endif
