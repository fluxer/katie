/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016-2021 Ivailo Monev
**
** This file is part of the plugins of the Katie Toolkit.
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

#include <QtDeclarative/qdeclarativeextensionplugin.h>
#include <QtDeclarative/qdeclarative.h>

#include "qdeclarativeparticles_p.h"

QT_BEGIN_NAMESPACE

class QParticlesQmlModule : public QDeclarativeExtensionPlugin
{
    Q_OBJECT
public:
    virtual void registerTypes(const char *uri)
    {
        Q_ASSERT(QLatin1String(uri) == QLatin1String("Qt.labs.particles"));
        qmlRegisterType<QDeclarativeParticleMotion>(uri,1,0,"ParticleMotion");
        qmlRegisterType<QDeclarativeParticleMotionGravity>(uri,1,0,"ParticleMotionGravity");
        qmlRegisterType<QDeclarativeParticleMotionLinear>(uri,1,0,"ParticleMotionLinear");
        qmlRegisterType<QDeclarativeParticleMotionWander>(uri,1,0,"ParticleMotionWander");
        qmlRegisterType<QDeclarativeParticles>(uri,1,0,"Particles");
    }
};

QT_END_NAMESPACE

#include "moc_particles.cpp"

Q_EXPORT_PLUGIN2(qmlparticlesplugin, QT_PREPEND_NAMESPACE(QParticlesQmlModule))
