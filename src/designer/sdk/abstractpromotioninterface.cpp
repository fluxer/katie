/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016 Ivailo Monev
**
** This file is part of the Katie Designer of the Katie Toolkit.
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

#include "abstractpromotioninterface.h"

QT_BEGIN_NAMESPACE

QDesignerPromotionInterface::~QDesignerPromotionInterface()
{
}

/*!
    \class QDesignerPromotionInterface

    \brief The QDesignerPromotionInterface provides functions for modifying
           the promoted classes in Designer.
    \inmodule QtDesigner
    \internal
    \since 4.3
*/

/*!
    \class QDesignerPromotionInterface::PromotedClass
    A pair of database items containing the base class and the promoted class.

    \typedef QDesignerPromotionInterface::PromotedClasses
    A list of PromotedClass items.

    virtual QDesignerPromotionInterface::PromotedClasses promotedClasses()  const

    Returns a list of promoted classes along with their base classes in alphabetical order.
    It can be used to populate tree models for editing promoted widgets.

*/

/*!
    \fn virtual QSet<QString> QDesignerPromotionInterface::referencedPromotedClassNames()  const

    Returns a set of promoted classed that are referenced by the currently opened forms.
*/

/*! 
    \fn virtual bool QDesignerPromotionInterface::addPromotedClass(const QString &baseClass, const QString &className, const QString &includeFile, QString *errorMessage)

    Add a promoted class named \a with the base class \a and include file \a includeFile. Returns \c true on success or \c false along
    with an error message in \a errorMessage on failure.
*/

/*! 
    \fn  virtual bool QDesignerPromotionInterface::removePromotedClass(const QString &className, QString *errorMessage)

    Remove the promoted class named \a className unless it is referenced by a form. Returns \c true on success or \c false along
    with an error message in \a errorMessage on failure.
*/

/*! 
    \fn  virtual bool QDesignerPromotionInterface::changePromotedClassName(const QString &oldClassName, const QString &newClassName,  QString *errorMessage)

    Change the class name of a promoted class from \a oldClassName to  \a newClassName. Returns \c true on success or \c false along
    with an error message in \a errorMessage on failure.
*/

/*! 
    \fn  virtual bool QDesignerPromotionInterface::setPromotedClassIncludeFile(const QString &className, const QString &includeFile, QString *errorMessage)

    Change the include file of a promoted class named \a className to be \a includeFile. Returns \c true on success or \c false along
    with an error message in \a errorMessage on failure.
*/

/*! \fn virtual QList<QDesignerWidgetDataBaseItemInterface *> QDesignerPromotionInterface::promotionBaseClasses() const

     Return a list of base classes that are suitable for promotion.
*/

QT_END_NAMESPACE
