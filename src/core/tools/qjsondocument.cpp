/****************************************************************************
**
** Copyright (C) 2021 Ivailo Monev
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

#include "qjsondocument.h"
#include "qstringlist.h"
#include "qvariant.h"
#include "qscopedpointer.h"
#include "qcoreapplication.h"

#include <jansson.h>

QT_BEGIN_NAMESPACE

class QJsonDocumentPrivate {
public:
    QJsonDocumentPrivate() : ref(1) { }

    QVariant jsonToVariant(const QByteArray &jsondata);
    void variantToJson(const QVariant &jsonvariant, json_t *jroot, quint16 jdepth);

    QAtomicInt ref;
    QByteArray json;
    QVariant variant;
    QString error;

private:
    Q_DISABLE_COPY(QJsonDocumentPrivate);
};

QVariant QJsonDocumentPrivate::jsonToVariant(const QByteArray &jsondata)
{
    QVariant result;

    if (jsondata.isEmpty()) {
        error = QCoreApplication::translate("QJsonDocument", "Data is empty");
        return result;
    }

    json_error_t jerror;
    json_t *jroot = json_loadb(jsondata.constData(), jsondata.size(), JSON_ALLOW_NUL, &jerror);

    if (!jroot) {
        error = jerror.text;
        return result;
    }

    switch(json_typeof(jroot)) {
        case JSON_OBJECT: {
            QVariantMap mapresult;
            const char *jkey;
            json_t *jobject;
            json_object_foreach(jroot, jkey, jobject) {
                switch(json_typeof(jobject)) {
                    case JSON_OBJECT: {
                        char* jdata = json_dumps(jobject, 0);
                        mapresult.insert(jkey, jsonToVariant(jdata));
                        ::free(jdata);
                        break;
                    }
                    case JSON_ARRAY: {
                        QVariantList listvalue;
                        for (size_t i = 0; i < json_array_size(jobject); i++) {
                            json_t *jarray = json_array_get(jobject, i);
                            switch(json_typeof(jarray)) {
                                case JSON_OBJECT: {
                                    char* jdata = json_dumps(jarray, 0);
                                    listvalue.append(jsonToVariant(jdata));
                                    ::free(jdata);
                                    break;
                                }
                                case JSON_STRING: {
                                    listvalue.append(QVariant(QString::fromUtf8(json_string_value(jarray), json_string_length(jarray))));
                                    break;
                                }
                                case JSON_INTEGER: {
                                    listvalue.append(QVariant(json_integer_value(jarray)));
                                    break;
                                }
                                case JSON_REAL: {
                                    listvalue.append(QVariant(json_real_value(jarray)));
                                    break;
                                }
                                case JSON_TRUE: {
                                    listvalue.append(QVariant(true));
                                    break;
                                }
                                case JSON_FALSE: {
                                    listvalue.append(QVariant(false));
                                    break;
                                }
                                case JSON_NULL: {
                                    listvalue.append(QVariant());
                                    break;
                                }
                                default: {
                                    error = QCoreApplication::translate("QJsonDocument", "Unknown JSON type in array");
                                    break;
                                }
                            }
                        }
                        mapresult.insert(jkey, listvalue);
                        break;
                    }
                    case JSON_STRING: {
                        mapresult.insert(jkey, QVariant(QString::fromUtf8(json_string_value(jobject), json_string_length(jobject))));
                        break;
                    }
                    case JSON_INTEGER: {
                        mapresult.insert(jkey, QVariant(json_integer_value(jobject)));
                        break;
                    }
                    case JSON_REAL: {
                        mapresult.insert(jkey, QVariant(json_real_value(jobject)));
                        break;
                    }
                    case JSON_TRUE: {
                        mapresult.insert(jkey, QVariant(true));
                        break;
                    }
                    case JSON_FALSE: {
                        mapresult.insert(jkey, QVariant(false));
                        break;
                    }
                    case JSON_NULL: {
                        mapresult.insert(jkey, QVariant());
                        break;
                    }
                    default: {
                        error = QCoreApplication::translate("QJsonDocument", "Unknown JSON type");
                        break;
                    }
                }
            }
            result = mapresult;
            break;
        }
        case JSON_ARRAY: {
            QVariantList listvalue;
            for (size_t i = 0; i < json_array_size(jroot); i++) {
                json_t *jarray = json_array_get(jroot, i);
                switch(json_typeof(jarray)) {
                    case JSON_OBJECT: {
                        char* jdata = json_dumps(jarray, 0);
                        listvalue.append(jsonToVariant(jdata));
                        ::free(jdata);
                        break;
                    }
                    case JSON_STRING: {
                        listvalue.append(QVariant(QString::fromUtf8(json_string_value(jarray), json_string_length(jarray))));
                        break;
                    }
                    case JSON_INTEGER: {
                        listvalue.append(QVariant(json_integer_value(jarray)));
                        break;
                    }
                    case JSON_REAL: {
                        listvalue.append(QVariant(json_real_value(jarray)));
                        break;
                    }
                    case JSON_TRUE: {
                        listvalue.append(QVariant(true));
                        break;
                    }
                    case JSON_FALSE: {
                        listvalue.append(QVariant(false));
                        break;
                    }
                    case JSON_NULL: {
                        listvalue.append(QVariant());
                        break;
                    }
                    default: {
                        error = QCoreApplication::translate("QJsonDocument", "Unknown JSON type in array");
                        break;
                    }
                }
            }
            result = listvalue;
            break;
        }
        default: {
            error = QCoreApplication::translate("QJsonDocument", "Rootless values are not supported");
            break;
        }
    }

    json_decref(jroot);

    // qDebug() << "converted" << jsondata << "to" << result;

    return result;
}

void QJsonDocumentPrivate::variantToJson(const QVariant &jsonvariant, json_t *jroot, quint16 jdepth)
{
    if (jsonvariant.isNull()) {
        error = QCoreApplication::translate("QJsonDocument", "Data variant is null");
        return;
    } else if (Q_UNLIKELY(jdepth >= JSON_PARSER_MAX_DEPTH)) {
        error = QCoreApplication::translate("QJsonDocument", "Maximum depth reached");
        return;
    }

    switch (jsonvariant.type()) {
        case QVariant::Map:
        case QVariant::Hash: {
            const QVariantMap jsonmap = jsonvariant.toMap();
            if (jdepth == 1 && jsonmap.isEmpty()) {
                error = QCoreApplication::translate("QJsonDocument", "Data variant is null");
                return;
            }
            QVariantMap::const_iterator jsonmapit = jsonmap.constBegin();
            while(jsonmapit != jsonmap.constEnd()) {
                const QVariant value = jsonmapit.value();
                const QByteArray bytearraykey = jsonmapit.key().toUtf8();
                jsonmapit++;
                switch(value.type()) {
                    case QVariant::Invalid: {
                        json_object_set_new_nocheck(jroot, bytearraykey.constData(), json_null());
                        break;
                    }
                    case QVariant::Bool: {
                        json_object_set_new_nocheck(jroot, bytearraykey.constData(), value.toBool() ? json_true() : json_false());
                        break;
                    }
                    case QVariant::Int:
                    case QVariant::LongLong: {
                        json_object_set_new_nocheck(jroot, bytearraykey.constData(), json_integer(value.toLongLong()));
                        break;
                    }
                    case QVariant::UInt:
                    case QVariant::ULongLong: {
                        json_object_set_new_nocheck(jroot, bytearraykey.constData(), json_integer(value.toULongLong()));
                        break;
                    }
                    case QVariant::Float:
                    case QVariant::Double: {
                        json_object_set_new_nocheck(jroot, bytearraykey.constData(), json_real(value.toReal()));
                        break;
                    }
                    case QVariant::ByteArray:
                    case QVariant::String: {
                        const QByteArray bytearrayvalue = value.toByteArray();
                        json_object_set_new_nocheck(jroot, bytearraykey.constData(), json_string(bytearrayvalue.constData()));
                        break;
                    }
                    case QVariant::List: {
                        // mixed variant type lists are not supported
                        bool isobject = true;
                        const QVariantList valueaslist = value.toList();
                        foreach (const QVariant &listvalue, valueaslist) {
                            if (listvalue.type() != QVariant::Map && listvalue.type() != QVariant::Hash) {
                                isobject = false;
                                break;
                            }
                        }
                        if (isobject) {
                            json_t *jarray = json_array();
                            foreach (const QVariant &listvalue, valueaslist) {
                                jdepth++;
                                json_t *jrootn = json_object();
                                variantToJson(listvalue, jrootn, jdepth);
                                json_array_append_new(jarray, jrootn);
                                jdepth--;
                            }
                            json_object_set_new_nocheck(jroot, bytearraykey.constData(), jarray);
                            break;
                        }
                    }
                    case QVariant::StringList: {
                        json_t *jarray = json_array();
                        foreach(const QString &listvalue, value.toStringList()) {
                            const QByteArray bytearrayvalue = listvalue.toUtf8();
                            json_array_append_new(jarray, json_stringn_nocheck(bytearrayvalue.constData(), bytearrayvalue.size()));
                        }
                        json_object_set_new_nocheck(jroot, bytearraykey.constData(), jarray);
                        break;
                    }
                    case QVariant::Hash:
                    case QVariant::Map: {
                        jdepth++;
                        json_t *jrootn = json_object();
                        variantToJson(value.toMap(), jrootn, jdepth);
                        json_object_set_new_nocheck(jroot, bytearraykey.constData(), jrootn);
                        jdepth--;
                        break;
                    }
                    default: {
                        error = QCoreApplication::translate("QJsonDocument", "Unknown variant type");
                        break;
                    }
                }
            }
            break;
        }
        case QVariant::Invalid:
        case QVariant::Bool:
        case QVariant::Int:
        case QVariant::LongLong:
        case QVariant::UInt:
        case QVariant::ULongLong:
        case QVariant::Float:
        case QVariant::Double:
        case QVariant::ByteArray:
        case QVariant::String: {
            error = QCoreApplication::translate("QJsonDocument", "Rootless values are not supported");
            break;
        }
        case QVariant::List:
        case QVariant::StringList: {
            error = QCoreApplication::translate("QJsonDocument", "Rootless arrays are not supported");
            break;
        }
        default: {
            error = QCoreApplication::translate("QJsonDocument", "Unknown variant type");
            break;
        }
    }

    // qDebug() << "converted" << jsonmap << "to" << json_dumps(jroot, 0);
}

QJsonDocument::QJsonDocument()
   : d_ptr(nullptr)
{
}

QJsonDocument::~QJsonDocument()
{
    Q_D(QJsonDocument);
    if (d && !d->ref.deref()) {
        delete d;
    }
}

QJsonDocument::QJsonDocument(const QJsonDocument &other)
    : d_ptr(other.d_ptr)
{
    Q_D(QJsonDocument);
    if (d) {
        d->ref.ref();
    }
}

/*!
    Assigns the \a other document to this QJsonDocument.
    Returns a reference to this object.
*/
QJsonDocument &QJsonDocument::operator=(const QJsonDocument &other)
{
    qAtomicAssign(d_ptr, other.d_ptr);
    return *this;
}

/*!
    Creates a QJsonDocument from the QVariant \a variant.

    If the \a variant contains any other type than a QVariant::Invalid,
    QVariant::Bool, QVariant::Int, QVariant::LongLong, QVariant::UInt,
    QVariant::ULongLong, QVariant::Float, QVariant::Double,
    QVariant::ByteArray, QVariant::String, QVariant::List,
    QVariant::StringList, QVariant::Hash or QVariant::Map the returned document
    is null.

    \sa toVariant()
 */
QJsonDocument QJsonDocument::fromVariant(const QVariant &variant)
{
    static const size_t jflags = JSON_SORT_KEYS | JSON_INDENT(4);

    QScopedPointer<QJsonDocumentPrivate> d(new QJsonDocumentPrivate());
    d->variant = variant;
    json_t *jroot = json_object();
    d->variantToJson(d->variant, jroot, 1);
    char *jdata = json_dumps(jroot, jflags);
    d->json = jdata;
    ::free(jdata);
    json_decref(jroot);

    QJsonDocument jd;
    if (Q_UNLIKELY(!d->error.isEmpty())) {
        d->json.clear();
        d->variant.clear();
    }

    jd.d_ptr = d.take();

    // qDebug() << "QJsonDocument::fromVariant" << jd.errorString();

    return jd;
}

/*!
    Returns a QVariant representing the Json document.

    The returned variant will be a QVariantList if the document is
    a array and a QVariantMap if the document is a object.

    \sa fromVariant()
 */
QVariant QJsonDocument::toVariant() const
{
    Q_D(const QJsonDocument);
    if (!d) {
        return QVariant();
    }

    return d->variant;
}

/*!
    Parses a UTF-8 encoded JSON document and creates a QJsonDocument
    from it.

    \a json contains the json document to be parsed.

    \sa toJson(), errorString()
 */
QJsonDocument QJsonDocument::fromJson(const QByteArray &json)
{
    QScopedPointer<QJsonDocumentPrivate> d(new QJsonDocumentPrivate());
    d->json = json;
    d->variant = d->jsonToVariant(json);

    if (Q_UNLIKELY(!d->error.isEmpty())) {
        d->json.clear();
        d->variant.clear();
    }

    QJsonDocument jd;
    jd.d_ptr = d.take();

    // qDebug() << "QJsonDocument::fromJson" << jd.errorString();

    return jd;
}

QByteArray QJsonDocument::toJson() const
{
    Q_D(const QJsonDocument);
    if (!d) {
        return QByteArray();
    }

    return d->json;
}

/*!
    Returns \c true if the \a other document is equal to this document.
 */
bool QJsonDocument::operator==(const QJsonDocument &other) const
{
    Q_D(const QJsonDocument);
    if (d == other.d_ptr) {
        return true;
    }

    if (!d || !other.d_ptr) {
        return false;
    }

    return (d->json == other.d_ptr->json && d->variant == other.d_ptr->variant);
}

/*!
    \fn bool QJsonDocument::operator!=(const QJsonDocument &other) const

    returns \c true if \a other is not equal to this document
 */

/*!
    returns true if this document is null.

    Null documents are documents created through the default constructor.

    Documents created from UTF-8 encoded text are validated during parsing. If
    validation fails, the returned document will be null.
 */
bool QJsonDocument::isNull() const
{
    Q_D(const QJsonDocument);
    return (!d || (d->json.isEmpty() && d->variant.isNull()));
}

QString QJsonDocument::errorString() const
{
    Q_D(const QJsonDocument);
    if (!d) {
        return QString();
    }

    return d->error;
}

QDebug operator<<(QDebug dbg, const QJsonDocument &jd)
{
    if (jd.isNull()) {
        dbg << "QJsonDocument()";
        return dbg;
    }
    dbg.nospace() << "QJsonDocument("
                  << jd.toJson().constData()
                  << ")";
    return dbg.space();
}

QT_END_NAMESPACE
