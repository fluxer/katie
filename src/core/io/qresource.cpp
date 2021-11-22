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

#include "qresource.h"
#include "qresource_p.h"
#include "qresource_iterator_p.h"
#include "qset.h"
#include "qhash.h"
#include "qmutex.h"
#include "qdebug.h"
#include "qlocale.h"
#include "qvector.h"
#include "qdatetime.h"
#include "qbytearray.h"
#include "qstringlist.h"
#include "qshareddata.h"
#include "qplatformdefs.h"
#include "qabstractfileengine_p.h"
#include "qfilesystemmetadata_p.h"
#include "qcore_unix_p.h"

//#define DEBUG_RESOURCE_MATCH

QT_BEGIN_NAMESPACE


class QStringSplitter
{
public:
    QStringSplitter(const QString &s)
        : m_string(s), m_pos(0), m_splitChar(QLatin1Char('/'))
    {
    }

    inline bool hasNext() {
        while (m_pos < m_string.length() && m_string[m_pos] == m_splitChar)
            ++m_pos;
        return m_pos < m_string.length();
    }

    inline QStringRef next() {
        int start = m_pos;
        while (m_pos < m_string.length() && m_string[m_pos] != m_splitChar)
            ++m_pos;
        return QStringRef(&m_string, start, m_pos - start);
    }

    QString m_string;
    int m_pos;
    const QChar m_splitChar;
};


//resource glue
class QResourceRoot
{
    enum Flags
    {
        Compressed = 0x01,
        Directory = 0x02
    };
    const uchar *tree, *names, *payloads;
    inline int findOffset(int node) const { return node * 14; } //sizeof each tree element
    int hash(int node) const;
    QString name(int node) const;
    short flags(int node) const;
public:
    mutable QAtomicInt ref;

    inline QResourceRoot(): tree(nullptr), names(nullptr), payloads(nullptr) {}
    inline QResourceRoot(const uchar *t, const uchar *n, const uchar *d) { setSource(t, n, d); }
    virtual ~QResourceRoot() { }
    int findNode(const QString &path, const QLocale &locale=QLocale()) const;
    inline bool isContainer(int node) const { return flags(node) & Directory; }
    inline bool isCompressed(int node) const { return flags(node) & Compressed; }
    const uchar *data(int node, qint64 *size) const;
    QStringList children(int node) const;
    virtual QString mappingRoot() const { return QString(); }
    bool mappingRootSubdir(const QString &path, QString *match=nullptr) const;
    inline bool operator==(const QResourceRoot &other) const
    { return tree == other.tree && names == other.names && payloads == other.payloads; }
    inline bool operator!=(const QResourceRoot &other) const
    { return !operator==(other); }
    enum ResourceRootType { Resource_Builtin, Resource_File, Resource_Buffer };
    virtual ResourceRootType type() const { return Resource_Builtin; }

protected:
    inline void setSource(const uchar *t, const uchar *n, const uchar *d) {
        tree = t;
        names = n;
        payloads = d;
    }
};

Q_DECLARE_TYPEINFO(QResourceRoot, Q_MOVABLE_TYPE);

static std::recursive_mutex qGlobalResourceMutex;

typedef QList<QResourceRoot*> ResourceList;
Q_GLOBAL_STATIC(ResourceList, resourceList)

/*!
    \class QResource
    \brief The QResource class provides an interface for reading directly from resources.

    \ingroup io

    \reentrant
    \since 4.2

    QResource is an object that represents a set of data (and possibly
    children) relating to a single resource entity. QResource gives direct
    access to the bytes in their raw format. In this way direct access
    allows reading data without buffer copying or indirection. Indirection
    is often useful when interacting with the resource entity as if it is a
    file, this can be achieved with QFile. The data and children behind a
    QResource are normally compiled into an application/library, but it is
    also possible to load a resource at runtime. When loaded at run time
    the resource file will be loaded as one big set of data and then given
    out in pieces via references into the resource tree.

    A QResource can either be loaded with an absolute path, either treated
    as a file system rooted with a \c{/} character, or in resource notation
    rooted with a \c{:} character.

    A QResource that is representing a file will have data backing it, this
    data can possibly be compressed, in which case qUncompress() must be
    used to access the real data; this happens implicitly when accessed
    through a QFile. A QResource that is representing a directory will have
    only children and no data.

    \section1 Dynamic Resource Loading

    A resource can be left out of an application's binary and loaded when
    it is needed at run-time by using the registerResource() function. The
    resource file passed into registerResource() must be a binary resource
    as created by rcc. Further information about binary resources can be
    found in \l{The Qt Resource System} documentation.

    This can often be useful when loading a large set of application icons
    that may change based on a setting, or that can be edited by a user and
    later recreated. The resource is immediately loaded into memory, either
    as a result of a single file read operation, or as a memory mapped file.

    This approach can prove to be a significant performance gain as only a
    single file will be loaded, and pieces of data will be given out via the
    path requested in setFileName().

    The unregisterResource() function removes a reference to a particular
    file. If there are QResources that currently reference resources related
    to the unregistered file, they will continue to be valid but the resource
    file itself will be removed from the resource roots, and thus no further
    QResource can be created pointing into this resource data. The resource
    itself will be unmapped from memory when the last QResource that points
    to it is destroyed.

    \sa {The Qt Resource System}, QFile, QDir, QFileInfo
*/

class QResourcePrivate {
public:
    inline QResourcePrivate() { clear(); }
    inline ~QResourcePrivate() { clear(); }

    void ensureInitialized() const;
    void ensureChildren() const;

    bool load(const QString &file);
    void clear();

    QLocale locale;
    QString fileName, absoluteFilePath;
    QList<QResourceRoot*> related;
    bool container;
    mutable bool compressed;
    mutable qint64 size;
    mutable const uchar *data;
    mutable QStringList children;
};

void
QResourcePrivate::clear()
{
    absoluteFilePath.clear();
    container = false;
    compressed = false;
    data = nullptr;
    size = 0;
    children.clear();
    for(int i = 0; i < related.size(); ++i) {
        QResourceRoot *root = related.at(i);
        if(!root->ref.deref())
            delete root;
    }
    related.clear();
}

bool
QResourcePrivate::load(const QString &file)
{
    related.clear();
    std::lock_guard<std::recursive_mutex> lock(qGlobalResourceMutex);
    const ResourceList *list = resourceList();
    QString cleaned = QDir::cleanPath(file);
    for(int i = 0; i < list->size(); ++i) {
        QResourceRoot *res = list->at(i);
        const int node = res->findNode(cleaned, locale);
        if(node != -1) {
            if(related.isEmpty()) {
                container = res->isContainer(node);
                if(!container) {
                    data = res->data(node, &size);
                    compressed = res->isCompressed(node);
                } else {
                    data = nullptr;
                    size = 0;
                    compressed = false;
                }
            } else if(Q_UNLIKELY(res->isContainer(node) != container)) {
                qWarning("QResourceInfo: Resource [%s] has both data and children!", file.toLatin1().constData());
            }
            res->ref.ref();
            related.append(res);
        } else if(res->mappingRootSubdir(file)) {
            container = true;
            data = nullptr;
            size = 0;
            compressed = false;
            res->ref.ref();
            related.append(res);
        }
    }
    return !related.isEmpty();
}

void
QResourcePrivate::ensureInitialized() const
{
    if(!related.isEmpty())
        return;
    QResourcePrivate *that = const_cast<QResourcePrivate *>(this);
    that->absoluteFilePath = fileName;
    if(!that->absoluteFilePath.startsWith(QLatin1String(":/")))
        that->absoluteFilePath.prepend(QLatin1String(":/"));

    if(fileName.startsWith(QLatin1String(":/"))) {
        that->load(fileName.mid(1));
    }
}

void
QResourcePrivate::ensureChildren() const
{
    ensureInitialized();
    if(!children.isEmpty() || !container || related.isEmpty())
        return;

    QString path = absoluteFilePath, k;
    if(path.startsWith(QLatin1Char(':')))
        path = path.mid(1);
    QString cleaned = QDir::cleanPath(path);
    for(int i = 0; i < related.size(); ++i) {
        QResourceRoot *res = related.at(i);
        if(res->mappingRootSubdir(path, &k) && !k.isEmpty()) {
            if(!children.contains(k)) {
                children.append(k);
            }
        } else {
            const int node = res->findNode(cleaned);
            if(node != -1) {
                QStringList related_children = res->children(node);
                for(int kid = 0; kid < related_children.size(); ++kid) {
                    k = related_children.at(kid);
                    if(!children.contains(k)) {
                        children.append(k);
                    }
                }
            }
        }
    }
}

/*!
    Constructs a QResource pointing to \a file. \a locale is used to
    load a specific localization of a resource data.

    \sa QFileInfo, setFileName(), setLocale()
*/

QResource::QResource(const QString &file, const QLocale &locale) : d_ptr(new QResourcePrivate())
{
    Q_D(QResource);
    d->fileName = file;
    d->locale = locale;
}

/*!
    Releases the resources of the QResource object.
*/
QResource::~QResource()
{
    delete d_ptr;
}

/*!
    Sets a QResource to only load the localization of resource to for \a
    locale. If a resource for the specific locale is not found then the
    C locale is used.

    \sa setFileName()
*/

void QResource::setLocale(const QLocale &locale)
{
    Q_D(QResource);
    d->clear();
    d->locale = locale;
}

/*!
    Returns the locale used to locate the data for the QResource.
*/

QLocale QResource::locale() const
{
    Q_D(const QResource);
    return d->locale;
}

/*!
    Sets a QResource to point to \a file. \a file should be absolute.

    \sa absoluteFilePath()
*/

void QResource::setFileName(const QString &file)
{
    Q_D(QResource);
    d->clear();
    d->fileName = file;
}

/*!
    Returns the full path to the file that this QResource represents as it
    was passed.

    \sa absoluteFilePath()
*/

QString QResource::fileName() const
{
    Q_D(const QResource);
    d->ensureInitialized();
    return d->fileName;
}

/*!
    Returns the real path that this QResource represents.

    \sa fileName()
*/

QString QResource::absoluteFilePath() const
{
    Q_D(const QResource);
    d->ensureInitialized();
    return d->absoluteFilePath;
}

/*!
    Returns true if the resource really exists in the resource hierarchy,
    false otherwise.

*/

bool QResource::isValid() const
{
    Q_D(const QResource);
    d->ensureInitialized();
    return !d->related.isEmpty();
}

/*!
    \fn bool QResource::isFile() const

    Returns true if the resource represents a file and thus has data
    backing it, false if it represents a directory.

    \sa isDir()
*/


/*!
    Returns true if the resource represents a file and the data backing it
    is in a compressed format, false otherwise.

    \sa data(), isFile()
*/

bool QResource::isCompressed() const
{
    Q_D(const QResource);
    d->ensureInitialized();
    return d->compressed;
}

/*!
    Returns the size of the data backing the resource.

    \sa data(), isFile()
*/

qint64 QResource::size() const
{
    Q_D(const QResource);
    d->ensureInitialized();
    return d->size;
}

/*!
    Returns direct access to a read only segment of data that this resource
    represents. If the resource is compressed the data returns is
    compressed and qUncompress() must be used to access the data. If
    the resource is a directory 0 is returned.

    \sa size(), isCompressed(), isFile()
*/

const uchar *QResource::data() const
{
    Q_D(const QResource);
    d->ensureInitialized();
    return d->data;
}

/*!
    Returns true if the resource represents a directory and thus may have
    children() in it, false if it represents a file.

    \sa isFile()
*/

bool QResource::isDir() const
{
    Q_D(const QResource);
    d->ensureInitialized();
    return d->container;
}

/*!
    Returns a list of all resources in this directory, if the resource
    represents a file the list will be empty.

    \sa isDir()
*/

QStringList QResource::children() const
{
    Q_D(const QResource);
    d->ensureChildren();
    return d->children;
}

inline int QResourceRoot::hash(int node) const
{
    if(!node) //root
        return 0;
    const int offset = findOffset(node);
    int name_offset = (tree[offset+0] << 24) + (tree[offset+1] << 16) +
                      (tree[offset+2] << 8) + (tree[offset+3] << 0);
    name_offset += 2; //jump past name length
    return (names[name_offset+0] << 24) + (names[name_offset+1] << 16) +
           (names[name_offset+2] << 8) + (names[name_offset+3] << 0);
}
inline QString QResourceRoot::name(int node) const
{
    if(!node) // root
        return QString();
    const int offset = findOffset(node);

    int name_offset = (tree[offset+0] << 24) + (tree[offset+1] << 16) +
                      (tree[offset+2] << 8) + (tree[offset+3] << 0);
    const short name_length = (names[name_offset+0] << 8) +
                              (names[name_offset+1] << 0);
    name_offset += 2;
    name_offset += 4; //jump past hash

    QString ret(name_length, Qt::Uninitialized);
    QChar *strData = ret.data();
    for(int i = 0; i < name_length*2; i+=2) {
        QChar c(names[name_offset+i+1], names[name_offset+i]);
        *strData = c;
        ++strData;
    }
    return ret;
}

int QResourceRoot::findNode(const QString &_path, const QLocale &locale) const
{
    QString path = _path;
    {
        QString root = mappingRoot();
        if(!root.isEmpty()) {
            if(root == path) {
                path = QLatin1Char('/');
            } else {
                if(!root.endsWith(QLatin1Char('/')))
                    root += QLatin1Char('/');
                if(path.size() >= root.size() && path.startsWith(root))
                    path = path.mid(root.length()-1);
                if(path.isEmpty())
                    path = QLatin1Char('/');
            }
        }
    }
#ifdef DEBUG_RESOURCE_MATCH
    qDebug() << "!!!!" << "START" << path << locale.country() << locale.language();
#endif

    if(path == QLatin1String("/"))
        return 0;

    //the root node is always first
    int child_count = (tree[6] << 24) + (tree[7] << 16) +
                      (tree[8] << 8) + (tree[9] << 0);
    int child       = (tree[10] << 24) + (tree[11] << 16) +
                      (tree[12] << 8) + (tree[13] << 0);

    //now iterate up the tree
    int node = -1;

    QStringSplitter splitter(path);
    while (child_count && splitter.hasNext()) {
        QStringRef segment = splitter.next();

#ifdef DEBUG_RESOURCE_MATCH
        qDebug() << "  CHILDREN" << segment;
        for(int j = 0; j < child_count; ++j) {
            qDebug() << "   " << child+j << " :: " << name(child+j);
        }
#endif
        const int h = qHash(segment);

        //do the binary search for the hash
        int l = 0, r = child_count-1;
        int sub_node = (l+r+1)/2;
        while(r != l) {
            const int sub_node_hash = hash(child+sub_node);
            if(h == sub_node_hash)
                break;
            else if(h < sub_node_hash)
                r = sub_node - 1;
            else
                l = sub_node;
            sub_node = (l + r + 1) / 2;
        }
        sub_node += child;

        //now do the "harder" compares
        bool found = false;
        if(hash(sub_node) == h) {
            while(sub_node > child && hash(sub_node-1) == h) //backup for collisions
                --sub_node;
            for(; sub_node < child+child_count && hash(sub_node) == h; ++sub_node) { //here we go...
                if(name(sub_node) == segment) {
                    found = true;
                    int offset = findOffset(sub_node);
#ifdef DEBUG_RESOURCE_MATCH
                    qDebug() << "  TRY" << sub_node << name(sub_node) << offset;
#endif
                    offset += 4;  //jump past name

                    const short flags = (tree[offset+0] << 8) +
                                        (tree[offset+1] << 0);
                    offset += 2;

                    if(!splitter.hasNext()) {
                        if(!(flags & Directory)) {
                            const short country = (tree[offset+0] << 8) +
                                                  (tree[offset+1] << 0);
                            offset += 2;

                            const short language = (tree[offset+0] << 8) +
                                                   (tree[offset+1] << 0);
                            offset += 2;
#ifdef DEBUG_RESOURCE_MATCH
                            qDebug() << "    " << "LOCALE" << country << language;
#endif
                            if(country == locale.country() && language == locale.language()) {
#ifdef DEBUG_RESOURCE_MATCH
                                qDebug() << "!!!!" << "FINISHED" << __LINE__ << sub_node;
#endif
                                return sub_node;
                            } else if((country == QLocale::AnyCountry && language == locale.language()) ||
                                      (country == QLocale::AnyCountry && language == QLocale::C && node == -1)) {
                                node = sub_node;
                            }
                            continue;
                        } else {
#ifdef DEBUG_RESOURCE_MATCH
                            qDebug() << "!!!!" << "FINISHED" << __LINE__ << sub_node;
#endif

                            return sub_node;
                        }
                    }

                    if(!(flags & Directory))
                        return -1;

                    child_count = (tree[offset+0] << 24) + (tree[offset+1] << 16) +
                                  (tree[offset+2] << 8) + (tree[offset+3] << 0);
                    offset += 4;
                    child = (tree[offset+0] << 24) + (tree[offset+1] << 16) +
                            (tree[offset+2] << 8) + (tree[offset+3] << 0);
                    break;
                }
            }
        }
        if(!found)
            break;
    }
#ifdef DEBUG_RESOURCE_MATCH
    qDebug() << "!!!!" << "FINISHED" << __LINE__ << node;
#endif
    return node;
}
short QResourceRoot::flags(int node) const
{
    if(node == -1)
        return 0;
    const int offset = findOffset(node) + 4; //jump past name
    return (tree[offset+0] << 8) + (tree[offset+1] << 0);
}
const uchar *QResourceRoot::data(int node, qint64 *size) const
{
    if(node == -1) {
        *size = 0;
        return nullptr;
    }
    int offset = findOffset(node) + 4; //jump past name

    const short flags = (tree[offset+0] << 8) + (tree[offset+1] << 0);
    offset += 2;

    offset += 4; //jump past locale

    if(!(flags & Directory)) {
        const int data_offset = (tree[offset+0] << 24) + (tree[offset+1] << 16) +
                                (tree[offset+2] << 8) + (tree[offset+3] << 0);
        const uint data_length = (payloads[data_offset+0] << 24) + (payloads[data_offset+1] << 16) +
                                 (payloads[data_offset+2] << 8) + (payloads[data_offset+3] << 0);
        const uchar *ret = payloads+data_offset+4;
        *size = data_length;
        return ret;
    }
    *size = 0;
    return nullptr;
}
QStringList QResourceRoot::children(int node) const
{
    if(node == -1)
        return QStringList();
    int offset = findOffset(node) + 4; //jump past name

    const short flags = (tree[offset+0] << 8) + (tree[offset+1] << 0);
    offset += 2;

    QStringList ret;
    if(flags & Directory) {
        const int child_count = (tree[offset+0] << 24) + (tree[offset+1] << 16) +
                                (tree[offset+2] << 8) + (tree[offset+3] << 0);
        offset += 4;
        const int child_off = (tree[offset+0] << 24) + (tree[offset+1] << 16) +
                              (tree[offset+2] << 8) + (tree[offset+3] << 0);
        ret.reserve(child_count);
        for(int i = child_off; i < child_off+child_count; ++i)
            ret << name(i);
    }
    return ret;
}
bool QResourceRoot::mappingRootSubdir(const QString &path, QString *match) const
{
    const QString root = mappingRoot();
    if(!root.isEmpty()) {
        const QStringList root_segments = root.split(QLatin1Char('/'), QString::SkipEmptyParts),
                          path_segments = path.split(QLatin1Char('/'), QString::SkipEmptyParts);
        if(path_segments.size() <= root_segments.size()) {
            int matched = 0;
            for(int i = 0; i < path_segments.size(); ++i) {
                if(root_segments[i] != path_segments[i])
                    break;
                ++matched;
            }
            if(matched == path_segments.size()) {
                if(match && root_segments.size() > matched)
                    *match = root_segments.at(matched);
                return true;
            }
        }
    }
    return false;
}

Q_CORE_EXPORT bool qRegisterResourceData(int version, const unsigned char *tree,
                                         const unsigned char *name, const unsigned char *data)
{
    std::lock_guard<std::recursive_mutex> lock(qGlobalResourceMutex);
    if(version == Q_RCC_OUTPUT_REVISION && resourceList()) {
        bool found = false;
        QResourceRoot res(tree, name, data);
        for(int i = 0; i < resourceList()->size(); ++i) {
            if(*resourceList()->at(i) == res) {
                found = true;
                break;
            }
        }
        if(!found) {
            QResourceRoot *root = new QResourceRoot(tree, name, data);
            root->ref.ref();
            resourceList()->append(root);
        }
        return true;
    }
    return false;
}

Q_CORE_EXPORT bool qUnregisterResourceData(int version, const unsigned char *tree,
                                           const unsigned char *name, const unsigned char *data)
{
    std::lock_guard<std::recursive_mutex> lock(qGlobalResourceMutex);
    if(version == Q_RCC_OUTPUT_REVISION && resourceList()) {
        QResourceRoot res(tree, name, data);
        for(int i = 0; i < resourceList()->size(); ) {
            if(*resourceList()->at(i) == res) {
                QResourceRoot *root = resourceList()->takeAt(i);
                if(!root->ref.deref())
                    delete root;
            } else {
                ++i;
            }
        }
        return true;
    }
    return false;
}

//run time resource creation

class QDynamicBufferResourceRoot: public QResourceRoot
{
    QString root;
    const uchar *buffer;

public:
    inline QDynamicBufferResourceRoot(const QString &_root) : root(_root), buffer(nullptr) { }
    inline ~QDynamicBufferResourceRoot() { }
    inline const uchar *mappingBuffer() const { return buffer; }
    virtual QString mappingRoot() const { return root; }
    virtual ResourceRootType type() const { return Resource_Buffer; }

    bool registerSelf(const uchar *b) {
        //magic number
        if(b[0] != 'q' || b[1] != 'r' || b[2] != 'e' || b[3] != 's') {
            return false;
        }
        //setup the data now
        int offset = 4;

        const int version = (b[offset+0] << 24) + (b[offset+1] << 16) +
                         (b[offset+2] << 8) + (b[offset+3] << 0);
        offset += 4;

        const int tree_offset = (b[offset+0] << 24) + (b[offset+1] << 16) +
                                (b[offset+2] << 8) + (b[offset+3] << 0);
        offset += 4;

        const int data_offset = (b[offset+0] << 24) + (b[offset+1] << 16) +
                                (b[offset+2] << 8) + (b[offset+3] << 0);
        offset += 4;

        const int name_offset = (b[offset+0] << 24) + (b[offset+1] << 16) +
                                (b[offset+2] << 8) + (b[offset+3] << 0);

        if(version == Q_RCC_OUTPUT_REVISION) {
            buffer = b;
            setSource(b+tree_offset, b+name_offset, b+data_offset);
            return true;
        }
        return false;
    }
};


class QDynamicFileResourceRoot: public QDynamicBufferResourceRoot
{
    QString fileName;

public:
    inline QDynamicFileResourceRoot(const QString &_root) : QDynamicBufferResourceRoot(_root) { }
    ~QDynamicFileResourceRoot() {
        delete [] (uchar *)mappingBuffer();
    }
    QString mappingFile() const { return fileName; }
    virtual ResourceRootType type() const { return Resource_File; }

    bool registerSelf(const QString &f) {
        QFile file(f);
        if (file.open(QIODevice::ReadOnly)) {
            qint64 data_len = file.size();
            uchar *data = new uchar[data_len];
            if (data_len != file.read((char*)data, data_len)) {
                delete [] data;
                return false;
            } else if (QDynamicBufferResourceRoot::registerSelf(data)) {
                fileName = f;
                return true;
            }
        }
        return false;
    }
};

/*!
   \fn bool QResource::registerResource(const QString &rccFileName, const QString &mapRoot)

   Registers the resource with the given \a rccFileName at the location in the
   resource tree specified by \a mapRoot, and returns true if the file is
   successfully opened; otherwise returns false.

   \sa unregisterResource()
*/

bool
QResource::registerResource(const QString &rccFilename, const QString &resourceRoot)
{
    if(Q_UNLIKELY(!resourceRoot.startsWith(QLatin1String(":/")))) {
        qWarning("QResource::registerResource: Registering a resource [%s] must be rooted in an absolute path (start with :/) [%s]",
                 rccFilename.toLocal8Bit().data(), resourceRoot.toLocal8Bit().data());
        return false;
    }

    QDynamicFileResourceRoot *root = new QDynamicFileResourceRoot(resourceRoot);
    if(root->registerSelf(rccFilename)) {
        root->ref.ref();
        std::lock_guard<std::recursive_mutex> lock(qGlobalResourceMutex);
        resourceList()->append(root);
        return true;
    }
    delete root;
    return false;
}

/*!
  \fn bool QResource::unregisterResource(const QString &rccFileName, const QString &mapRoot)

  Unregisters the resource with the given \a rccFileName at the location in
  the resource tree specified by \a mapRoot, and returns true if the
  resource is successfully unloaded and no references exist for the
  resource; otherwise returns false.

  \sa registerResource()
*/

bool
QResource::unregisterResource(const QString &rccFilename, const QString &resourceRoot)
{
    std::lock_guard<std::recursive_mutex> lock(qGlobalResourceMutex);
    ResourceList *list = resourceList();
    for(int i = 0; i < list->size(); ++i) {
        QResourceRoot *res = list->at(i);
        if(res->type() == QResourceRoot::Resource_File) {
            QDynamicFileResourceRoot *root = reinterpret_cast<QDynamicFileResourceRoot*>(res);
            if(root->mappingFile() == rccFilename && root->mappingRoot() == resourceRoot) {
                resourceList()->removeAt(i);
                if(!root->ref.deref()) {
                    delete root;
                    return true;
                }
                return false;
            }
        }
    }
    return false;
}


/*!
   \fn bool QResource::registerResource(const uchar *rccData, const QString &mapRoot)
   \since 4.3

   Registers the resource with the given \a rccData at the location in the
   resource tree specified by \a mapRoot, and returns true if the file is
   successfully opened; otherwise returns false.

   \warning The data must remain valid throughout the life of any QFile
   that may reference the resource data.

   \sa unregisterResource()
*/

bool
QResource::registerResource(const uchar *rccData, const QString &resourceRoot)
{
    if(Q_UNLIKELY(!resourceRoot.startsWith(QLatin1String(":/")))) {
        qWarning("QResource::registerResource: Registering a resource [%p] must be rooted in an absolute path (start with :/) [%s]",
                 rccData, resourceRoot.toLocal8Bit().data());
        return false;
    }

    QDynamicBufferResourceRoot *root = new QDynamicBufferResourceRoot(resourceRoot);
    if(root->registerSelf(rccData)) {
        root->ref.ref();
        std::lock_guard<std::recursive_mutex> lock(qGlobalResourceMutex);
        resourceList()->append(root);
        return true;
    }
    delete root;
    return false;
}

/*!
  \fn bool QResource::unregisterResource(const uchar *rccData, const QString &mapRoot)
  \since 4.3

  Unregisters the resource with the given \a rccData at the location in the
  resource tree specified by \a mapRoot, and returns true if the resource is
  successfully unloaded and no references exist into the resource; otherwise returns false.

  \sa registerResource()
*/

bool
QResource::unregisterResource(const uchar *rccData, const QString &resourceRoot)
{
    std::lock_guard<std::recursive_mutex> lock(qGlobalResourceMutex);
    ResourceList *list = resourceList();
    for(int i = 0; i < list->size(); ++i) {
        QResourceRoot *res = list->at(i);
        if(res->type() == QResourceRoot::Resource_Buffer) {
            QDynamicBufferResourceRoot *root = reinterpret_cast<QDynamicBufferResourceRoot*>(res);
            if(root->mappingBuffer() == rccData && root->mappingRoot() == resourceRoot) {
                resourceList()->removeAt(i);
                if(!root->ref.deref()) {
                    delete root;
                    return true;
                }
                return false;
            }
        }
    }
    return false;
}

//resource engine
class QResourceFileEnginePrivate : public QAbstractFileEnginePrivate
{
protected:
    Q_DECLARE_PUBLIC(QResourceFileEngine)
private:
    uchar *map(qint64 offset, qint64 size);
    bool unmap(uchar *ptr);
    qint64 offset;
    QResource resource;
    QByteArray uncompressed;
protected:
    QResourceFileEnginePrivate() : offset(0) { }
};

bool QResourceFileEngine::mkdir(const QString &, bool) const
{
    return false;
}

bool QResourceFileEngine::rmdir(const QString &, bool) const
{
    return false;
}

bool QResourceFileEngine::setSize(qint64)
{
    return false;
}

bool QResourceFileEngine::caseSensitive() const
{
    return true;
}

QResourceFileEngine::QResourceFileEngine(const QString &file) :
    QAbstractFileEngine(*new QResourceFileEnginePrivate)
{
    Q_D(QResourceFileEngine);
    d->resource.setFileName(file);
    if(d->resource.isCompressed() && d->resource.size()) {
        d->uncompressed = qUncompress(reinterpret_cast<const char*>(d->resource.data()), d->resource.size());
    }
}

QResourceFileEngine::~QResourceFileEngine()
{
}

void QResourceFileEngine::setFileName(const QString &file)
{
    Q_D(QResourceFileEngine);
    d->resource.setFileName(file);
}

bool QResourceFileEngine::open(QIODevice::OpenMode flags)
{
    Q_D(QResourceFileEngine);
    if (Q_UNLIKELY(d->resource.fileName().isEmpty())) {
        qWarning("QResourceFileEngine::open: Missing file name");
        return false;
    }
    if(flags & QIODevice::WriteOnly)
        return false;
    if(!d->resource.isValid())
       return false;
    return true;
}

bool QResourceFileEngine::close()
{
    Q_D(QResourceFileEngine);
    d->offset = 0;
    d->uncompressed.clear();
    return true;
}

bool QResourceFileEngine::flush()
{
    return true;
}

qint64 QResourceFileEngine::read(char *data, qint64 len)
{
    Q_D(QResourceFileEngine);
    if(len > size()-d->offset)
        len = size()-d->offset;
    if(len <= 0)
        return 0;
    if(d->resource.isCompressed())
        memcpy(data, d->uncompressed.constData()+d->offset, len);
    else
        memcpy(data, d->resource.data()+d->offset, len);
    d->offset += len;
    return len;
}

qint64 QResourceFileEngine::write(const char *, qint64)
{
    return -1;
}

bool QResourceFileEngine::remove()
{
    return false;
}

bool QResourceFileEngine::copy(const QString &)
{
    return false;
}

bool QResourceFileEngine::rename(const QString &)
{
    return false;
}

bool QResourceFileEngine::link(const QString &)
{
    return false;
}

qint64 QResourceFileEngine::size() const
{
    Q_D(const QResourceFileEngine);
    if(!d->resource.isValid())
        return 0;
    if(d->resource.isCompressed())
        return d->uncompressed.size();
    return d->resource.size();
}

qint64 QResourceFileEngine::pos() const
{
    Q_D(const QResourceFileEngine);
    return d->offset;
}

bool QResourceFileEngine::atEnd() const
{
    Q_D(const QResourceFileEngine);
    if(!d->resource.isValid())
        return true;
    return d->offset == size();
}

bool QResourceFileEngine::seek(qint64 pos)
{
    Q_D(QResourceFileEngine);
    if(!d->resource.isValid())
        return false;

    if(d->offset > size())
        return false;
    d->offset = pos;
    return true;
}

bool QResourceFileEngine::isSequential() const
{
    return false;
}

QAbstractFileEngine::FileFlags QResourceFileEngine::fileFlags(QAbstractFileEngine::FileFlags type) const
{
    Q_D(const QResourceFileEngine);
    QAbstractFileEngine::FileFlags ret = 0;
    if(!d->resource.isValid())
        return ret;

    if(type & PermsMask)
        ret |= QAbstractFileEngine::FileFlags(ReadOwnerPerm|ReadUserPerm|ReadGroupPerm|ReadOtherPerm);
    if(type & TypesMask) {
        if(d->resource.isDir())
            ret |= DirectoryType;
        else
            ret |= FileType;
    }
    if(type & FlagsMask) {
        ret |= ExistsFlag;
        if(d->resource.absoluteFilePath() == QLatin1String(":/"))
            ret |= RootFlag;
    }
    return ret;
}

bool QResourceFileEngine::setPermissions(uint)
{
    return false;
}

QString QResourceFileEngine::fileName(FileName file) const
{
    Q_D(const QResourceFileEngine);
    const QString filepath = d->resource.fileName();
    if(file == BaseName) {
        int slash = filepath.lastIndexOf(QLatin1Char('/'));
        if (slash == -1)
            return filepath;
        return filepath.mid(slash + 1);
    } else if(file == PathName || file == AbsolutePathName) {
        const QString path = (file == AbsolutePathName) ? d->resource.absoluteFilePath() : filepath;
        const int slash = path.lastIndexOf(QLatin1Char('/'));
        if (slash <= 1 || slash == path.size())
            return QLatin1String(":/");
        return path.left(slash);
    } else if(file == CanonicalName || file == CanonicalPathName) {
        const QString absoluteFilePath = d->resource.absoluteFilePath();
        if(file == CanonicalPathName) {
            const int slash = absoluteFilePath.lastIndexOf(QLatin1Char('/'));
            if (slash != -1)
                return absoluteFilePath.left(slash);
        }
        return absoluteFilePath;
    }
    return filepath;
}

bool QResourceFileEngine::isRelativePath() const
{
    return false;
}

uint QResourceFileEngine::ownerId(FileOwner) const
{
    return QFileSystemMetaData::nobodyID;
}

QString QResourceFileEngine::owner(FileOwner) const
{
    return QString();
}

QDateTime QResourceFileEngine::fileTime(FileTime) const
{
    return QDateTime();
}

/*!
    \internal
*/
QAbstractFileEngine::Iterator *QResourceFileEngine::beginEntryList(QDir::Filters filters,
                                                                   const QStringList &filterNames)
{
    return new QResourceFileEngineIterator(filters, filterNames);
}

bool QResourceFileEngine::extension(Extension extension, const ExtensionOption *option, ExtensionReturn *output)
{
    Q_D(QResourceFileEngine);
    if (extension == MapExtension) {
        const MapExtensionOption *options = (MapExtensionOption*)(option);
        MapExtensionReturn *returnValue = static_cast<MapExtensionReturn*>(output);
        returnValue->address = d->map(options->offset, options->size);
        return (returnValue->address != nullptr);
    }
    if (extension == UnMapExtension) {
        UnMapExtensionOption *options = (UnMapExtensionOption*)option;
        return d->unmap(options->address);
    }
    return false;
}

bool QResourceFileEngine::supportsExtension(Extension extension) const
{
    return (extension == UnMapExtension || extension == MapExtension);
}

uchar *QResourceFileEnginePrivate::map(qint64 offset, qint64 size)
{
    Q_Q(QResourceFileEngine);
    if (offset < 0 || size <= 0 || !resource.isValid() || offset + size > resource.size()) {
        q->setError(QFile::UnspecifiedError, QString());
        return nullptr;
    }
    uchar *address = const_cast<uchar *>(resource.data());
    return (address + offset);
}

bool QResourceFileEnginePrivate::unmap(uchar *ptr)
{
    Q_UNUSED(ptr);
    return true;
}

QT_END_NAMESPACE
