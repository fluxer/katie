/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016 Ivailo Monev
**
** This file is part of the tools applications of the Katie Toolkit.
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

#ifndef UIC_H
#define UIC_H

#include "customwidgetsinfo.h"
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QHash>
#include <QtCore/QStack>
#include <QtXml/qxmlstream.h>

QT_BEGIN_NAMESPACE

class QTextStream;
class QIODevice;

class Driver;
class DomUI;
class DomWidget;
class DomSpacer;
class DomLayout;
class DomLayoutItem;
class DomItem;

struct Option;

class Uic
{
public:
    Uic(Driver *driver);
    ~Uic();

    bool printDependencies();

    inline Driver *driver() const
    { return drv; }

    inline QTextStream &output()
    { return out; }

    inline const Option &option() const
    { return opt; }

    inline QString pixmapFunction() const
    { return pixFunction; }

    inline void setPixmapFunction(const QString &f)
    { pixFunction = f; }

    inline bool hasExternalPixmap() const
    { return externalPix; }

    inline void setExternalPixmap(bool b)
    { externalPix = b; }

    inline const CustomWidgetsInfo *customWidgetsInfo() const
    { return &cWidgetsInfo; }

    bool write(QIODevice *in);

    bool write(DomUI *ui);

    bool isMainWindow(const QString &className) const;
    bool isToolBar(const QString &className) const;
    bool isStatusBar(const QString &className) const;
    bool isButton(const QString &className) const;
    bool isContainer(const QString &className) const;
    bool isCustomWidgetContainer(const QString &className) const;
    bool isMenuBar(const QString &className) const;
    bool isMenu(const QString &className) const;

private:
    // copyright header
    void writeCopyrightHeader(DomUI *ui);
    DomUI *parseUiFile(QXmlStreamReader &reader);

    // header protection
    void writeHeaderProtectionStart();
    void writeHeaderProtectionEnd();

private:
    Driver *drv;
    QTextStream &out;
    Option &opt;
    CustomWidgetsInfo cWidgetsInfo;
    QString pixFunction;
    bool externalPix;
};

QT_END_NAMESPACE

#endif // UIC_H
