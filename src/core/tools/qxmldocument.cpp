/****************************************************************************
**
** Copyright (C) 2022 Ivailo Monev
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

#include "qxmldocument.h"
#include "qstringlist.h"
#include "qmap.h"
#include "qscopedpointer.h"

#include <expat.h>

QT_BEGIN_NAMESPACE

class QXmlDocumentPrivate {
public:
    QXmlDocumentPrivate() : ref(1), q(nullptr), xmlparser(nullptr) { }

    QAtomicInt ref;
    QByteArray xml;
    QString error;
    QXmlDocument *q;
    XML_Parser xmlparser;

    static void beginElement(void *xmldata, const XML_Char *xmlelement, const XML_Char **xmlattributes);
    static void endElement(void *xmldata, const XML_Char *xmlelement);

private:
    Q_DISABLE_COPY(QXmlDocumentPrivate);
};

void QXmlDocumentPrivate::beginElement(void *xmldata, const XML_Char *xmlelement, const XML_Char **xmlattributes)
{
    QXmlDocumentPrivate* xd = reinterpret_cast<QXmlDocumentPrivate*>(xmldata);

    const QByteArray byteelement(xmlelement);
    // qDebug() << Q_FUNC_INFO << byteelement;

    QList<QXmlAttribute> listattributes;
    for (int i = 0; xmlattributes[i]; i += 2) {
        QXmlAttribute attribute;
        attribute.name = xmlattributes[i];
        attribute.value = xmlattributes[i + 1];
        listattributes.append(attribute);
        // qDebug() << Q_FUNC_INFO << attribute.name << attribute.value;
    }

    if (xd->q->beginElement(byteelement, listattributes) == false) {
        XML_StopParser(xd->xmlparser, false);
    }
}

void QXmlDocumentPrivate::endElement(void *xmldata, const XML_Char *xmlelement)
{
    QXmlDocumentPrivate* xd = reinterpret_cast<QXmlDocumentPrivate*>(xmldata);

    const QByteArray byteelement(xmlelement);
    // qDebug() << Q_FUNC_INFO << byteelement;

    if (xd->q->endElement(byteelement) == false) {
        XML_StopParser(xd->xmlparser, false);
    }
}

QXmlDocument::QXmlDocument()
   : d_ptr(nullptr)
{
}

QXmlDocument::~QXmlDocument()
{
    Q_D(QXmlDocument);
    if (d && !d->ref.deref()) {
        delete d;
    }
}

QXmlDocument::QXmlDocument(const QXmlDocument &other)
    : d_ptr(other.d_ptr)
{
    Q_D(QXmlDocument);
    if (d) {
        d->ref.ref();
    }
}

/*!
    Assigns the \a other document to this QXmlDocument.
    Returns a reference to this object.
*/
QXmlDocument &QXmlDocument::operator=(const QXmlDocument &other)
{
    qAtomicAssign(d_ptr, other.d_ptr);
    return *this;
}

/*!
    Reimplement this method in sub-class. If false is returned from
    the method parsing will stop.
 */
bool QXmlDocument::beginElement(const QByteArray &element, const QList<QXmlAttribute> &attributes)
{
    return false;
}

/*!
    Reimplement this method in sub-class. If false is returned from
    the method parsing will stop.
 */
bool QXmlDocument::endElement(const QByteArray &element)
{
    return true;
}

/*!
    Parses a UTF-8 encoded XML document and creates a QXmlDocument
    from it.

    \a xml contains the xml document to be parsed.

    \sa toXml(), errorString()
 */
void QXmlDocument::parse(const QByteArray &xml)
{
    QScopedPointer<QXmlDocumentPrivate> xd(new QXmlDocumentPrivate());
    xd->xml = xml;

    XML_Parser xmlparser = XML_ParserCreate(NULL);
    if (Q_LIKELY(xmlparser)) {
        xd->q = this;
        xd->xmlparser = xmlparser;

        XML_SetUserData(xmlparser, xd.data());
        XML_SetElementHandler(xmlparser, QXmlDocumentPrivate::beginElement, QXmlDocumentPrivate::endElement);

        if (XML_Parse(xmlparser, xml.constData(), xml.size(), true) == XML_STATUS_ERROR) {
            // qDebug() << Q_FUNC_INFO << XML_GetCurrentLineNumber(xmlparser) << XML_ErrorString(XML_GetErrorCode(xmlparser));
            xd->error = QString::fromLatin1(XML_ErrorString(XML_GetErrorCode(xmlparser)));
        }

        XML_ParserFree(xmlparser);
    }

    if (Q_UNLIKELY(!xd->error.isEmpty())) {
        xd->xml.clear();
    }

    qAtomicAssign(d_ptr, xd.take());
}

/*!
    Returns \c true if the \a other document is equal to this document.
 */
bool QXmlDocument::operator==(const QXmlDocument &other) const
{
    Q_D(const QXmlDocument);
    if (d == other.d_ptr) {
        return true;
    }

    if (!d || !other.d_ptr) {
        return false;
    }

    return (d->xml == other.d_ptr->xml);
}

/*!
    \fn bool QXmlDocument::operator!=(const QXmlDocument &other) const

    returns \c true if \a other is not equal to this document
 */

/*!
    returns true if this document is null.

    Null documents are documents created through the default constructor.

    Documents created from UTF-8 encoded text are validated during parsing. If
    validation fails, the returned document will be null.
 */
bool QXmlDocument::isNull() const
{
    Q_D(const QXmlDocument);
    return (!d || d->xml.isEmpty());
}

/*!
    returns the error that occured during parsing.
 */
QString QXmlDocument::errorString() const
{
    Q_D(const QXmlDocument);
    if (!d) {
        return QString();
    }

    return d->error;
}

QT_END_NAMESPACE
