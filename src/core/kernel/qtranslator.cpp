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

#include "qtranslator.h"

#ifndef QT_NO_TRANSLATION

#include "qcoreapplication.h"
#include "qlibraryinfo.h"
#include "qlocale.h"
#include "qdir.h"
#include "qtextcodec.h"
#include "qdebug.h"

QT_BEGIN_NAMESPACE

static bool isCharEqual(const char* const byteptr, const int bytelen, const char* const charptr, const int charlen)
{
    if (bytelen != charlen) {
        return false;
    }
    return (::memcmp(byteptr, charptr, charlen) == 0);
}

class QTranslatorPrivate
{
public:
    QTranslatorPrivate();

    QByteArray domain;
    mutable QByteArray data;
    mutable QTextConverter converter;
    quint32 offset;

private:
    Q_DISABLE_COPY(QTranslatorPrivate);
};

QTranslatorPrivate::QTranslatorPrivate()
    : offset(0)
{
}


/*!
    \class QTranslator

    \brief The QTranslator class provides internationalization support for text
    output.

    \ingroup i18n

    An object of this class contains a set of translations from a
    source language to a target language. QTranslator provides
    functions to look up translations in a translation file.
    Translation files are created using \l{Qt Linguist}.

    The most common use of QTranslator is to: load a translation
    file, install it using QApplication::installTranslator(), and use
    it via QObject::tr(). Here's the \c main() function from the
    \l{linguist/hellotr}{Hello tr()} example:

    \snippet examples/linguist/hellotr/main.cpp 2

    Note that the translator must be created \e before the
    application's widgets.

    Most applications will never need to do anything else with this
    class. The other functions provided by this class are useful for
    applications that work on translator files.

    \section1 Looking up Translations

    It is possible to look up a translation using translate() (as tr()
    and QApplication::translate() do). The translate() function takes
    up to three parameters:

    \list
    \o The \e context - usually the class name for the tr() caller.
    \o The \e {source text} - usually the argument to tr().
    \endlist

    For example, the "Cancel" in a dialog might have "Anuluj" when the
    program runs in Polish (in this case the source text would be
    "Cancel"). The context would (normally) be the dialog's class
    name; there would normally be no comment, and the translated text
    would be "Anuluj".

    \section1 Using Multiple Translations

    Multiple translation files can be installed in an application.
    Translations are searched for in the reverse order in which they were
    installed, so the most recently installed translation file is searched
    for translations first and the earliest translation file is searched
    last. The search stops as soon as a translation containing a matching
    string is found.

    This mechanism makes it possible for a specific translation to be
    "selected" or given priority over the others; simply uninstall the
    translator from the application by passing it to the
    QApplication::removeTranslator() function and reinstall it with
    QApplication::installTranslator(). It will then be the first
    translation to be searched for matching strings.

    \sa QApplication::installTranslator(), QApplication::removeTranslator(),
        QObject::tr(), QApplication::translate(), {I18N Example},
        {Hello tr() Example}, {Arrow Pad Example}, {Troll Print Example}
*/

/*!
    Constructs an empty message file object that is not connected to any file.
*/
QTranslator::QTranslator()
    : d_ptr(new QTranslatorPrivate())
{
}

/*!
    Destroys the object and frees any allocated resources.
*/
QTranslator::~QTranslator()
{
    if (QCoreApplication::instance()) {
        QCoreApplication::removeTranslator(this);
    }
    delete d_ptr;
}

/*!
    Loads translation for \a domain and \a locale, \a domain being
    filename relative to the translations installation directory
    without the suffix and \a locale being either empty or locale of
    choice; if empty the system locale will be used. Returns true if
    the translation is successfully loaded; otherwise returns false.

    The previous contents of this translator object are discarded.

    \sa QLibraryInfo
*/
bool QTranslator::load(const QString &domain, const QString &locale)
{
    Q_D(QTranslator);
    d->data.clear();
    d->converter = QTextConverter();
    d->offset = 0;
    if (domain.isEmpty()) {
        qWarning("QTranslator::load: Domain is empty");
        return false;
    }

    QString translationfilepath = QLibraryInfo::location(QLibraryInfo::TranslationsPath);
    translationfilepath.append(QDir::separator());
    if (locale.isEmpty()) {
        translationfilepath.append(QLocale::system().name());
    } else {
        translationfilepath.append(locale);
    }
    translationfilepath.append(QDir::separator());
    translationfilepath.append(domain);
    translationfilepath.append(QLatin1String(".tr"));
    // qDebug() << Q_FUNC_INFO << translationfilepath;

    QFile translationfile(translationfilepath);
    if (!translationfile.open(QFile::ReadOnly)) {
        qWarning("QTranslator::load: %s", qPrintable(translationfile.errorString()));
        return false;
    }

    return loadFromData(translationfile.readAll());
}

/*!
    Loads translation from \a data. Returns true if the translation
    is successfully loaded; otherwise returns false.

    The previous contents of this translator object are discarded.

    \sa QLibraryInfo
*/
bool QTranslator::loadFromData(const QByteArray &data)
{
    Q_D(QTranslator);
    d->data = data;
    d->converter = QTextConverter();
    d->offset = 0;

    if (data.isEmpty()) {
        qWarning("QTranslator::load: Empty data");
        d->data.clear();
        return false;
    }
    
    QDataStream trheaderstream(&d->data, QIODevice::ReadOnly);
    QByteArray trmagic;
    QByteArray trcodec;
    trheaderstream >> trmagic;
    trheaderstream >> trcodec;

    if (trmagic != "KATIE_TRANSLATION") {
        qWarning("QTranslator::load: Invalid magic");
        d->data.clear();
        return false;
    }

    d->converter = QTextConverter(trcodec);
    d->offset = trheaderstream.device()->pos();
    return true;
}

/*!
    Returns the translation for the key (\a context, \a sourceText).
    The text will be translated depending on either the locale specified
    when the translation was loaded or the system locale.

    \sa load()
*/
QString QTranslator::translate(const char *context, const char *sourceText) const
{
    if (isEmpty()) {
        return QString::fromUtf8(sourceText);
    }

    Q_D(const QTranslator);
    const int contextlen = qstrlen(context);
    const int sourcelen = qstrlen(sourceText);
    QDataStream trdatastream(&d->data, QIODevice::ReadOnly);
    QIODevice* trdatadevice = trdatastream.device();
    trdatadevice->seek(d->offset);
    while (!trdatadevice->atEnd()) {
        QByteArray trmsgctxt;
        QByteArray trmsgid;
        QByteArray trmsgstr;
        QByteArray trmsgid_plural;
        QByteArray trmsgstr_plural;
        trdatastream >> trmsgctxt;
        trdatastream >> trmsgid;
        trdatastream >> trmsgstr;
        trdatastream >> trmsgid_plural;
        trdatastream >> trmsgstr_plural;

        // this search method assumes plurals and regular messages are unique strings
        if (isCharEqual(trmsgctxt.constData(), trmsgctxt.size(), context, contextlen)
            && isCharEqual(trmsgid_plural.constData(), trmsgid_plural.size(), sourceText, sourcelen)) {
            d->converter.reset();
            return d->converter.toUnicode(trmsgstr_plural.constData(), trmsgstr_plural.size());
        }

        if (isCharEqual(trmsgctxt.constData(), trmsgctxt.size(), context, contextlen)
            && isCharEqual(trmsgid.constData(), trmsgid.size(), sourceText, sourcelen)) {
            d->converter.reset();
            return d->converter.toUnicode(trmsgstr.constData(), trmsgstr.size());
        }
    }
    return QString::fromUtf8(sourceText);
}

/*!
    Returns the translation for the key (\a context, \a sourceText).
    The text will be translated depending on either the locale specified
    when the translation was loaded or the system locale.

    If no translation is found empty string is returned.

    \sa load()
*/
QString QTranslator::translateStrict(const char *context, const char *sourceText) const
{
    if (isEmpty()) {
        return QString();
    }

    Q_D(const QTranslator);
    const int contextlen = qstrlen(context);
    const int sourcelen = qstrlen(sourceText);
    QDataStream trdatastream(&d->data, QIODevice::ReadOnly);
    QIODevice* trdatadevice = trdatastream.device();
    trdatadevice->seek(d->offset);
    while (!trdatadevice->atEnd()) {
        QByteArray trmsgctxt;
        QByteArray trmsgid;
        QByteArray trmsgstr;
        QByteArray trmsgid_plural;
        QByteArray trmsgstr_plural;
        trdatastream >> trmsgctxt;
        trdatastream >> trmsgid;
        trdatastream >> trmsgstr;
        trdatastream >> trmsgid_plural;
        trdatastream >> trmsgstr_plural;

        if (isCharEqual(trmsgctxt.constData(), trmsgctxt.size(), context, contextlen)
            && isCharEqual(trmsgid_plural.constData(), trmsgid_plural.size(), sourceText, sourcelen)) {
            d->converter.reset();
            return d->converter.toUnicode(trmsgstr_plural.constData(), trmsgstr_plural.size());
        }

        if (isCharEqual(trmsgctxt.constData(), trmsgctxt.size(), context, contextlen)
            && isCharEqual(trmsgid.constData(), trmsgid.size(), sourceText, sourcelen)) {
            d->converter.reset();
            return d->converter.toUnicode(trmsgstr.constData(), trmsgstr.size());
        }
    }
    return QString();
}

/*!
    Returns true if this translator is empty, otherwise returns false.
*/
bool QTranslator::isEmpty() const
{
    Q_D(const QTranslator);
    return d->data.isEmpty();
}

QT_END_NAMESPACE

#endif // QT_NO_TRANSLATION
