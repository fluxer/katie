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

#include "qvariant.h"
#include "qbitarray.h"
#include "qbytearray.h"
#include "qdatastream.h"
#include "qdebug.h"
#include "qmap.h"
#include "qdatetime.h"
#include "qmath.h"
#include "qlist.h"
#include "qstring.h"
#include "qstringlist.h"
#include "qurl.h"
#include "qlocale.h"
#include "qvariant_p.h"

#ifndef QT_BOOTSTRAPPED
#  include "qeasingcurve.h"
#  include "qjsondocument.h"
#  include "qsize.h"
#  include "qpoint.h"
#  include "qrect.h"
#  include "qline.h"
#endif

#include <float.h>

QT_BEGIN_NAMESPACE

static const QLatin1String qStringListDelim = QLatin1String(",");

static void construct(QVariant::Private *x, const void *copy)
{
    x->is_shared = false;

    switch (x->type) {
    case QVariant::String:
        v_construct<QString>(x, copy);
        break;
    case QVariant::Char:
        v_construct<QChar>(x, copy);
        break;
    case QVariant::StringList:
        v_construct<QStringList>(x, copy);
        break;
    case QVariant::Map:
        v_construct<QVariantMap>(x, copy);
        break;
    case QVariant::Hash:
        v_construct<QVariantHash>(x, copy);
        break;
    case QVariant::List:
        v_construct<QVariantList>(x, copy);
        break;
    case QVariant::Date:
        v_construct<QDate>(x, copy);
        break;
    case QVariant::Time:
        v_construct<QTime>(x, copy);
        break;
    case QVariant::DateTime:
        v_construct<QDateTime>(x, copy);
        break;
    case QVariant::ByteArray:
        v_construct<QByteArray>(x, copy);
        break;
    case QVariant::BitArray:
        v_construct<QBitArray>(x, copy);
        break;
#ifndef QT_BOOTSTRAPPED
    case QVariant::Size:
        v_construct<QSize>(x, copy);
        break;
    case QVariant::SizeF:
        v_construct<QSizeF>(x, copy);
        break;
    case QVariant::Rect:
        v_construct<QRect>(x, copy);
        break;
    case QVariant::LineF:
        v_construct<QLineF>(x, copy);
        break;
    case QVariant::Line:
        v_construct<QLine>(x, copy);
        break;
    case QVariant::RectF:
        v_construct<QRectF>(x, copy);
        break;
    case QVariant::Point:
        v_construct<QPoint>(x, copy);
        break;
    case QVariant::PointF:
        v_construct<QPointF>(x, copy);
        break;
#endif
#ifndef QT_BOOTSTRAPPED
    case QVariant::Url:
        v_construct<QUrl>(x, copy);
        break;
#endif
    case QVariant::Locale:
        v_construct<QLocale>(x, copy);
        break;
#ifndef QT_NO_REGEXP
    case QVariant::RegExp:
        v_construct<QRegExp>(x, copy);
        break;
#endif
#ifndef QT_BOOTSTRAPPED
    case QVariant::EasingCurve:
        v_construct<QEasingCurve>(x, copy);
        break;
    case QVariant::JsonDocument:
        v_construct<QJsonDocument>(x, copy);
        break;
#endif
    case QVariant::Int:
        x->data.i = copy ? *static_cast<const int *>(copy) : 0;
        break;
    case QVariant::UInt:
        x->data.u = copy ? *static_cast<const uint *>(copy) : 0u;
        break;
    case QVariant::Bool:
        x->data.b = copy ? *static_cast<const bool *>(copy) : false;
        break;
    case QVariant::Double:
        x->data.d = copy ? *static_cast<const double*>(copy) : 0.0;
        break;
    case QVariant::Float:
        x->data.f = copy ? *static_cast<const float*>(copy) : 0.0f;
        break;
    case QMetaType::QObjectStar:
        x->data.o = copy ? *static_cast<QObject *const*>(copy) : 0;
        break;
    case QVariant::LongLong:
        x->data.ll = copy ? *static_cast<const qlonglong *>(copy) : Q_INT64_C(0);
        break;
    case QVariant::ULongLong:
        x->data.ull = copy ? *static_cast<const qulonglong *>(copy) : Q_UINT64_C(0);
        break;
    case QVariant::Invalid:
    case QVariant::UserType:
        break;
    default:
        void *ptr = QMetaType::construct(x->type, copy);
        if (!ptr) {
            x->type = QVariant::Invalid;
        } else {
            x->is_shared = true;
            x->data.shared = new QVariant::PrivateShared(ptr);
        }
        break;
    }
    x->is_null = !copy;
}

static void clear(QVariant::Private *d)
{
    switch (d->type) {
    case QVariant::String:
        v_clear<QString>(d);
        break;
    case QVariant::Char:
        v_clear<QChar>(d);
        break;
    case QVariant::StringList:
        v_clear<QStringList>(d);
        break;
    case QVariant::Map:
        v_clear<QVariantMap>(d);
        break;
    case QVariant::Hash:
        v_clear<QVariantHash>(d);
        break;
    case QVariant::List:
        v_clear<QVariantList>(d);
        break;
    case QVariant::Date:
        v_clear<QDate>(d);
        break;
    case QVariant::Time:
        v_clear<QTime>(d);
        break;
    case QVariant::DateTime:
        v_clear<QDateTime>(d);
        break;
    case QVariant::ByteArray:
        v_clear<QByteArray>(d);
        break;
    case QVariant::BitArray:
        v_clear<QBitArray>(d);
        break;
#ifndef QT_BOOTSTRAPPED
    case QVariant::Point:
        v_clear<QPoint>(d);
        break;
    case QVariant::PointF:
        v_clear<QPointF>(d);
        break;
    case QVariant::Size:
        v_clear<QSize>(d);
        break;
    case QVariant::SizeF:
        v_clear<QSizeF>(d);
        break;
    case QVariant::Rect:
        v_clear<QRect>(d);
        break;
    case QVariant::LineF:
        v_clear<QLineF>(d);
        break;
    case QVariant::Line:
        v_clear<QLine>(d);
        break;
    case QVariant::RectF:
        v_clear<QRectF>(d);
        break;
#endif
#ifndef QT_BOOTSTRAPPED
    case QVariant::Url:
        v_clear<QUrl>(d);
        break;
#endif
    case QVariant::Locale:
        v_clear<QLocale>(d);
        break;
#ifndef QT_NO_REGEXP
    case QVariant::RegExp:
        v_clear<QRegExp>(d);
        break;
#endif
#ifndef QT_BOOTSTRAPPED
    case QVariant::EasingCurve:
        v_clear<QEasingCurve>(d);
        break;
    case QVariant::JsonDocument:
        v_clear<QJsonDocument>(d);
        break;
#endif
    case QVariant::LongLong:
    case QVariant::ULongLong:
    case QVariant::Double:
    case QVariant::Float:
    case QMetaType::QObjectStar:
        break;
    case QVariant::Invalid:
    case QVariant::UserType:
    case QVariant::Int:
    case QVariant::UInt:
    case QVariant::Bool:
        break;
    default:
        QMetaType::destroy(d->type, d->data.shared->ptr);
        delete d->data.shared;
        break;
    }

    d->type = QVariant::Invalid;
    d->is_null = true;
    d->is_shared = false;
}

static bool isNull(const QVariant::Private *d)
{
    switch(d->type) {
    case QVariant::String:
        return v_cast<QString>(d)->isNull();
    case QVariant::Char:
        return v_cast<QChar>(d)->isNull();
    case QVariant::Date:
        return v_cast<QDate>(d)->isNull();
    case QVariant::Time:
        return v_cast<QTime>(d)->isNull();
    case QVariant::DateTime:
        return v_cast<QDateTime>(d)->isNull();
    case QVariant::ByteArray:
        return v_cast<QByteArray>(d)->isNull();
    case QVariant::BitArray:
        return v_cast<QBitArray>(d)->isNull();
#ifndef QT_BOOTSTRAPPED
    case QVariant::Size:
        return v_cast<QSize>(d)->isNull();
    case QVariant::SizeF:
        return v_cast<QSizeF>(d)->isNull();
    case QVariant::Rect:
        return v_cast<QRect>(d)->isNull();
    case QVariant::Line:
        return v_cast<QLine>(d)->isNull();
    case QVariant::LineF:
        return v_cast<QLineF>(d)->isNull();
    case QVariant::RectF:
        return v_cast<QRectF>(d)->isNull();
    case QVariant::Point:
        return v_cast<QPoint>(d)->isNull();
    case QVariant::PointF:
        return v_cast<QPointF>(d)->isNull();
    case QVariant::JsonDocument:
        return v_cast<QJsonDocument>(d)->isNull();
    case QVariant::EasingCurve:
    case QVariant::Url:
#endif
    case QVariant::Locale:
    case QVariant::RegExp:
    case QVariant::StringList:
    case QVariant::Map:
    case QVariant::Hash:
    case QVariant::List:
    case QVariant::Invalid:
    case QVariant::UserType:
    case QVariant::Int:
    case QVariant::UInt:
    case QVariant::LongLong:
    case QVariant::ULongLong:
    case QVariant::Bool:
    case QVariant::Double:
    case QVariant::Float:
    case QMetaType::QObjectStar:
        break;
    }
    return d->is_null;
}

/*
  \internal
  \since 4.4

  We cannot use v_cast() for QMetaType's numeric types because they're smaller than QVariant::Private::Data,
  which in turns makes v_cast() believe the value is stored in d->data.ptr. But
  it's not, since we're a QMetaType type.
 */
template<typename T>
inline bool compareNumericMetaType(const QVariant::Private *const a, const QVariant::Private *const b)
{
    return *static_cast<const T *>(a->data.shared->ptr) == *static_cast<const T *>(b->data.shared->ptr);
}

/*!
  \internal

  Compares \a a to \a b. The caller guarantees that \a a and \a b
  are of the same type.
 */
static bool compare(const QVariant::Private *a, const QVariant::Private *b)
{
    switch(a->type) {
    case QVariant::List:
        return *v_cast<QVariantList>(a) == *v_cast<QVariantList>(b);
    case QVariant::Map: {
        const QVariantMap *m1 = v_cast<QVariantMap>(a);
        const QVariantMap *m2 = v_cast<QVariantMap>(b);
        if (m1->count() != m2->count())
            return false;
        QVariantMap::ConstIterator it = m1->constBegin();
        QVariantMap::ConstIterator it2 = m2->constBegin();
        while (it != m1->constEnd()) {
            if (*it != *it2 || it.key() != it2.key())
                return false;
            ++it;
            ++it2;
        }
        return true;
    }
    case QVariant::Hash:
        return *v_cast<QVariantHash>(a) == *v_cast<QVariantHash>(b);
    case QVariant::String:
        return *v_cast<QString>(a) == *v_cast<QString>(b);
    case QVariant::Char:
        return *v_cast<QChar>(a) == *v_cast<QChar>(b);
    case QVariant::StringList:
        return *v_cast<QStringList>(a) == *v_cast<QStringList>(b);
#ifndef QT_BOOTSTRAPPED
    case QVariant::Size:
        return *v_cast<QSize>(a) == *v_cast<QSize>(b);
    case QVariant::SizeF:
        return *v_cast<QSizeF>(a) == *v_cast<QSizeF>(b);
    case QVariant::Rect:
        return *v_cast<QRect>(a) == *v_cast<QRect>(b);
    case QVariant::Line:
        return *v_cast<QLine>(a) == *v_cast<QLine>(b);
    case QVariant::LineF:
        return *v_cast<QLineF>(a) == *v_cast<QLineF>(b);
    case QVariant::RectF:
        return *v_cast<QRectF>(a) == *v_cast<QRectF>(b);
    case QVariant::Point:
        return *v_cast<QPoint>(a) == *v_cast<QPoint>(b);
    case QVariant::PointF:
        return *v_cast<QPointF>(a) == *v_cast<QPointF>(b);
    case QVariant::Url:
        return *v_cast<QUrl>(a) == *v_cast<QUrl>(b);
#endif
    case QVariant::Locale:
        return *v_cast<QLocale>(a) == *v_cast<QLocale>(b);
#ifndef QT_NO_REGEXP
    case QVariant::RegExp:
        return *v_cast<QRegExp>(a) == *v_cast<QRegExp>(b);
#endif
    case QVariant::Int:
        return a->data.i == b->data.i;
    case QVariant::UInt:
        return a->data.u == b->data.u;
    case QVariant::LongLong:
        return a->data.ll == b->data.ll;
    case QVariant::ULongLong:
        return a->data.ull == b->data.ull;
    case QVariant::Bool:
        return a->data.b == b->data.b;
    case QVariant::Double:
        return a->data.d == b->data.d;
    case QVariant::Float:
        return a->data.f == b->data.f;
    case QMetaType::QObjectStar:
        return a->data.o == b->data.o;
    case QVariant::Date:
        return *v_cast<QDate>(a) == *v_cast<QDate>(b);
    case QVariant::Time:
        return *v_cast<QTime>(a) == *v_cast<QTime>(b);
    case QVariant::DateTime:
        return *v_cast<QDateTime>(a) == *v_cast<QDateTime>(b);
#ifndef QT_BOOTSTRAPPED
    case QVariant::EasingCurve:
        return *v_cast<QEasingCurve>(a) == *v_cast<QEasingCurve>(b);
    case QVariant::JsonDocument:
        return *v_cast<QJsonDocument>(a) == *v_cast<QJsonDocument>(b);
#endif
    case QVariant::ByteArray:
        return *v_cast<QByteArray>(a) == *v_cast<QByteArray>(b);
    case QVariant::BitArray:
        return *v_cast<QBitArray>(a) == *v_cast<QBitArray>(b);
    case QVariant::Invalid:
        return true;
    case QMetaType::Long:
        return compareNumericMetaType<long>(a, b);
    case QMetaType::ULong:
        return compareNumericMetaType<ulong>(a, b);
    case QMetaType::Short:
        return compareNumericMetaType<short>(a, b);
    case QMetaType::UShort:
        return compareNumericMetaType<ushort>(a, b);
    case QMetaType::UChar:
        return compareNumericMetaType<uchar>(a, b);
    case QMetaType::Char:
        return compareNumericMetaType<char>(a, b);
    default:
        break;
    }
    if (!QMetaType::isRegistered(a->type))
        qFatal("QVariant::compare: type %d unknown to QVariant.", a->type);

    const void *a_ptr = a->is_shared ? a->data.shared->ptr : &(a->data.ptr);
    const void *b_ptr = b->is_shared ? b->data.shared->ptr : &(b->data.ptr);

    /* The reason we cannot place this test in a case branch above for the types
     * QMetaType::VoidStar, QMetaType::QObjectStar and so forth, as that wouldn't include
     * user defined pointer types. */
    const char *const typeName = QMetaType::typeName(a->type);
    uint typeNameLen = qstrlen(typeName);
    if (typeNameLen > 0 && typeName[typeNameLen - 1] == '*')
        return *static_cast<void *const *>(a_ptr) == *static_cast<void *const *>(b_ptr);

    if (a->is_null && b->is_null)
        return true;

    return a_ptr == b_ptr;
}

/*!
  \internal
 */
static qlonglong qMetaTypeNumber(const QVariant::Private *d)
{
    switch (d->type) {
    case QMetaType::Int:
        return d->data.i;
    case QMetaType::LongLong:
        return d->data.ll;
    case QMetaType::Char:
        return qlonglong(*static_cast<signed char *>(d->data.shared->ptr));
    case QMetaType::Short:
        return qlonglong(*static_cast<short *>(d->data.shared->ptr));
    case QMetaType::Long:
        return qlonglong(*static_cast<long *>(d->data.shared->ptr));
    case QVariant::Float:
        return qRound64(d->data.f);
    case QVariant::Double:
        return qRound64(d->data.d);
    }
    Q_ASSERT(false);
    return 0;
}

static qulonglong qMetaTypeUNumber(const QVariant::Private *d)
{
    switch (d->type) {
    case QVariant::UInt:
        return d->data.u;
    case QVariant::ULongLong:
        return d->data.ull;
    case QMetaType::UChar:
        return qulonglong(*static_cast<unsigned char *>(d->data.shared->ptr));
    case QMetaType::UShort:
        return qulonglong(*static_cast<ushort *>(d->data.shared->ptr));
    case QMetaType::ULong:
        return qulonglong(*static_cast<ulong *>(d->data.shared->ptr));
    }
    Q_ASSERT(false);
    return 0;
}

static qlonglong qConvertToNumber(const QVariant::Private *d, bool *ok)
{
    *ok = true;

    switch (d->type) {
    case QVariant::String:
        return v_cast<QString>(d)->toLongLong(ok);
    case QVariant::Char:
        return v_cast<QChar>(d)->unicode();
    case QVariant::ByteArray:
        return v_cast<QByteArray>(d)->toLongLong(ok);
    case QVariant::Bool:
        return qlonglong(d->data.b);
    case QVariant::Double:
    case QVariant::Int:
    case QMetaType::Char:
    case QMetaType::Short:
    case QMetaType::Long:
    case QVariant::Float:
    case QMetaType::LongLong:
        return qMetaTypeNumber(d);
    case QVariant::ULongLong:
    case QVariant::UInt:
    case QMetaType::UChar:
    case QMetaType::UShort:
    case QMetaType::ULong:
        return qlonglong(qMetaTypeUNumber(d));
    }

    *ok = false;
    return Q_INT64_C(0);
}

static qulonglong qConvertToUnsignedNumber(const QVariant::Private *d, bool *ok)
{
    *ok = true;

    switch (d->type) {
    case QVariant::String:
        return v_cast<QString>(d)->toULongLong(ok);
    case QVariant::Char:
        return v_cast<QChar>(d)->unicode();
    case QVariant::ByteArray:
        return v_cast<QByteArray>(d)->toULongLong(ok);
    case QVariant::Bool:
        return qulonglong(d->data.b);
    case QVariant::Double:
    case QVariant::Int:
    case QMetaType::Char:
    case QMetaType::Short:
    case QMetaType::Long:
    case QVariant::Float:
    case QMetaType::LongLong:
        return qulonglong(qMetaTypeNumber(d));
    case QVariant::ULongLong:
    case QVariant::UInt:
    case QMetaType::UChar:
    case QMetaType::UShort:
    case QMetaType::ULong:
        return qMetaTypeUNumber(d);
    }

    *ok = false;
    return Q_UINT64_C(0);
}

/*!
 \internal

 Converts \a d to type \a t, which is placed in \a result.
 */
static bool convert(const QVariant::Private *d, int t, void *result, bool *ok)
{
    Q_ASSERT(d->type != t);
    Q_ASSERT(result);

    bool dummy;
    if (!ok)
        ok = &dummy;

    switch (t) {
#ifndef QT_BOOTSTRAPPED
    case QVariant::Url:
        switch (d->type) {
        case QVariant::String:
            *static_cast<QUrl *>(result) = QUrl(*v_cast<QString>(d));
            return true;
        default:
            return false;
        }
#endif
    case QVariant::String: {
        QString *str = static_cast<QString *>(result);
        switch (d->type) {
        case QVariant::Char:
            *str = QString(*v_cast<QChar>(d));
            return true;
        case QMetaType::Char:
        case QMetaType::UChar:
            *str = QChar::fromAscii(*static_cast<char *>(d->data.shared->ptr));
            return true;
        case QMetaType::Short:
        case QMetaType::Long:
        case QVariant::Int:
        case QVariant::LongLong:
            *str = QString::number(qMetaTypeNumber(d));
            return true;
        case QVariant::UInt:
        case QVariant::ULongLong:
        case QMetaType::UShort:
        case QMetaType::ULong:
            *str = QString::number(qMetaTypeUNumber(d));
            return true;
        case QVariant::Float:
            *str = QString::number(d->data.f, 'g', FLT_DIG);
            return true;
        case QVariant::Double:
            *str = QString::number(d->data.d, 'g', DBL_DIG);
            return true;
#if !defined(QT_NO_DATESTRING)
        case QVariant::Date:
            *str = v_cast<QDate>(d)->toString(Qt::ISODate);
            return true;
        case QVariant::Time:
            *str = v_cast<QTime>(d)->toString(Qt::ISODate);
            return true;
        case QVariant::DateTime:
            *str = v_cast<QDateTime>(d)->toString(Qt::ISODate);
            return true;
#endif
        case QVariant::Bool:
            *str = QLatin1String(d->data.b ? "true" : "false");
            return true;
        case QVariant::ByteArray:
            *str = QString::fromAscii(v_cast<QByteArray>(d)->constData());
            return true;
        case QVariant::StringList:
            *str = v_cast<QStringList>(d)->join(qStringListDelim);
            return true;
#ifndef QT_BOOTSTRAPPED
        case QVariant::Url:
            *str = v_cast<QUrl>(d)->toString();
            return true;
#endif
        default:
            return false;
        }
    }
    case QVariant::Char: {
        switch (d->type) {
        case QVariant::Int:
        case QVariant::LongLong:
        case QMetaType::Char:
        case QMetaType::Short:
        case QMetaType::Long:
        case QVariant::Float:
            *static_cast<QChar *>(result) = QChar(ushort(qMetaTypeNumber(d)));
            return true;
        case QVariant::UInt:
        case QVariant::ULongLong:
        case QMetaType::UChar:
        case QMetaType::UShort:
        case QMetaType::ULong:
            *static_cast<QChar *>(result) = QChar(ushort(qMetaTypeUNumber(d)));
            return true;
        default:
            return false;
        }
    }
#ifndef QT_BOOTSTRAPPED
    case QVariant::Size: {
        switch (d->type) {
        case QVariant::SizeF:
            *static_cast<QSize *>(result) = v_cast<QSizeF>(d)->toSize();
            return true;
        default:
            return false;
        }
    }

    case QVariant::SizeF: {
        switch (d->type) {
        case QVariant::Size:
            *static_cast<QSizeF *>(result) = QSizeF(*(v_cast<QSize>(d)));
            return true;
        default:
            return false;
        }
    }

    case QVariant::Line: {
        switch (d->type) {
        case QVariant::LineF:
            *static_cast<QLine *>(result) = v_cast<QLineF>(d)->toLine();
            return true;
        default:
            return false;
        }
    }

    case QVariant::LineF: {
        switch (d->type) {
        case QVariant::Line:
            *static_cast<QLineF *>(result) = QLineF(*(v_cast<QLine>(d)));
            return true;
        default:
            return false;
        }
    }
#endif
    case QVariant::StringList:
        switch (d->type) {
        case QVariant::List: {
            QStringList *slst = static_cast<QStringList *>(result);
            const QVariantList *list = v_cast<QVariantList >(d);
            for (int i = 0; i < list->size(); ++i)
                slst->append(list->at(i).toString());
            return true;
        }
        case QVariant::String: {
            QStringList *slst = static_cast<QStringList *>(result);
            *slst = v_cast<QString>(d)->split(qStringListDelim);
            return true;
        }
        default:
            return false;
        }
    case QVariant::Date: {
        QDate *dt = static_cast<QDate *>(result);
        switch (d->type) {
        case QVariant::DateTime:
            *dt = v_cast<QDateTime>(d)->date();
            return dt->isValid();
#ifndef QT_NO_DATESTRING
        case QVariant::String:
            *dt = QDate::fromString(*v_cast<QString>(d), Qt::ISODate);
            return dt->isValid();
#endif
        default:
            return false;
        }
    }
    case QVariant::Time: {
        QTime *tt = static_cast<QTime *>(result);
        switch (d->type) {
        case QVariant::DateTime:
            *tt = v_cast<QDateTime>(d)->time();
            return tt->isValid();
#ifndef QT_NO_DATESTRING
        case QVariant::String:
            *tt = QTime::fromString(*v_cast<QString>(d), Qt::ISODate);
            return tt->isValid();
#endif
        default:
            return false;
        }
    }
    case QVariant::DateTime: {
        QDateTime *dt = static_cast<QDateTime *>(result);
        switch (d->type) {
#ifndef QT_NO_DATESTRING
        case QVariant::String:
            *dt = QDateTime::fromString(*v_cast<QString>(d), Qt::ISODate);
            return dt->isValid();
#endif
        case QVariant::Date:
            *dt = QDateTime(*v_cast<QDate>(d));
            return dt->isValid();
        default:
            return false;
        }
    }
    case QVariant::ByteArray: {
        switch (d->type) {
        case QVariant::String:
            *static_cast<QByteArray *>(result) = v_cast<QString>(d)->toAscii();
            return true;
        case QVariant::Double:
            *static_cast<QByteArray *>(result) = QByteArray::number(d->data.d, 'g', DBL_DIG);
            return true;
        case QVariant::Float:
            *static_cast<QByteArray *>(result) = QByteArray::number(d->data.f, 'g', FLT_DIG);
            return true;
        case QMetaType::Char:
        case QMetaType::UChar:
            *static_cast<QByteArray *>(result) = QByteArray(1, *static_cast<char *>(d->data.shared->ptr));
            return true;
        case QVariant::Int:
        case QVariant::LongLong:
        case QMetaType::Short:
        case QMetaType::Long:
            *static_cast<QByteArray *>(result) = QByteArray::number(qMetaTypeNumber(d));
            return true;
        case QVariant::UInt:
        case QVariant::ULongLong:
        case QMetaType::UShort:
        case QMetaType::ULong:
            *static_cast<QByteArray *>(result) = QByteArray::number(qMetaTypeUNumber(d));
            return true;
        case QVariant::Bool:
            *static_cast<QByteArray *>(result) = QByteArray(d->data.b ? "true" : "false");
            return true;
        default:
            return false;
        }
    }
    case QMetaType::Short:
        *static_cast<short *>(result) = short(qConvertToNumber(d, ok));
        return *ok;
    case QMetaType::Long:
        *static_cast<long *>(result) = long(qConvertToNumber(d, ok));
        return *ok;
    case QMetaType::UShort:
        *static_cast<ushort *>(result) = ushort(qConvertToUnsignedNumber(d, ok));
        return *ok;
    case QMetaType::ULong:
        *static_cast<ulong *>(result) = ulong(qConvertToUnsignedNumber(d, ok));
        return *ok;
    case QVariant::Int:
        *static_cast<int *>(result) = int(qConvertToNumber(d, ok));
        return *ok;
    case QVariant::UInt:
        *static_cast<uint *>(result) = uint(qConvertToUnsignedNumber(d, ok));
        return *ok;
    case QVariant::LongLong:
        *static_cast<qlonglong *>(result) = qConvertToNumber(d, ok);
        return *ok;
    case QVariant::ULongLong: {
        *static_cast<qulonglong *>(result) = qConvertToUnsignedNumber(d, ok);
        return *ok;
    }
    case QMetaType::UChar: {
        *static_cast<uchar *>(result) = qConvertToUnsignedNumber(d, ok);
        return *ok;
    }
    case QVariant::Bool: {
        *static_cast<bool *>(result) = false;
        switch(d->type) {
        case QVariant::ByteArray: {
            QByteArray str = v_cast<QByteArray>(d)->toLower();
            *static_cast<bool *>(result) = (!str.isEmpty() && str != "0" && str != "false");
            return true;
        }
        case QVariant::String: {
            QString str = v_cast<QString>(d)->toLower();
            *static_cast<bool *>(result) = (!str.isEmpty() && str != QLatin1String("0") && str != QLatin1String("false"));
            return true;
        }
        case QVariant::Char:
            *static_cast<bool *>(result) = !v_cast<QChar>(d)->isNull();
            return true;
        case QVariant::Double:
        case QVariant::Int:
        case QVariant::LongLong:
        case QMetaType::Char:
        case QMetaType::Short:
        case QMetaType::Long:
        case QVariant::Float:
            *static_cast<bool *>(result) = (qMetaTypeNumber(d) != Q_INT64_C(0));
            return true;
        case QVariant::UInt:
        case QVariant::ULongLong:
        case QMetaType::UChar:
        case QMetaType::UShort:
        case QMetaType::ULong:
            *static_cast<bool *>(result) = qMetaTypeUNumber(d) != Q_UINT64_C(0);
            return true;
        default:
            return false;
        }
    }
    case QVariant::Double: {
        *static_cast<double *>(result) = 0.0;
        switch (d->type) {
        case QVariant::String:
            *static_cast<double *>(result) = v_cast<QString>(d)->toDouble(ok);
            return *ok;
        case QVariant::ByteArray:
            *static_cast<double *>(result) = v_cast<QByteArray>(d)->toDouble(ok);
            return *ok;
        case QVariant::Bool:
            *static_cast<double *>(result) = double(d->data.b);
            return true;
        case QVariant::Float:
            *static_cast<double *>(result) = double(d->data.f);
            return true;
        case QVariant::LongLong:
        case QVariant::Int:
        case QMetaType::Char:
        case QMetaType::Short:
        case QMetaType::Long:
            *static_cast<double *>(result) = double(qMetaTypeNumber(d));
            return true;
        case QVariant::UInt:
        case QVariant::ULongLong:
        case QMetaType::UChar:
        case QMetaType::UShort:
        case QMetaType::ULong:
            *static_cast<double *>(result) = double(qMetaTypeUNumber(d));
            return true;
        default:
            return false;
        }
    }
    case QVariant::Float: {
        *static_cast<float *>(result) = 0.0f;
        switch (d->type) {
        case QVariant::String:
            *static_cast<float *>(result) = v_cast<QString>(d)->toFloat(ok);
            return *ok;
        case QVariant::ByteArray:
            *static_cast<float *>(result) = v_cast<QByteArray>(d)->toFloat(ok);
            return *ok;
        case QVariant::Bool:
            *static_cast<float *>(result) = float(d->data.b);
            return true;
        case QVariant::Double:
            *static_cast<float *>(result) = float(d->data.d);
            return true;
        case QVariant::LongLong:
        case QVariant::Int:
        case QMetaType::Char:
        case QMetaType::Short:
        case QMetaType::Long:
            *static_cast<float *>(result) = float(qMetaTypeNumber(d));
            return true;
        case QVariant::UInt:
        case QVariant::ULongLong:
        case QMetaType::UChar:
        case QMetaType::UShort:
        case QMetaType::ULong:
            *static_cast<float *>(result) = float(qMetaTypeUNumber(d));
            return true;
        default:
            return false;
        }
    }
    case QVariant::List:
        if (d->type == QVariant::StringList) {
            QVariantList *lst = static_cast<QVariantList *>(result);
            const QStringList *slist = v_cast<QStringList>(d);
            for (int i = 0; i < slist->size(); ++i)
                lst->append(QVariant(slist->at(i)));
            return true;
        } else if (d->type == QVariant::List) {
            *static_cast<QVariantList *>(result) = *static_cast<QList<QVariant> *>(d->data.shared->ptr);
            return true;
        }
        return false;
    case QVariant::Map:
        if (d->type == QVariant::Map) {
            *static_cast<QVariantMap *>(result) = *static_cast<QMap<QString, QVariant> *>(d->data.shared->ptr);
            return true;
        }
        return false;
    case QVariant::Hash:
        if (d->type == QVariant::Hash) {
            *static_cast<QVariantHash *>(result) = *static_cast<QHash<QString, QVariant> *>(d->data.shared->ptr);
            return true;
        }
        return false;
#ifndef QT_BOOTSTRAPPED
    case QVariant::Rect:
        if (d->type == QVariant::RectF) {
            *static_cast<QRect *>(result) = (v_cast<QRectF>(d))->toRect();
            return true;
        }
        return false;
    case QVariant::RectF:
        if (d->type == QVariant::Rect) {
            *static_cast<QRectF *>(result) = *v_cast<QRect>(d);
            return true;
        }
        return false;
    case QVariant::PointF:
        if (d->type == QVariant::Point) {
            *static_cast<QPointF *>(result) = *v_cast<QPoint>(d);
            return true;
        }
        return false;
    case QVariant::Point:
        if (d->type == QVariant::PointF) {
            *static_cast<QPoint *>(result) = (v_cast<QPointF>(d))->toPoint();
            return true;
        }
        return false;
    case QMetaType::Char:
    {
        *static_cast<qint8 *>(result) = qint8(qConvertToNumber(d, ok));
        return *ok;
    }
#endif
    default:
        return false;
    }
    return false;
}

#if !defined(QT_NO_DEBUG_STREAM) && !defined(Q_BROKEN_DEBUG_STREAM)
static void streamDebug(QDebug dbg, const QVariant &v)
{
    switch (v.userType()) {
    case QVariant::Int:
        dbg.nospace() << v.toInt();
        break;
    case QVariant::UInt:
        dbg.nospace() << v.toUInt();
        break;
    case QVariant::LongLong:
        dbg.nospace() << v.toLongLong();
        break;
    case QVariant::ULongLong:
        dbg.nospace() << v.toULongLong();
        break;
    case QVariant::Float:
        dbg.nospace() << v.toFloat();
        break;
    case QMetaType::QObjectStar:
        dbg.nospace() << qvariant_cast<QObject *>(v);
        break;
    case QVariant::Double:
        dbg.nospace() << v.toDouble();
        break;
    case QVariant::Bool:
        dbg.nospace() << v.toBool();
        break;
    case QVariant::String:
        dbg.nospace() << v.toString();
        break;
    case QVariant::Char:
        dbg.nospace() << v.toChar();
        break;
    case QVariant::StringList:
        dbg.nospace() << v.toStringList();
        break;
    case QVariant::Map:
        dbg.nospace() << v.toMap();
        break;
    case QVariant::Hash:
        dbg.nospace() << v.toHash();
        break;
    case QVariant::List:
        dbg.nospace() << v.toList();
        break;
    case QVariant::Date:
        dbg.nospace() << v.toDate();
        break;
    case QVariant::Time:
        dbg.nospace() << v.toTime();
        break;
    case QVariant::DateTime:
        dbg.nospace() << v.toDateTime();
        break;
#ifndef QT_BOOTSTRAPPED
    case QVariant::EasingCurve:
        dbg.nospace() << v.toEasingCurve();
        break;
    case QVariant::JsonDocument:
        dbg.nospace() << v.toJsonDocument();
        break;
#endif
    case QVariant::ByteArray:
        dbg.nospace() << v.toByteArray();
        break;
#ifndef QT_BOOTSTRAPPED
    case QVariant::Url:
        dbg.nospace() << v.toUrl();
        break;
    case QVariant::Point:
        dbg.nospace() << v.toPoint();
        break;
    case QVariant::PointF:
        dbg.nospace() << v.toPointF();
        break;
    case QVariant::Rect:
        dbg.nospace() << v.toRect();
        break;
    case QVariant::Size:
        dbg.nospace() << v.toSize();
        break;
    case QVariant::SizeF:
        dbg.nospace() << v.toSizeF();
        break;
    case QVariant::Line:
        dbg.nospace() << v.toLine();
        break;
    case QVariant::LineF:
        dbg.nospace() << v.toLineF();
        break;
    case QVariant::RectF:
        dbg.nospace() << v.toRectF();
        break;
#endif
    case QVariant::BitArray:
        dbg.nospace() << v.toBitArray();
        break;
    default:
        break;
    }
}
#endif

const QVariant::Handler qt_kernel_variant_handler = {
    construct,
    clear,
    isNull,
#ifndef QT_NO_DATASTREAM
    0,
    0,
#endif
    compare,
    convert,
#if !defined(QT_NO_DEBUG_STREAM) && !defined(Q_BROKEN_DEBUG_STREAM)
    streamDebug
#else
    0
#endif
};

Q_CORE_EXPORT const QVariant::Handler *qcoreVariantHandler()
{
    return &qt_kernel_variant_handler;
}


const QVariant::Handler *QVariant::handler = &qt_kernel_variant_handler;

/*!
    \class QVariant
    \brief The QVariant class acts like a union for the most common Qt data types.

    \ingroup objectmodel
    \ingroup shared


    Because C++ forbids unions from including types that have
    non-default constructors or destructors, most interesting Qt
    classes cannot be used in unions. Without QVariant, this would be
    a problem for QObject::property() and for database work, etc.

    A QVariant object holds a single value of a single type() at a
    time. (Some type()s are multi-valued, for example a string list.)
    You can find out what type, T, the variant holds, convert it to a
    different type using convert(), get its value using one of the
    toT() functions (e.g., toSize()) and check whether the type can
    be converted to a particular type using canConvert().

    The methods named toT() (e.g., toInt(), toString()) are const. If
    you ask for the stored type, they return a copy of the stored
    object. If you ask for a type that can be generated from the
    stored type, toT() copies and converts and leaves the object
    itself unchanged. If you ask for a type that cannot be generated
    from the stored type, the result depends on the type; see the
    function documentation for details.

    Here is some example code to demonstrate the use of QVariant:

    \snippet doc/src/snippets/code/src_corelib_kernel_qvariant.cpp 0

    You can even store QList<QVariant> and QMap<QString, QVariant>
    values in a variant, so you can easily construct arbitrarily
    complex data structures of arbitrary types. This is very powerful
    and versatile, but may prove less memory and speed efficient than
    storing specific types in standard data structures.

    QVariant also supports the notion of null values, where you can
    have a defined type with no value set. However, note that QVariant
    types can only be cast when they have had a value set.

    \snippet doc/src/snippets/code/src_corelib_kernel_qvariant.cpp 1

    QVariant can be extended to support other types than those
    mentioned in the \l Type enum. See the \l QMetaType documentation
    for details.

    \section1 A Note on GUI Types

    Because QVariant is part of the QtCore library, it cannot provide
    conversion functions to data types defined in QtGui, such as
    QColor, QImage, and QPixmap. In other words, there is no \c
    toColor() function. Instead, you can use the QVariant::value() or
    the qvariant_cast() template function. For example:

    \snippet doc/src/snippets/code/src_corelib_kernel_qvariant.cpp 2

    The inverse conversion (e.g., from QColor to QVariant) is
    automatic for all data types supported by QVariant, including
    GUI-related types:

    \snippet doc/src/snippets/code/src_corelib_kernel_qvariant.cpp 3

    \section1 Using canConvert() and convert() Consecutively

    When using canConvert() and convert() consecutively, it is possible for
    canConvert() to return true, but convert() to return false. This
    is typically because canConvert() only reports the general ability of
    QVariant to convert between types given suitable data; it is still
    possible to supply data which cannot actually be converted.

    For example, canConvert() would return true when called on a variant
    containing a string because, in principle, QVariant is able to convert
    strings of numbers to integers.
    However, if the string contains non-numeric characters, it cannot be
    converted to an integer, and any attempt to convert it will fail.
    Hence, it is important to have both functions return true for a
    successful conversion.

    \sa QMetaType
*/

/*!
    \enum QVariant::Type

    This enum type defines the types of variable that a QVariant can
    contain.

    \value Invalid  no type
    \value BitArray  a QBitArray
    \value Bitmap  a QBitmap
    \value Bool  a bool
    \value Brush  a QBrush
    \value ByteArray  a QByteArray
    \value Char  a QChar
    \value Color  a QColor
    \value Cursor  a QCursor
    \value Date  a QDate
    \value DateTime  a QDateTime
    \value Double  a double
    \value EasingCurve a QEasingCurve
    \value QJsonDocument QJsonDocument
    \value Font  a QFont
    \value Hash a QVariantHash
    \value Icon  a QIcon
    \value Image  a QImage
    \value Int  an int
    \value KeySequence  a QKeySequence
    \value Line  a QLine
    \value LineF  a QLineF
    \value List  a QVariantList
    \value Locale  a QLocale
    \value LongLong a \l qlonglong
    \value Map  a QVariantMap
    \value Matrix  a QMatrix
    \value Transform  a QTransform
    \value Matrix4x4  a QMatrix4x4
    \value Palette  a QPalette
    \value Pen  a QPen
    \value Pixmap  a QPixmap
    \value Point  a QPoint
    \value PointArray  a QPointArray
    \value PointF  a QPointF
    \value Polygon a QPolygon
    \value Quaternion  a QQuaternion
    \value Rect  a QRect
    \value RectF  a QRectF
    \value RegExp  a QRegExp
    \value Region  a QRegion
    \value Size  a QSize
    \value SizeF  a QSizeF
    \value SizePolicy  a QSizePolicy
    \value String  a QString
    \value StringList  a QStringList
    \value TextFormat  a QTextFormat
    \value TextLength  a QTextLength
    \value Time  a QTime
    \value UInt  a \l uint
    \value ULongLong a \l qulonglong
    \value Url  a QUrl
    \value Vector2D  a QVector2D
    \value Vector3D  a QVector3D
    \value Vector4D  a QVector4D

    \value UserType Base value for user-defined types.

    \omitvalue LastGuiType
    \omitvalue LastCoreType
    \omitvalue LastType
*/

/*!
    \fn QVariant::QVariant()

    Constructs an invalid variant.
*/


/*!
    \fn QVariant::QVariant(int typeOrUserType, const void *copy)

    Constructs variant of type \a typeOrUserType, and initializes with
    \a copy if \a copy is not 0.

    Note that you have to pass the address of the variable you want stored.

    Usually, you never have to use this constructor, use QVariant::fromValue()
    instead to construct variants from the pointer types represented by
    \c QMetaType::VoidStar, \c QMetaType::QObjectStar and
    \c QMetaType::QWidgetStar.

    \sa QVariant::fromValue(), Type
*/

/*!
    \fn QVariant::QVariant(Type type)

    Constructs a null variant of type \a type.
*/



/*!
    \fn QVariant::create(int type, const void *copy)

    \internal

    Constructs a variant private of type \a type, and initializes with \a copy if
    \a copy is not 0.
*/

void QVariant::create(int type, const void *copy)
{
    d.type = type;
    handler->construct(&d, copy);
}

/*!
    \fn QVariant::~QVariant()

    Destroys the QVariant and the contained object.

    Note that subclasses that reimplement clear() should reimplement
    the destructor to call clear(). This destructor calls clear(), but
    because it is the destructor, QVariant::clear() is called rather
    than a subclass's clear().
*/

QVariant::~QVariant()
{
    if ((d.is_shared && !d.data.shared->ref.deref()) || (!d.is_shared && d.type > Char && d.type < UserType))
        handler->clear(&d);
}

/*!
  \fn QVariant::QVariant(const QVariant &p)

    Constructs a copy of the variant, \a p, passed as the argument to
    this constructor.
*/

QVariant::QVariant(const QVariant &p)
    : d(p.d)
{
    if (d.is_shared) {
        d.data.shared->ref.ref();
    } else if (p.d.type > Char && p.d.type < QVariant::UserType) {
        handler->construct(&d, p.constData());
        d.is_null = p.d.is_null;
    }
}

#ifndef QT_NO_DATASTREAM
/*!
    Reads the variant from the data stream, \a s.
*/
QVariant::QVariant(QDataStream &s)
{
    d.is_null = true;
    s >> *this;
}
#endif //QT_NO_DATASTREAM

/*!
  \fn QVariant::QVariant(const QString &val)

    Constructs a new variant with a string value, \a val.
*/

/*!
  \fn QVariant::QVariant(const QLatin1String &val)

    Constructs a new variant with a string value, \a val.
*/

/*!
  \fn QVariant::QVariant(const char *val)

    Constructs a new variant with a string value of \a val.
    The variant creates a deep copy of \a val, using the encoding
    set by QTextCodec::setCodecForCStrings().

    Note that \a val is converted to a QString for storing in the
    variant and QVariant::type() will return QMetaType::QString for
    the variant.

    You can disable this operator by defining \c
    QT_NO_CAST_FROM_ASCII when you compile your applications.

    \sa QTextCodec::setCodecForCStrings()
*/

#ifndef QT_NO_CAST_FROM_ASCII
QVariant::QVariant(const char *val)
{
    QString s = QString::fromAscii(val);
    create(String, &s);
}
#endif

/*!
  \fn QVariant::QVariant(const QStringList &val)

    Constructs a new variant with a string list value, \a val.
*/

/*!
  \fn QVariant::QVariant(const QMap<QString, QVariant> &val)

    Constructs a new variant with a map of QVariants, \a val.
*/

/*!
  \fn QVariant::QVariant(const QHash<QString, QVariant> &val)

    Constructs a new variant with a hash of QVariants, \a val.
*/

/*!
  \fn QVariant::QVariant(const QDate &val)

    Constructs a new variant with a date value, \a val.
*/

/*!
  \fn QVariant::QVariant(const QTime &val)

    Constructs a new variant with a time value, \a val.
*/

/*!
  \fn QVariant::QVariant(const QDateTime &val)

    Constructs a new variant with a date/time value, \a val.
*/

/*!
    \since 4.9
    \fn QVariant::QVariant(const QJsonDocument &val)

    Constructs a new variant with a json document value, \a val.
*/

/*!
    \since 4.7
  \fn QVariant::QVariant(const QEasingCurve &val)

    Constructs a new variant with an easing curve value, \a val.
*/

/*!
  \fn QVariant::QVariant(const QByteArray &val)

    Constructs a new variant with a bytearray value, \a val.
*/

/*!
  \fn QVariant::QVariant(const QBitArray &val)

    Constructs a new variant with a bitarray value, \a val.
*/

/*!
  \fn QVariant::QVariant(const QPoint &val)

  Constructs a new variant with a point value of \a val.
 */

/*!
  \fn QVariant::QVariant(const QPointF &val)

  Constructs a new variant with a point value of \a val.
 */

/*!
  \fn QVariant::QVariant(const QRectF &val)

  Constructs a new variant with a rect value of \a val.
 */

/*!
  \fn QVariant::QVariant(const QLineF &val)

  Constructs a new variant with a line value of \a val.
 */

/*!
  \fn QVariant::QVariant(const QLine &val)

  Constructs a new variant with a line value of \a val.
 */

/*!
  \fn QVariant::QVariant(const QRect &val)

  Constructs a new variant with a rect value of \a val.
 */

/*!
  \fn QVariant::QVariant(const QSize &val)

  Constructs a new variant with a size value of \a val.
 */

/*!
  \fn QVariant::QVariant(const QSizeF &val)

  Constructs a new variant with a size value of \a val.
 */

/*!
  \fn QVariant::QVariant(const QUrl &val)

  Constructs a new variant with a url value of \a val.
 */

/*!
  \fn QVariant::QVariant(int val)

    Constructs a new variant with an integer value, \a val.
*/

/*!
  \fn QVariant::QVariant(uint val)

    Constructs a new variant with an unsigned integer value, \a val.
*/

/*!
  \fn QVariant::QVariant(qlonglong val)

    Constructs a new variant with a long long integer value, \a val.
*/

/*!
  \fn QVariant::QVariant(qulonglong val)

    Constructs a new variant with an unsigned long long integer value, \a val.
*/


/*!
  \fn QVariant::QVariant(bool val)

    Constructs a new variant with a boolean value, \a val.
*/

/*!
  \fn QVariant::QVariant(double val)

    Constructs a new variant with a floating point value, \a val.
*/

/*!
  \fn QVariant::QVariant(float val)

    Constructs a new variant with a floating point value, \a val.
    \since 4.6
*/

/*!
    \fn QVariant::QVariant(const QList<QVariant> &val)

    Constructs a new variant with a list value, \a val.
*/

/*!
  \fn QVariant::QVariant(const QChar &c)

  Constructs a new variant with a char value, \a c.
*/

/*!
  \fn QVariant::QVariant(const QLocale &l)

  Constructs a new variant with a locale value, \a l.
*/

/*!
  \fn QVariant::QVariant(const QRegExp &regExp)

  Constructs a new variant with the regexp value \a regExp.
*/

QVariant::QVariant(Type type)
{ create(type, nullptr); }
QVariant::QVariant(int typeOrUserType, const void *copy)
{ create(typeOrUserType, copy); d.is_null = false; }

QVariant::QVariant(int val)
{ d.is_null = false; d.type = Int; d.data.i = val; }
QVariant::QVariant(uint val)
{ d.is_null = false; d.type = UInt; d.data.u = val; }
QVariant::QVariant(qlonglong val)
{ d.is_null = false; d.type = LongLong; d.data.ll = val; }
QVariant::QVariant(qulonglong val)
{ d.is_null = false; d.type = ULongLong; d.data.ull = val; }
QVariant::QVariant(bool val)
{ d.is_null = false; d.type = Bool; d.data.b = val; }
QVariant::QVariant(double val)
{ d.is_null = false; d.type = Double; d.data.d = val; }
QVariant::QVariant(float val)
{ d.is_null = false; d.type = Float; d.data.f = val; }

QVariant::QVariant(const QByteArray &val)
{ d.is_null = false; d.type = ByteArray; v_construct<QByteArray>(&d, val); }
QVariant::QVariant(const QBitArray &val)
{ d.is_null = false; d.type = BitArray; v_construct<QBitArray>(&d, val);  }
QVariant::QVariant(const QString &val)
{ d.is_null = false; d.type = String; v_construct<QString>(&d, val);  }
QVariant::QVariant(const QChar &val)
{ d.is_null = false; d.type = Char; v_construct<QChar>(&d, val);  }
QVariant::QVariant(const QLatin1String &val)
{ QString str(val); d.is_null = false; d.type = String; v_construct<QString>(&d, str); }
QVariant::QVariant(const QStringList &val)
{ d.is_null = false; d.type = StringList; v_construct<QStringList>(&d, val); }

QVariant::QVariant(const QDate &val)
{ d.is_null = false; d.type = Date; v_construct<QDate>(&d, val); }
QVariant::QVariant(const QTime &val)
{ d.is_null = false; d.type = Time; v_construct<QTime>(&d, val); }
QVariant::QVariant(const QDateTime &val)
{ d.is_null = false; d.type = DateTime; v_construct<QDateTime>(&d, val); }
#ifndef QT_BOOTSTRAPPED
QVariant::QVariant(const QEasingCurve &val)
{ d.is_null = false; d.type = EasingCurve; v_construct<QEasingCurve>(&d, val); }
QVariant::QVariant(const QJsonDocument &jsonDocument) { d.is_null = false; d.type = JsonDocument; v_construct<QJsonDocument>(&d, jsonDocument); }
#endif
QVariant::QVariant(const QList<QVariant> &list)
{ d.is_null = false; d.type = List; v_construct<QVariantList>(&d, list); }
QVariant::QVariant(const QMap<QString, QVariant> &map)
{ d.is_null = false; d.type = Map; v_construct<QVariantMap>(&d, map); }
QVariant::QVariant(const QHash<QString, QVariant> &hash)
{ d.is_null = false; d.type = Hash; v_construct<QVariantHash>(&d, hash); }
#ifndef QT_BOOTSTRAPPED
QVariant::QVariant(const QPoint &pt) { d.is_null = false; d.type = Point; v_construct<QPoint>(&d, pt); }
QVariant::QVariant(const QPointF &pt) { d.is_null = false; d.type = PointF; v_construct<QPointF>(&d, pt); }
QVariant::QVariant(const QRectF &r) { d.is_null = false; d.type = RectF; v_construct<QRectF>(&d, r); }
QVariant::QVariant(const QLineF &l) { d.is_null = false; d.type = LineF; v_construct<QLineF>(&d, l); }
QVariant::QVariant(const QLine &l) { d.is_null = false; d.type = Line; v_construct<QLine>(&d, l); }
QVariant::QVariant(const QRect &r) { d.is_null = false; d.type = Rect; v_construct<QRect>(&d, r); }
QVariant::QVariant(const QSize &s) { d.is_null = false; d.type = Size; v_construct<QSize>(&d, s); }
QVariant::QVariant(const QSizeF &s) { d.is_null = false; d.type = SizeF; v_construct<QSizeF>(&d, s); }
QVariant::QVariant(const QUrl &u) { d.is_null = false; d.type = Url; v_construct<QUrl>(&d, u); }
#endif
QVariant::QVariant(const QLocale &l) { d.is_null = false; d.type = Locale; v_construct<QLocale>(&d, l); }
#ifndef QT_NO_REGEXP
QVariant::QVariant(const QRegExp &regExp) { d.is_null = false; d.type = RegExp; v_construct<QRegExp>(&d, regExp); }
#endif

/*!
    Returns the storage type of the value stored in the variant.
    Although this function is declared as returning QVariant::Type,
    the return value should be interpreted as QMetaType::Type. In
    particular, QVariant::UserType is returned here only if the value
    is equal or greater than QMetaType::User.

    Note that return values in the ranges QVariant::Char through
    QVariant::RegExp and QVariant::Font through QVariant::Transform
    correspond to the values in the ranges QMetaType::QChar through
    QMetaType::QRegExp and QMetaType::QFont through QMetaType::QQuaternion.

    Pay particular attention when working with char and QChar
    variants.  Note that there is no QVariant constructor specifically
    for type char, but there is one for QChar. For a variant of type
    QChar, this function returns QVariant::Char, which is the same as
    QMetaType::QChar, but for a variant of type \c char, this function
    returns QMetaType::Char, which is \e not the same as
    QVariant::Char.

    Also note that the types \c void*, \c long, \c short, \c unsigned
    \c long, \c unsigned \c short, \c unsigned \c char, \c float, \c
    QObject*, and \c QWidget* are represented in QMetaType::Type but
    not in QVariant::Type, and they can be returned by this function.
    However, they are considered to be user defined types when tested
    against QVariant::Type.

    To test whether an instance of QVariant contains a data type that
    is compatible with the data type you are interested in, use
    canConvert().
*/

QVariant::Type QVariant::type() const
{
    return d.type >= QMetaType::User ? UserType : static_cast<Type>(d.type);
}

/*!
    Returns the storage type of the value stored in the variant. For
    non-user types, this is the same as type().

    \sa type()
*/

int QVariant::userType() const
{
    return d.type;
}

/*!
    Assigns the value of the variant \a variant to this variant.
*/
QVariant& QVariant::operator=(const QVariant &variant)
{
    if (this == &variant)
        return *this;

    clear();
    if (variant.d.is_shared) {
        variant.d.data.shared->ref.ref();
        d = variant.d;
    } else if (variant.d.type > Char && variant.d.type < UserType) {
        d.type = variant.d.type;
        handler->construct(&d, variant.constData());
        d.is_null = variant.d.is_null;
    } else {
        d = variant.d;
    }

    return *this;
}

/*!
    \fn void QVariant::swap(QVariant &other)
    \since 4.8

    Swaps variant \a other with this variant. This operation is very
    fast and never fails.
*/

/*!
    \fn void QVariant::detach()

    \internal
*/

void QVariant::detach()
{
    if (!d.is_shared || d.data.shared->ref == 1)
        return;

    Private dd;
    dd.type = d.type;
    handler->construct(&dd, constData());
    if (!d.data.shared->ref.deref())
        handler->clear(&d);
    d.data.shared = dd.data.shared;
}

// ### Qt 5: change typeName()(and froends= to return a QString. Suggestion from Harald.
/*!
    Returns the name of the type stored in the variant. The returned
    strings describe the C++ datatype used to store the data: for
    example, "QFont", "QString", or "QVariantList". An Invalid
    variant returns 0.
*/
const char *QVariant::typeName() const
{
    return typeToName(Type(d.type));
}

/*!
    Convert this variant to type Invalid and free up any resources
    used.
*/
void QVariant::clear()
{
    if ((d.is_shared && !d.data.shared->ref.deref()) || (!d.is_shared && d.type < UserType && d.type > Char))
        handler->clear(&d);
    d.type = Invalid;
    d.is_null = true;
    d.is_shared = false;
}

/*!
    Converts the enum representation of the storage type, \a typ, to
    its string representation.

    Returns a null pointer if the type is QVariant::Invalid or doesn't exist.
*/
const char *QVariant::typeToName(Type typ)
{
    if (typ == Invalid)
        return nullptr;
    if (typ == UserType)
        return "UserType";

    return QMetaType::typeName(typ);
}


/*!
    Converts the string representation of the storage type given in \a
    name, to its enum representation.

    If the string representation cannot be converted to any enum
    representation, the variant is set to \c Invalid.
*/
QVariant::Type QVariant::nameToType(const char *name)
{
    if (!name || !*name)
        return Invalid;
    int metaType = QMetaType::type(name);
    return metaType <= int(LastGuiType) ? QVariant::Type(metaType) : UserType;
}

#ifndef QT_NO_DATASTREAM
/*!
    Internal function for loading a variant from stream \a s. Use the
    stream operators instead.

    \internal
*/
void QVariant::load(QDataStream &s)
{
    clear();

    qint32 u;
    s >> u;
    qint8 is_null = false;
    s >> is_null;
    if (u == QVariant::UserType) {
        QByteArray name;
        s >> name;
        u = QMetaType::type(name.constData());
        if (!u) {
            s.setStatus(QDataStream::ReadCorruptData);
            return;
        }
    }
    create(static_cast<int>(u), nullptr);
    d.is_null = is_null;

    if (!isValid()) {
        // Since we wrote something, we should read something
        QString x;
        s >> x;
        d.is_null = true;
        return;
    }

    // const cast is safe since we operate on a newly constructed variant
    if (!QMetaType::load(s, d.type, const_cast<void *>(constData()))) {
        s.setStatus(QDataStream::ReadCorruptData);
        qWarning("QVariant::load: unable to load type %d.", d.type);
    }
}

/*!
    Internal function for saving a variant to the stream \a s. Use the
    stream operators instead.

    \internal
*/
void QVariant::save(QDataStream &s) const
{
    qint32 tp = type();
    s << tp;
    s << qint8(d.is_null);
    if (tp == QVariant::UserType) {
        s << QMetaType::typeName(userType());
    }

    if (!isValid()) {
        s << QString();
        return;
    }

    if (!QMetaType::save(s, d.type, constData())) {
        Q_ASSERT_X(false, "QVariant::save", "Invalid type to save");
        qWarning("QVariant::save: unable to save type %d.", d.type);
    }
}

/*!
    \since 4.4

    Reads a variant \a p from the stream \a s.

    \sa \link datastreamformat.html Format of the QDataStream
    operators \endlink
*/
QDataStream& operator>>(QDataStream &s, QVariant &p)
{
    p.load(s);
    return s;
}

/*!
    Writes a variant \a p to the stream \a s.

    \sa \link datastreamformat.html Format of the QDataStream
    operators \endlink
*/
QDataStream& operator<<(QDataStream &s, const QVariant &p)
{
    p.save(s);
    return s;
}

/*!
    Reads a variant type \a p in enum representation from the stream \a s.
*/
QDataStream& operator>>(QDataStream &s, QVariant::Type &p)
{
    qint32 u;
    s >> u;
    p = (QVariant::Type)u;

    return s;
}

/*!
    Writes a variant type \a p to the stream \a s.
*/
QDataStream& operator<<(QDataStream &s, const QVariant::Type p)
{
    s << static_cast<qint32>(p);

    return s;
}

#endif //QT_NO_DATASTREAM

/*!
    \fn bool QVariant::isValid() const

    Returns true if the storage type of this variant is not
    QVariant::Invalid; otherwise returns false.
*/

template <typename T>
inline T qVariantToHelper(const QVariant::Private &d, QVariant::Type t,
                          const QVariant::Handler *handler)
{
    if (d.type == t)
        return *v_cast<T>(&d);

    T ret;
    handler->convert(&d, t, &ret, nullptr);
    return ret;
}

/*!
    \fn QStringList QVariant::toStringList() const

    Returns the variant as a QStringList if the variant has type()
    StringList, \l String, or \l List of a type that can be converted
    to QString; otherwise returns an empty list.

    \sa canConvert(), convert()
*/
QStringList QVariant::toStringList() const
{
    return qVariantToHelper<QStringList>(d, StringList, handler);
}

/*!
    Returns the variant as a QString if the variant has type() \l
    String, \l Bool, \l ByteArray, \l Char, \l Date, \l DateTime, \l
    Double, \l Int, \l LongLong, \l StringList, \l Time, \l UInt, or
    \l ULongLong; otherwise returns an empty string.

    \sa canConvert(), convert()
*/
QString QVariant::toString() const
{
    return qVariantToHelper<QString>(d, String, handler);
}

/*!
    Returns the variant as a QMap<QString, QVariant> if the variant
    has type() \l Map; otherwise returns an empty map.

    \sa canConvert(), convert()
*/
QVariantMap QVariant::toMap() const
{
    return qVariantToHelper<QVariantMap>(d, Map, handler);
}

/*!
    Returns the variant as a QHash<QString, QVariant> if the variant
    has type() \l Hash; otherwise returns an empty map.

    \sa canConvert(), convert()
*/
QVariantHash QVariant::toHash() const
{
    return qVariantToHelper<QVariantHash>(d, Hash, handler);
}

/*!
    \fn QDate QVariant::toDate() const

    Returns the variant as a QDate if the variant has type() \l Date,
    \l DateTime, or \l String; otherwise returns an invalid date.

    If the type() is \l String, an invalid date will be returned if the
    string cannot be parsed as a Qt::ISODate format date.

    \sa canConvert(), convert()
*/
QDate QVariant::toDate() const
{
    return qVariantToHelper<QDate>(d, Date, handler);
}

/*!
    \fn QTime QVariant::toTime() const

    Returns the variant as a QTime if the variant has type() \l Time,
    \l DateTime, or \l String; otherwise returns an invalid time.

    If the type() is \l String, an invalid time will be returned if
    the string cannot be parsed as a Qt::ISODate format time.

    \sa canConvert(), convert()
*/
QTime QVariant::toTime() const
{
    return qVariantToHelper<QTime>(d, Time, handler);
}

/*!
    \fn QDateTime QVariant::toDateTime() const

    Returns the variant as a QDateTime if the variant has type() \l
    DateTime, \l Date, or \l String; otherwise returns an invalid
    date/time.

    If the type() is \l String, an invalid date/time will be returned
    if the string cannot be parsed as a Qt::ISODate format date/time.

    \sa canConvert(), convert()
*/
QDateTime QVariant::toDateTime() const
{
    return qVariantToHelper<QDateTime>(d, DateTime, handler);
}

/*!
    \since 4.7
    \fn QEasingCurve QVariant::toEasingCurve() const

    Returns the variant as a QEasingCurve if the variant has type() \l
    EasingCurve; otherwise returns a default easing curve.

    \sa canConvert(), convert()
*/
#ifndef QT_BOOTSTRAPPED
QEasingCurve QVariant::toEasingCurve() const
{
    return qVariantToHelper<QEasingCurve>(d, EasingCurve, handler);
}

/*!
    \since 4.9

    Returns the variant as a QJsonDocument if the variant has userType() \l
    QJsonDocument; otherwise returns a default constructed QJsonDocument.

    \sa canConvert(), convert()
*/
QJsonDocument QVariant::toJsonDocument() const
{
    return qVariantToHelper<QJsonDocument>(d, JsonDocument, handler);
}
#endif

/*!
    \fn QByteArray QVariant::toByteArray() const

    Returns the variant as a QByteArray if the variant has type() \l
    ByteArray or \l String (converted using QString::fromAscii());
    otherwise returns an empty byte array.

    \sa canConvert(), convert()
*/
QByteArray QVariant::toByteArray() const
{
    return qVariantToHelper<QByteArray>(d, ByteArray, handler);
}

#ifndef QT_BOOTSTRAPPED
/*!
    \fn QPoint QVariant::toPoint() const

    Returns the variant as a QPoint if the variant has type()
    \l Point or \l PointF; otherwise returns a null QPoint.

    \sa canConvert(), convert()
*/
QPoint QVariant::toPoint() const
{
    return qVariantToHelper<QPoint>(d, Point, handler);
}

/*!
    \fn QRect QVariant::toRect() const

    Returns the variant as a QRect if the variant has type() \l Rect;
    otherwise returns an invalid QRect.

    \sa canConvert(), convert()
*/
QRect QVariant::toRect() const
{
    return qVariantToHelper<QRect>(d, Rect, handler);
}

/*!
    \fn QSize QVariant::toSize() const

    Returns the variant as a QSize if the variant has type() \l Size;
    otherwise returns an invalid QSize.

    \sa canConvert(), convert()
*/
QSize QVariant::toSize() const
{
    return qVariantToHelper<QSize>(d, Size, handler);
}

/*!
    \fn QSizeF QVariant::toSizeF() const

    Returns the variant as a QSizeF if the variant has type() \l
    SizeF; otherwise returns an invalid QSizeF.

    \sa canConvert(), convert()
*/
QSizeF QVariant::toSizeF() const
{
    return qVariantToHelper<QSizeF>(d, SizeF, handler);
}

/*!
    \fn QRectF QVariant::toRectF() const

    Returns the variant as a QRectF if the variant has type() \l Rect
    or \l RectF; otherwise returns an invalid QRectF.

    \sa canConvert(), convert()
*/
QRectF QVariant::toRectF() const
{
    return qVariantToHelper<QRectF>(d, RectF, handler);
}

/*!
    \fn QLineF QVariant::toLineF() const

    Returns the variant as a QLineF if the variant has type() \l
    LineF; otherwise returns an invalid QLineF.

    \sa canConvert(), convert()
*/
QLineF QVariant::toLineF() const
{
    return qVariantToHelper<QLineF>(d, LineF, handler);
}

/*!
    \fn QLine QVariant::toLine() const

    Returns the variant as a QLine if the variant has type() \l Line;
    otherwise returns an invalid QLine.

    \sa canConvert(), convert()
*/
QLine QVariant::toLine() const
{
    return qVariantToHelper<QLine>(d, Line, handler);
}

/*!
    \fn QPointF QVariant::toPointF() const

    Returns the variant as a QPointF if the variant has type() \l
    Point or \l PointF; otherwise returns a null QPointF.

    \sa canConvert(), convert()
*/
QPointF QVariant::toPointF() const
{
    return qVariantToHelper<QPointF>(d, PointF, handler);
}

/*!
    \fn QUrl QVariant::toUrl() const

    Returns the variant as a QUrl if the variant has type()
    \l Url; otherwise returns an invalid QUrl.

    \sa canConvert(), convert()
*/
QUrl QVariant::toUrl() const
{
    return qVariantToHelper<QUrl>(d, Url, handler);
}
#endif // !QT_BOOTSTRAPPED

/*!
    \fn QLocale QVariant::toLocale() const

    Returns the variant as a QLocale if the variant has type()
    \l Locale; otherwise returns an invalid QLocale.

    \sa canConvert(), convert()
*/
QLocale QVariant::toLocale() const
{
    return qVariantToHelper<QLocale>(d, Locale, handler);
}

/*!
    \fn QRegExp QVariant::toRegExp() const
    \since 4.1

    Returns the variant as a QRegExp if the variant has type() \l
    RegExp; otherwise returns an empty QRegExp.

    \sa canConvert(), convert()
*/
#ifndef QT_NO_REGEXP
QRegExp QVariant::toRegExp() const
{
    return qVariantToHelper<QRegExp>(d, RegExp, handler);
}
#endif

/*!
    \fn QChar QVariant::toChar() const

    Returns the variant as a QChar if the variant has type() \l Char,
    \l Int, or \l UInt; otherwise returns an invalid QChar.

    \sa canConvert(), convert()
*/
QChar QVariant::toChar() const
{
    return qVariantToHelper<QChar>(d, Char, handler);
}

/*!
    Returns the variant as a QBitArray if the variant has type()
    \l BitArray; otherwise returns an empty bit array.

    \sa canConvert(), convert()
*/
QBitArray QVariant::toBitArray() const
{
    return qVariantToHelper<QBitArray>(d, BitArray, handler);
}

template <typename T>
inline T qNumVariantToHelper(const QVariant::Private &d,
                             const QVariant::Handler *handler, bool *ok, const T& val)
{
    int t = qMetaTypeId<T>();
    if (ok)
        *ok = true;
    if (d.type == t)
        return val;

    T ret;
    if (!handler->convert(&d, t, &ret, ok) && ok)
        *ok = false;
    return ret;
}

/*!
    Returns the variant as an int if the variant has type() \l Int,
    \l Bool, \l ByteArray, \l Char, \l Double, \l LongLong, \l
    String, \l UInt, or \l ULongLong; otherwise returns 0.

    If \a ok is non-null: \c{*}\a{ok} is set to true if the value could be
    converted to an int; otherwise \c{*}\a{ok} is set to false.

    \bold{Warning:} If the value is convertible to a \l LongLong but is too
    large to be represented in an int, the resulting arithmetic overflow will
    not be reflected in \a ok. A simple workaround is to use QString::toInt().
    Fixing this bug has been postponed to Qt 5 in order to avoid breaking existing code.

    \sa canConvert(), convert()
*/
int QVariant::toInt(bool *ok) const
{
    return qNumVariantToHelper<int>(d, handler, ok, d.data.i);
}

/*!
    Returns the variant as an unsigned int if the variant has type()
    \l UInt,  \l Bool, \l ByteArray, \l Char, \l Double, \l Int, \l
    LongLong, \l String, or \l ULongLong; otherwise returns 0.

    If \a ok is non-null: \c{*}\a{ok} is set to true if the value could be
    converted to an unsigned int; otherwise \c{*}\a{ok} is set to false.

    \bold{Warning:} If the value is convertible to a \l ULongLong but is too
    large to be represented in an unsigned int, the resulting arithmetic overflow will
    not be reflected in \a ok. A simple workaround is to use QString::toUInt().
    Fixing this bug has been postponed to Qt 5 in order to avoid breaking existing code.

    \sa canConvert(), convert()
*/
uint QVariant::toUInt(bool *ok) const
{
    return qNumVariantToHelper<uint>(d, handler, ok, d.data.u);
}

/*!
    Returns the variant as a long long int if the variant has type()
    \l LongLong, \l Bool, \l ByteArray, \l Char, \l Double, \l Int,
    \l String, \l UInt, or \l ULongLong; otherwise returns 0.

    If \a ok is non-null: \c{*}\c{ok} is set to true if the value could be
    converted to an int; otherwise \c{*}\c{ok} is set to false.

    \sa canConvert(), convert()
*/
qlonglong QVariant::toLongLong(bool *ok) const
{
    return qNumVariantToHelper<qlonglong>(d, handler, ok, d.data.ll);
}

/*!
    Returns the variant as as an unsigned long long int if the
    variant has type() \l ULongLong, \l Bool, \l ByteArray, \l Char,
    \l Double, \l Int, \l LongLong, \l String, or \l UInt; otherwise
    returns 0.

    If \a ok is non-null: \c{*}\a{ok} is set to true if the value could be
    converted to an int; otherwise \c{*}\a{ok} is set to false.

    \sa canConvert(), convert()
*/
qulonglong QVariant::toULongLong(bool *ok) const
{
    return qNumVariantToHelper<qulonglong>(d, handler, ok, d.data.ull);
}

/*!
    Returns the variant as a bool if the variant has type() Bool.

    Returns true if the variant has type() \l Bool, \l Char, \l Double,
    \l Int, \l LongLong, \l UInt, or \l ULongLong and the value is
    non-zero, or if the variant has type \l String or \l ByteArray and
    its lower-case content is not empty, "0" or "false"; otherwise
    returns false.

    \sa canConvert(), convert()
*/
bool QVariant::toBool() const
{
    return qVariantToHelper<bool>(d, Bool, handler);
}

/*!
    Returns the variant as a double if the variant has type() \l
    Double, \l Float, \l Bool, \l ByteArray, \l Int, \l LongLong, \l String, \l
    UInt, or \l ULongLong; otherwise returns 0.0.

    If \a ok is non-null: \c{*}\a{ok} is set to true if the value could be
    converted to a double; otherwise \c{*}\a{ok} is set to false.

    \sa canConvert(), convert()
*/
double QVariant::toDouble(bool *ok) const
{
    return qNumVariantToHelper<double>(d, handler, ok, d.data.d);
}

/*!
    Returns the variant as a float if the variant has type() \l
    Double, \l Float, \l Bool, \l ByteArray, \l Int, \l LongLong, \l String, \l
    UInt, or \l ULongLong; otherwise returns 0.0.

    \since 4.6

    If \a ok is non-null: \c{*}\a{ok} is set to true if the value could be
    converted to a double; otherwise \c{*}\a{ok} is set to false.

    \sa canConvert(), convert()
*/
float QVariant::toFloat(bool *ok) const
{
    return qNumVariantToHelper<float>(d, handler, ok, d.data.f);
}

/*!
    Returns the variant as a qreal if the variant has type() \l
    Double, \l Float, \l Bool, \l ByteArray, \l Int, \l LongLong, \l String, \l
    UInt, or \l ULongLong; otherwise returns 0.0.

    \since 4.6

    If \a ok is non-null: \c{*}\a{ok} is set to true if the value could be
    converted to a double; otherwise \c{*}\a{ok} is set to false.

    \sa canConvert(), convert()
*/
qreal QVariant::toReal(bool *ok) const
{
#if defined(QT_NO_FPU)
    return qNumVariantToHelper<qreal>(d, handler, ok, d.data.f);
#else
    return qNumVariantToHelper<qreal>(d, handler, ok, d.data.d);
#endif
}

/*!
    Returns the variant as a QVariantList if the variant has type()
    \l List or \l StringList; otherwise returns an empty list.

    \sa canConvert(), convert()
*/
QVariantList QVariant::toList() const
{
    return qVariantToHelper<QVariantList>(d, List, handler);
}

/*! \fn QVariant::canCast(Type t) const
    Use canConvert() instead.
*/

/*! \fn QVariant::cast(Type t)
    Use convert() instead.
*/

/*!
    Returns true if the variant's type can be cast to the requested
    type, \a t. Such casting is done automatically when calling the
    toInt(), toBool(), ... methods.

    The following casts are done automatically:

    \table
    \header \o Type \o Automatically Cast To
    \row \o \l Bool \o \l Char, \l Double, \l Int, \l LongLong, \l String, \l UInt, \l ULongLong
    \row \o \l ByteArray \o \l Double, \l Int, \l LongLong, \l String, \l UInt, \l ULongLong
    \row \o \l Char \o \l Bool, \l Int, \l UInt, \l LongLong, \l ULongLong
    \row \o \l Color \o \l String
    \row \o \l Date \o \l DateTime, \l String
    \row \o \l DateTime \o \l Date, \l String, \l Time
    \row \o \l Double \o \l Bool, \l Int, \l LongLong, \l String, \l UInt, \l ULongLong
    \row \o \l Font \o \l String
    \row \o \l Int \o \l Bool, \l Char, \l Double, \l LongLong, \l String, \l UInt, \l ULongLong
    \row \o \l KeySequence \o \l Int, \l String
    \row \o \l List \o \l StringList (if the list's items can be converted to strings)
    \row \o \l LongLong \o \l Bool, \l ByteArray, \l Char, \l Double, \l Int, \l String, \l UInt, \l ULongLong
    \row \o \l Point \o PointF
    \row \o \l Rect \o RectF
    \row \o \l String \o \l Bool, \l ByteArray, \l Char, \l Color, \l Date, \l DateTime, \l Double,
                         \l Font, \l Int, \l KeySequence, \l LongLong, \l StringList, \l Time, \l UInt,
                         \l ULongLong
    \row \o \l StringList \o \l List, \l String (if the list contains exactly one item)
    \row \o \l Time \o \l String
    \row \o \l UInt \o \l Bool, \l Char, \l Double, \l Int, \l LongLong, \l String, \l ULongLong
    \row \o \l ULongLong \o \l Bool, \l Char, \l Double, \l Int, \l LongLong, \l String, \l UInt
    \row \o \l JsonValue \o \l QString, \l Bool, \l Int, \l UInt, \l Double, \l Float, \l ULong
                            \l Long, \l LongLong, \l ULongLong, \l UShort, \l UChar, \l Char, \l Short,
                            \l List, \l Map, \l Hash
    \row \o \l JsonObject \o \l Map, \l Hash
    \row \o \l JsonArray \o \l List
    \endtable

    \sa convert()
*/
bool QVariant::canConvert(Type t) const
{
    if (d.type == t)
        return true;

    if (t == QVariant::Invalid) {
        return false;
    } else if (t == QVariant::Bool) {
        switch (d.type) {
        case QVariant::Double:
        case QVariant::Float:
        case QVariant::Int:
        case QVariant::UInt:
        case QVariant::LongLong:
        case QVariant::ULongLong:
        case QVariant::ByteArray:
        case QVariant::String:
        case QVariant::Char:
            return true;
        default:
            return false;
        }
    } else if (t == QVariant::Int) {
        switch (d.type) {
        case QVariant::UInt:
        case QVariant::String:
        case QVariant::Double:
        case QVariant::Float:
        case QVariant::Bool:
        case QVariant::LongLong:
        case QVariant::ULongLong:
        case QVariant::Char:
        case QVariant::ByteArray:
        case QVariant::KeySequence:
        case QMetaType::ULong:
        case QMetaType::Long:
        case QMetaType::UShort:
        case QMetaType::UChar:
        case QMetaType::Char:
        case QMetaType::Short:
            return true;
        default:
            return false;
        }
    } else if (t == QVariant::UInt) {
        switch (d.type) {
        case QVariant::Int:
        case QVariant::String:
        case QVariant::Double:
        case QVariant::Float:
        case QVariant::Bool:
        case QVariant::LongLong:
        case QVariant::ULongLong:
        case QVariant::Char:
        case QVariant::ByteArray:
            return true;
        default:
            return false;
        }
    } else if (t == QVariant::LongLong) {
        switch (d.type) {
        case QVariant::Int:
        case QVariant::String:
        case QVariant::Double:
        case QVariant::Float:
        case QVariant::Bool:
        case QVariant::UInt:
        case QVariant::ULongLong:
        case QVariant::Char:
        case QVariant::ByteArray:
            return true;
        default:
            return false;
        }
    } else if (t == QVariant::ULongLong) {
        switch (d.type) {
        case QVariant::Int:
        case QVariant::String:
        case QVariant::Double:
        case QVariant::Float:
        case QVariant::Bool:
        case QVariant::UInt:
        case QVariant::LongLong:
        case QVariant::Char:
        case QVariant::ByteArray:
            return true;
        default:
            return false;
        }
    } else if (t == QVariant::Double) {
        switch (d.type) {
        case QVariant::Int:
        case QVariant::String:
        case QVariant::ULongLong:
        case QVariant::Bool:
        case QVariant::UInt:
        case QVariant::LongLong:
        case QVariant::ByteArray:
        case QVariant::Float:
            return true;
        default:
            return false;
        }
    } else if (t == QVariant::Float) {
        switch (d.type) {
        case QVariant::Int:
        case QVariant::String:
        case QVariant::ULongLong:
        case QVariant::Bool:
        case QVariant::UInt:
        case QVariant::LongLong:
        case QVariant::ByteArray:
        case QVariant::Double:
            return true;
        default:
            return false;
        }
    } else if (t == QVariant::Char) {
        switch (d.type) {
        case QVariant::Int:
        case QVariant::UInt:
        case QVariant::LongLong:
        case QVariant::ULongLong:
            return true;
        default:
            return false;
        }
    /* Map */
    } else if (t == QVariant::List) {
        switch (d.type) {
        case QVariant::StringList:
            return true;
        default:
            return false;
        }
    } else if (t == QVariant::String) {
        switch (d.type) {
        case QVariant::ByteArray:
        case QVariant::Int:
        case QVariant::UInt:
        case QVariant::Bool:
        case QVariant::Double:
        case QVariant::Float:
        case QVariant::Date:
        case QVariant::Time:
        case QVariant::DateTime:
        case QVariant::LongLong:
        case QVariant::ULongLong:
        case QVariant::Char:
        case QVariant::Url:
        case QVariant::KeySequence:
        case QVariant::Font:
        case QVariant::Color:
            return true;
        case QVariant::StringList:
            return v_cast<QStringList>(&d)->size() >= 1;
        default:
            return false;
        }
    } else if (t == QVariant::StringList) {
        switch (d.type) {
        case QVariant::List:
        case QVariant::String:
            return true;
        default:
            return false;
        }
    } else if (t == QVariant::ByteArray) {
        switch (d.type) {
        case QVariant::String:
        case QVariant::Int:
        case QVariant::UInt:
        case QVariant::Bool:
        case QVariant::Double:
        case QVariant::Float:
        case QVariant::LongLong:
        case QVariant::ULongLong:
        case QVariant::Color:
            return true;
        default:
            return false;
        }
    /* BitArray */
    } else if (t == QVariant::Date || t == QVariant::Time) {
        switch (d.type) {
        case QVariant::String:
        case QVariant::DateTime:
            return true;
        default:
            return false;
        }
    } else if (t == QVariant::DateTime) {
        switch (d.type) {
        case QVariant::String:
        case QVariant::Date:
            return true;
        default:
            return false;
        }
    } else if (t == QVariant::Url) {
        switch (d.type) {
        case QVariant::String:
            return true;
        default:
            return false;
        }
    /* Locale */
    } else if (t == QVariant::Rect) {
        switch (d.type) {
        case QVariant::RectF:
            return true;
        default:
            return false;
        }
    } else if (t == QVariant::RectF) {
        switch (d.type) {
        case QVariant::Rect:
            return true;
        default:
            return false;
        }
    } else if (t == QVariant::Size) {
        switch (d.type) {
        case QVariant::SizeF:
            return true;
        default:
            return false;
        }
    } else if (t == QVariant::SizeF) {
        switch (d.type) {
        case QVariant::Size:
            return true;
        default:
            return false;
        }
    } else if (t == QVariant::Line) {
        switch (d.type) {
        case QVariant::LineF:
            return true;
        default:
            return false;
        }
    } else if (t == QVariant::LineF) {
        switch (d.type) {
        case QVariant::Line:
            return true;
        default:
            return false;
        }
    } else if (t == QVariant::Point) {
        switch (d.type) {
        case QVariant::PointF:
            return true;
        default:
            return false;
        }
    } else if (t == QVariant::PointF) {
        switch (d.type) {
        case QVariant::Point:
            return true;
        default:
            return false;
        }
    /* RegExp */
    /* Hash */
    /* EasingCurve */
    /* JsonDocument */
    } else if (t == QVariant::Font) {
        switch (d.type) {
        case QVariant::String:
            return true;
        default:
            return false;
        }
    } else if (t == QVariant::Pixmap) {
        switch (d.type) {
        case QVariant::Image:
        case QVariant::Bitmap:
        case QVariant::Brush:
            return true;
        default:
            return false;
        }
    } else if (t == QVariant::Brush) {
        switch (d.type) {
        case QVariant::Color:
        case QVariant::Pixmap:
            return true;
        default:
            return false;
        }
    } else if (t == QVariant::Color) {
        switch (d.type) {
        case QVariant::String:
        case QVariant::ByteArray:
        case QVariant::Brush:
            return true;
        default:
            return false;
        }
    /* Palette */
    /* Icon */
    } else if (t == QVariant::Image) {
        switch (d.type) {
        case QVariant::Pixmap:
        case QVariant::Bitmap:
            return true;
        default:
            return false;
        }
    /* Polygon */
    /* Region */
    } else if (t == QVariant::Bitmap) {
        switch (d.type) {
        case QVariant::Pixmap:
        case QVariant::Image:
            return true;
        default:
            return false;
        }
    /* Cursor */
    /* SizePolicy */
    } else if (t == QVariant::KeySequence) {
        switch (d.type) {
        case QVariant::String:
        case QVariant::Int:
            return true;
        default:
            return false;
        }
    /* Pen */
    /* TextLength */
    /* TextFormat */
    /* Matrix */
    /* Transform */
    /* Matrix4x4 */
    /* Vector2D */
    /* Vector3D */
    /* Vector4D */
    /* Quaternion */
    }

    switch (int(t)) {
        case QMetaType::Long:
        case QMetaType::Char:
        case QMetaType::UChar:
        case QMetaType::ULong:
        case QMetaType::Short:
        case QMetaType::UShort: {
            // almost the same as Int case
            switch (d.type) {
                case QVariant::Int: // Int included
                case QVariant::UInt:
                case QVariant::String:
                case QVariant::Double:
                case QVariant::Float:
                case QVariant::Bool:
                case QVariant::LongLong:
                case QVariant::ULongLong:
                case QVariant::Char:
                case QVariant::ByteArray:
                case QVariant::KeySequence:
                    return true;
                default:
                    return false;
            }
        }
    }

    return false;
}

/*!
    Casts the variant to the requested type, \a t. If the cast cannot be
    done, the variant is cleared. Returns true if the current type of
    the variant was successfully cast; otherwise returns false.

    \warning For historical reasons, converting a null QVariant results
    in a null value of the desired type (e.g., an empty string for
    QString) and a result of false.

    \sa canConvert(), clear()
*/

bool QVariant::convert(Type t)
{
    if (d.type == int(t))
        return true;

    QVariant oldValue = *this;

    clear();
    if (!oldValue.canConvert(t))
        return false;

    create(t, nullptr);
    if (oldValue.isNull())
        return false;

    bool isOk = true;
    if (!handler->convert(&oldValue.d, t, data(), &isOk))
        isOk = false;
    d.is_null = !isOk;
    return isOk;
}

/*!
    \fn bool operator==(const QVariant &v1, const QVariant &v2)

    \relates QVariant

    Returns true if \a v1 and \a v2 are equal; otherwise returns false.

    \warning This function doesn't support custom types registered
    with qRegisterMetaType().
*/
/*!
    \fn bool operator!=(const QVariant &v1, const QVariant &v2)

    \relates QVariant

    Returns false if \a v1 and \a v2 are equal; otherwise returns true.

    \warning This function doesn't support custom types registered
    with qRegisterMetaType().
*/

/*! \fn bool QVariant::operator==(const QVariant &v) const

    Compares this QVariant with \a v and returns true if they are
    equal; otherwise returns false.

    In the case of custom types, their equalness operators are not called.
    Instead the values' addresses are compared.
*/

/*!
    \fn bool QVariant::operator!=(const QVariant &v) const

    Compares this QVariant with \a v and returns true if they are not
    equal; otherwise returns false.

    \warning This function doesn't support custom types registered
    with qRegisterMetaType().
*/

static inline bool qIsNumericType(int tp)
{
    return (tp >= QVariant::Bool && tp <= QVariant::Float)
           || (tp >= QMetaType::Long && tp <= QMetaType::UChar);
}

static inline bool qIsFloatingPoint(int tp)
{
    return tp == QVariant::Double || tp == QVariant::Float;
}

/*! \internal
 */
bool QVariant::cmp(const QVariant &v) const
{
    if (d.type != v.d.type) {
        if (qIsNumericType(d.type) && qIsNumericType(v.d.type)) {
            if (qIsFloatingPoint(d.type) || qIsFloatingPoint(v.d.type))
                return qFuzzyCompare(toReal(), v.toReal());
            return toLongLong() == v.toLongLong();
        }
        QVariant v2(v);
        if (!v2.convert(Type(d.type)))
            return false;
        return handler->compare(&d, &v2.d);
    }
    return handler->compare(&d, &v.d);
}

/*! \internal
 */

const void *QVariant::constData() const
{
    return d.is_shared ? d.data.shared->ptr : reinterpret_cast<const void *>(&d.data.ptr);
}

/*!
    \fn const void* QVariant::data() const

    \internal
*/

/*! \internal */
void* QVariant::data()
{
    detach();
    return d.is_shared ? d.data.shared->ptr : &d.data.ptr;
}

/*!
  Returns true if this is a NULL variant, false otherwise.
*/
bool QVariant::isNull() const
{
    return handler->isNull(&d);
}

#ifndef QT_NO_DEBUG_STREAM
QDebug operator<<(QDebug dbg, const QVariant &v)
{
#ifndef Q_BROKEN_DEBUG_STREAM
    dbg.nospace() << "QVariant(" << v.typeName() << ", ";
    QVariant::handler->debugStream(dbg, v);
    dbg.nospace() << ')';
    return dbg.space();
#else
    qWarning("This compiler doesn't support streaming QVariant to QDebug");
    return dbg;
    Q_UNUSED(v);
#endif
}

QDebug operator<<(QDebug dbg, const QVariant::Type p)
{
#ifndef Q_BROKEN_DEBUG_STREAM
    dbg.nospace() << "QVariant::" << QVariant::typeToName(p);
    return dbg.space();
#else
    qWarning("This compiler doesn't support streaming QVariant::Type to QDebug");
    return dbg;
    Q_UNUSED(p);
#endif
}
#endif

/*!
    \fn int &QVariant::asInt()

    Use toInt() instead.
*/

/*!
    \fn uint &QVariant::asUInt()

    Use toUInt() instead.
*/

/*!
    \fn qlonglong &QVariant::asLongLong()

    Use toLongLong() instead.
*/

/*!
    \fn qulonglong &QVariant::asULongLong()

    Use toULongLong() instead.
*/

/*!
    \fn bool &QVariant::asBool()

    Use toBool() instead.
*/

/*!
    \fn double &QVariant::asDouble()

    Use toDouble() instead.
*/

/*!
    \fn QByteArray &QVariant::asByteArray()

    Use toByteArray() instead.
*/

/*!
    \fn QBitArray &QVariant::asBitArray()

    Use toBitArray() instead.
*/

/*!
    \fn QString &QVariant::asString()

    Use toString() instead.
*/

/*!
    \fn QStringList &QVariant::asStringList()

    Use toStringList() instead.
*/

/*!
    \fn QDate &QVariant::asDate()

    Use toDate() instead.
*/

/*!
    \fn QTime &QVariant::asTime()

    Use toTime() instead.
*/

/*!
    \fn QDateTime &QVariant::asDateTime()

    Use toDateTime() instead.
*/

/*!
    \fn QList<QVariant> &QVariant::asList()

    Use toList() instead.
*/

/*!
    \fn QMap<QString, QVariant> &QVariant::asMap()

    Use toMap() instead.
*/

/*!
    \fn QVariant::QVariant(bool b, int dummy)

    Use the QVariant(bool) constructor instead.

*/

/*!
    \fn const QByteArray QVariant::toCString() const

    Use toByteArray() instead.
*/

/*!
    \fn QByteArray &QVariant::asCString()

    Use toByteArray() instead.
*/

/*!
    \fn QPoint &QVariant::asPoint()

    Use toPoint() instead.
 */

/*!
    \fn QRect &QVariant::asRect()

    Use toRect() instead.
 */

/*!
    \fn QSize &QVariant::asSize()

    Use toSize() instead.
 */

/*! \fn void QVariant::setValue(const T &value)

    Stores a copy of \a value. If \c{T} is a type that QVariant
    doesn't support, QMetaType is used to store the value. A compile
    error will occur if QMetaType doesn't handle the type.

    Example:

    \snippet doc/src/snippets/code/src_corelib_kernel_qvariant.cpp 4

    \sa value(), fromValue(), canConvert()
 */

/*! \fn T QVariant::value() const

    Returns the stored value converted to the template type \c{T}.
    Call canConvert() to find out whether a type can be converted.
    If the value cannot be converted, \l{default-constructed value}
    will be returned.

    If the type \c{T} is supported by QVariant, this function behaves
    exactly as toString(), toInt() etc.

    Example:

    \snippet doc/src/snippets/code/src_corelib_kernel_qvariant.cpp 5

    \sa setValue(), fromValue(), canConvert()
*/

/*! \fn bool QVariant::canConvert() const

    Returns true if the variant can be converted to the template type \c{T},
    otherwise false.

    Example:

    \snippet doc/src/snippets/code/src_corelib_kernel_qvariant.cpp 6

    \sa convert()
*/

/*! \fn static QVariant QVariant::fromValue(const T &value)

    Returns a QVariant containing a copy of \a value. Behaves
    exactly like setValue() otherwise.

    Example:

    \snippet doc/src/snippets/code/src_corelib_kernel_qvariant.cpp 7

    \note If you are working with custom types, you should use
    the Q_DECLARE_METATYPE() macro to register your custom type.

    \sa setValue(), value()
*/

/*!
    \typedef QVariantList
    \relates QVariant

    Synonym for QList<QVariant>.
*/

/*!
    \typedef QVariantMap
    \relates QVariant

    Synonym for QMap<QString, QVariant>.
*/

/*!
    \typedef QVariantHash
    \relates QVariant
    \since 4.5

    Synonym for QHash<QString, QVariant>.
*/

QT_END_NAMESPACE
