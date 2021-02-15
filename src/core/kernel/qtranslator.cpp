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
#include "qobject_p.h"

#include <libintl.h>

QT_BEGIN_NAMESPACE

class QTranslatorPrivate : public QObjectPrivate
{
    Q_DECLARE_PUBLIC(QTranslator)
public:

    QTranslatorPrivate() {}

    QByteArray domain;
};

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
    Constructs an empty message file object with parent \a parent that
    is not connected to any file.
*/

QTranslator::QTranslator(QObject * parent)
    : QObject(*new QTranslatorPrivate, parent)
{
}


/*!
    Destroys the object and frees any allocated resources.
*/

QTranslator::~QTranslator()
{
    if (QCoreApplication::instance())
        QCoreApplication::removeTranslator(this);
}

/*!

    Loads \a filename (with ".mo" as suffix), which is file name relative
    to the translations installation directory. Returns true if the
    translation is successfully loaded; otherwise returns false.

    The previous contents of this translator object are discarded.

    \sa QLibraryInfo
*/

bool QTranslator::load(const QString &domain)
{
    if (domain.isEmpty()) {
        return false;
    }
    Q_D(QTranslator);
    d->domain = domain.toLatin1();
    bind_textdomain_codeset(d->domain.constData(), "UTF-8");
    return true;
}

/*!
    Returns the translation for the key (\a context, \a sourceText).
    The text will be translated depending on the system locale.

    \sa load()
*/
QString QTranslator::translate(const char *context, const char *sourceText) const
{
    Q_UNUSED(context);
    Q_D(const QTranslator);
    return QString::fromUtf8(dgettext(d->domain.constData(), sourceText));
}

/*!
    Returns true if this translator is empty, otherwise returns false.
    This function works with stripped and unstripped translation files.
*/
bool QTranslator::isEmpty() const
{
    Q_D(const QTranslator);
    return d->domain.isEmpty();
}

#include "moc_qtranslator.h"

QT_END_NAMESPACE

#endif // QT_NO_TRANSLATION
