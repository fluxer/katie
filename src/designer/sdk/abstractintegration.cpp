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

#include "abstractintegration.h"
#include "abstractformeditor.h"

#include <QtCore/QVariant>
#include <QtCore/QSharedPointer>

QT_BEGIN_NAMESPACE

// Add 'private' struct as a dynamic property.

static const char privatePropertyC[] = "_q_integrationprivate";

struct QDesignerIntegrationInterfacePrivate {
    QDesignerIntegrationInterfacePrivate() :
        headerSuffix(QLatin1String(".h")),
        headerLowercase(true) {}

    QString headerSuffix;
    bool headerLowercase;
};

typedef QSharedPointer<QDesignerIntegrationInterfacePrivate> QDesignerIntegrationInterfacePrivatePtr;

QT_END_NAMESPACE
Q_DECLARE_METATYPE(QT_PREPEND_NAMESPACE(QDesignerIntegrationInterfacePrivatePtr))
QT_BEGIN_NAMESPACE

static QDesignerIntegrationInterfacePrivatePtr integrationD(const QObject *o)
{
    const QVariant property = o->property(privatePropertyC);
    Q_ASSERT(property.canConvert<QDesignerIntegrationInterfacePrivatePtr>());
    return qvariant_cast<QDesignerIntegrationInterfacePrivatePtr>(property);
}

QDesignerIntegrationInterface::QDesignerIntegrationInterface(QDesignerFormEditorInterface *core, QObject *parent)
    : QObject(parent),
      m_core(core)
{
    core->setIntegration(this);
    const QDesignerIntegrationInterfacePrivatePtr d(new QDesignerIntegrationInterfacePrivate);
    setProperty(privatePropertyC, qVariantFromValue<QDesignerIntegrationInterfacePrivatePtr>(d));
}

QString QDesignerIntegrationInterface::headerSuffix() const
{
    return integrationD(this)->headerSuffix;
}

void QDesignerIntegrationInterface::setHeaderSuffix(const QString &headerSuffix)
{
    integrationD(this)->headerSuffix = headerSuffix;
}

bool QDesignerIntegrationInterface::isHeaderLowercase() const
{
    return integrationD(this)->headerLowercase;
}

void QDesignerIntegrationInterface::setHeaderLowercase(bool headerLowercase)
{
    integrationD(this)->headerLowercase = headerLowercase;
}

QT_END_NAMESPACE

#include "moc_abstractintegration.h"
