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

#include "cppwritedeclaration.h"
#include "cppwriteinitialization.h"
#include "driver.h"
#include "ui4.h"
#include "uic.h"
#include "customwidgetsinfo.h"

#include <QtCore/QTextStream>
#include <QtCore/QDebug>

QT_BEGIN_NAMESPACE

namespace {
    void openNameSpaces(const QStringList &namespaceList, QTextStream &output) {
        if (namespaceList.empty())
            return;
        const QStringList::const_iterator cend = namespaceList.constEnd();
        for (QStringList::const_iterator it = namespaceList.constBegin(); it != cend; ++it) {
            if (!it->isEmpty()) {
                output << "namespace " << *it << " {\n";
            }
        }
    }

    void closeNameSpaces(const QStringList &namespaceList, QTextStream &output) {
        if (namespaceList.empty())
            return;

        QListIterator<QString> it(namespaceList);
        it.toBack();
        while (it.hasPrevious()) {
            const QString ns = it.previous();
            if (!ns.isEmpty()) {
                output << "} // namespace " << ns << "\n";
            }
        }
    }
}

namespace CPP {

WriteDeclaration::WriteDeclaration(Uic *uic)
    : m_uic(uic),
    m_driver(uic->driver()),
    m_output(uic->output()),
    m_option(uic->option())
{
}

void WriteDeclaration::acceptUI(DomUI *node)
{
    QString qualifiedClassName = node->elementClass() + m_option.postfix;
    QString className = qualifiedClassName;

    QString varName = m_driver->findOrInsertWidget(node->elementWidget());
    QString widgetClassName = node->elementWidget()->attributeClass();

    QString exportMacro = node->elementExportMacro();
    if (!exportMacro.isEmpty())
        exportMacro.append(QLatin1Char(' '));

    QStringList namespaceList = qualifiedClassName.split(QLatin1String("::"));
    if (namespaceList.count()) {
        className = namespaceList.last();
        namespaceList.removeLast();
    }

    // This is a bit of the hack but covers the cases Qt in/without namespaces
    // and User defined classes in/without namespaces. The "strange" case
    // is a User using Qt-in-namespace having his own classes not in a namespace.
    // In this case the generated Ui helper classes will also end up in
    // the Qt namespace (which is harmless, but not "pretty")
    const bool needsMacro = namespaceList.count() == 0
        || namespaceList[0] == QLatin1String("qdesigner_internal");

    if (needsMacro)
        m_output << "QT_BEGIN_NAMESPACE\n\n";

    openNameSpaces(namespaceList, m_output);

    if (namespaceList.count())
        m_output << "\n";

    m_output << "class " << exportMacro << m_option.prefix << className << "\n"
           << "{\n"
           << "public:\n";

    TreeWalker::acceptWidget(node->elementWidget());
    if (const DomButtonGroups *domButtonGroups = node->elementButtonGroups())
        acceptButtonGroups(domButtonGroups);

    m_output << "\n";

    WriteInitialization(m_uic).acceptUI(node);

    m_output << "};\n\n";

    closeNameSpaces(namespaceList, m_output);

    if (namespaceList.count())
        m_output << "\n";

    if (m_option.generateNamespace && !m_option.prefix.isEmpty()) {
        namespaceList.append(QLatin1String("Ui"));

        openNameSpaces(namespaceList, m_output);

        m_output << m_option.indent << "class " << exportMacro << className << ": public " << m_option.prefix << className << " {};\n";

        closeNameSpaces(namespaceList, m_output);

        if (namespaceList.count())
            m_output << "\n";
    }

    if (needsMacro)
        m_output << "QT_END_NAMESPACE\n\n";
}

void WriteDeclaration::acceptWidget(DomWidget *node)
{
    QString className = QLatin1String("QWidget");
    if (node->hasAttributeClass())
        className = node->attributeClass();

    m_output << m_option.indent << m_uic->customWidgetsInfo()->realClassName(className) << " *" << m_driver->findOrInsertWidget(node) << ";\n";

    TreeWalker::acceptWidget(node);
}

void WriteDeclaration::acceptSpacer(DomSpacer *node)
{
     m_output << m_option.indent << "QSpacerItem *" << m_driver->findOrInsertSpacer(node) << ";\n";
     TreeWalker::acceptSpacer(node);
}

void WriteDeclaration::acceptLayout(DomLayout *node)
{
    QString className = QLatin1String("QLayout");
    if (node->hasAttributeClass())
        className = node->attributeClass();

    m_output << m_option.indent << className << " *" << m_driver->findOrInsertLayout(node) << ";\n";

    TreeWalker::acceptLayout(node);
}

void WriteDeclaration::acceptActionGroup(DomActionGroup *node)
{
    m_output << m_option.indent << "QActionGroup *" << m_driver->findOrInsertActionGroup(node) << ";\n";

    TreeWalker::acceptActionGroup(node);
}

void WriteDeclaration::acceptAction(DomAction *node)
{
    m_output << m_option.indent << "QAction *" << m_driver->findOrInsertAction(node) << ";\n";

    TreeWalker::acceptAction(node);
}

void WriteDeclaration::acceptButtonGroup(const DomButtonGroup *buttonGroup)
{
    m_output << m_option.indent << "QButtonGroup *" << m_driver->findOrInsertButtonGroup(buttonGroup) << ";\n";
    TreeWalker::acceptButtonGroup(buttonGroup);
}

} // namespace CPP

QT_END_NAMESPACE
