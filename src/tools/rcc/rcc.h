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

#ifndef RCC_H
#define RCC_H

#include <QtCore/QStringList>
#include <QtCore/QHash>
#include <QtCore/QString>

QT_BEGIN_NAMESPACE

class RCCFileInfo;
class QIODevice;
class QTextStream;


class RCCResourceLibrary
{
    RCCResourceLibrary(const RCCResourceLibrary &);
    RCCResourceLibrary &operator=(const RCCResourceLibrary &);

public:
    RCCResourceLibrary();
    ~RCCResourceLibrary();

    bool output(QIODevice &out, QIODevice &errorDevice);

    bool readFiles(bool ignoreErrors, QIODevice &errorDevice);

    enum Format { Binary, C_Code };
    void setFormat(Format f) { m_format = f; }
    Format format() const { return m_format; }

    void setInputFiles(const QStringList &files) { m_fileNames = files; }
    QStringList inputFiles() const { return m_fileNames; }

    QStringList dataFiles() const;

    // Return a map of resource identifier (':/newPrefix/images/p1.png') to file.
    typedef QHash<QString, QString> ResourceDataFileMap;
    ResourceDataFileMap resourceDataFileMap() const;

    void setVerbose(bool b) { m_verbose = b; }
    bool verbose() const { return m_verbose; }

    void setInitName(const QString &name);
    QString initName() const { return m_initName; }

    void setCompressLevel(int c) { m_compressLevel = c; }
    int compressLevel() const { return m_compressLevel; }

    void setCompressThreshold(int t) { m_compressThreshold = t; }
    int compressThreshold() const { return m_compressThreshold; }

    void setResourceRoot(const QString &root) { m_resourceRoot = root; }
    QString resourceRoot() const { return m_resourceRoot; }
    
    void setUseNameSpace(bool v) { m_useNameSpace = v; }
    bool useNameSpace() const { return m_useNameSpace; }
    
    QStringList failedResources() const { return m_failedResources; }

private:
    friend class RCCFileInfo;
    void reset();
    bool addFile(const QString &alias, const RCCFileInfo &file);
    bool interpretResourceFile(QIODevice *inputDevice, const QString &file,
        QString currentPath = QString(), bool ignoreErrors = false);
    bool writeHeader();
    bool writeDataBlobs();
    bool writeDataNames();
    bool writeDataStructure();
    bool writeInitializer();
    void writeAddNamespaceFunction(const QByteArray &name);
    void writeHex(quint8 number);
    void writeNumber2(quint16 number);
    void writeNumber4(quint32 number);
    void writeChar(char c) { m_out.append(c); }
    void writeByteArray(const QByteArray &);
    void write(const char *, int len);

    RCCFileInfo *m_root;
    QStringList m_fileNames;
    QString m_resourceRoot;
    QString m_initName;
    Format m_format;
    bool m_verbose;
    int m_compressLevel;
    int m_compressThreshold;
    int m_treeOffset;
    int m_namesOffset;
    int m_dataOffset;
    bool m_useNameSpace;
    QStringList m_failedResources;
    QIODevice *m_errorDevice;
    QByteArray m_out;
};

QT_END_NAMESPACE

#endif // RCC_H
