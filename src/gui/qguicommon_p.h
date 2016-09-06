#ifndef QGUICOMMON_P_H
#define QGUICOMMON_P_H

#include "qwidget.h"
#include "qfactoryloader_p.h"
#include "qiconengineplugin.h"
#include "qimageiohandler.h"

Q_GLOBAL_STATIC(QWidget, globalStyleInfoWidget)

#if !defined(QT_NO_LIBRARY)
Q_GLOBAL_STATIC_WITH_ARGS(QFactoryLoader, iconloader,
    (QIconEngineFactoryInterface_iid, QLatin1String("/iconengines"), Qt::CaseInsensitive))
#endif

#if !defined(QT_NO_LIBRARY)
Q_GLOBAL_STATIC_WITH_ARGS(QFactoryLoader, imageloader,
    (QImageIOHandlerFactoryInterface_iid, QLatin1String("/imageformats")))
#endif

#endif // QGUICOMMON_P_H
