
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

#include "qconfig.h"
#include "qplatformdefs.h"
#include "qlibrary.h"

#ifndef QT_NO_LIBRARY

#include "qlibrary_p.h"
#include "qstringlist.h"
#include "qfile.h"
#include "qfileinfo.h"
#include "qmutex.h"
#include "qmap.h"
#include "qsettings.h"
#include "qdatetime.h"
#include "qcoreapplication_p.h"
#include "qdebug.h"
#include "qvector.h"
#include "qdir.h"
#include "qfilesystementry_p.h"
#include "qcore_unix_p.h"

#include <errno.h>

#ifndef QT_NO_PLUGIN_CHECK
#  include <elf.h>
#  if QT_POINTER_SIZE == 8
#    define QT_ELF_EHDR_TYPE Elf64_Ehdr
#    define QT_ELF_SHDR_TYPE Elf64_Shdr
#    define QT_ELF_CLASS_TYPE ELFCLASS64
#  else
#    define QT_ELF_EHDR_TYPE Elf32_Ehdr
#    define QT_ELF_SHDR_TYPE Elf32_Shdr
#    define QT_ELF_CLASS_TYPE ELFCLASS32
#  endif
#endif

QT_BEGIN_NAMESPACE

Q_GLOBAL_STATIC(QMutex, qt_library_mutex)

/*!
    \class QLibrary
    \reentrant
    \brief The QLibrary class loads shared libraries at runtime.


    \ingroup plugins

    An instance of a QLibrary object operates on a single shared
    object file (which we call a "library", but is also known as a
    "DLL"). A QLibrary provides access to the functionality in the
    library in a platform independent way. You can either pass a file
    name in the constructor, or set it explicitly with setFileName().
    When loading the library, QLibrary searches in all the
    system-specific library locations (e.g. \c LD_LIBRARY_PATH on
    Unix), unless the file name has an absolute path. If the file
    cannot be found, QLibrary tries the name with different
    platform-specific file suffixes, like ".so" on Unix. This makes
    it possible to specify shared libraries that are only identified
    by their basename (i.e. without their suffix), so the same code
    will work on different operating systems.

    The most important functions are load() to dynamically load the
    library file, isLoaded() to check whether loading was successful,
    and resolve() to resolve a symbol in the library. The resolve()
    function implicitly tries to load the library if it has not been
    loaded yet. Multiple instances of QLibrary can be used to access
    the same physical library. Once loaded, libraries remain in memory
    until the application terminates. You can attempt to unload a
    library using unload(), but if other instances of QLibrary are
    using the same library, the call will fail, and unloading will
    only happen when every instance has called unload().

    A typical use of QLibrary is to resolve an exported symbol in a
    library, and to call the C function that this symbol represents.
    This is called "explicit linking" in contrast to "implicit
    linking", which is done by the link step in the build process when
    linking an executable against a library.

    The following code snippet loads a library, resolves the symbol
    "mysymbol", and calls the function if everything succeeded. If
    something goes wrong, e.g. the library file does not exist or the
    symbol is not defined, the function pointer will be 0 and won't be
    called.

    \snippet doc/src/snippets/code/src_corelib_plugin_qlibrary.cpp 0

    The symbol must be exported as a C function from the library for
    resolve() to work. This means that the function must be wrapped in
    an \c{extern "C"} block if the library is compiled with a C++
    compiler. On Windows, this also requires the use of a \c dllexport
    macro; see resolve() for the details of how this is done. For
    convenience, there is a static resolve() function which you can
    use if you just want to call a function in a library without
    explicitly loading the library first:

    \snippet doc/src/snippets/code/src_corelib_plugin_qlibrary.cpp 1

    \sa QPluginLoader
*/

/*!
    \enum QLibrary::LoadHint

    This enum describes the possible hints that can be used to change the way
    libraries are handled when they are loaded. These values indicate how
    symbols are resolved when libraries are loaded, and are specified using
    the setLoadHints() function.

    \value ResolveAllSymbolsHint
    Causes all symbols in a library to be resolved when it is loaded, not
    simply when resolve() is called.
    \value ExportExternalSymbolsHint
    Exports unresolved and external symbols in the library so that they can be
    resolved in other dynamically-loaded libraries loaded later.

    \sa loadHints
*/

#ifndef QT_NO_PLUGIN_CHECK
/*
  This opens the specified library, mmaps it into memory, and searches
  for the plugin seciton.  The advantage of this approach is that
  we can get the verification data without have to actually load the library.
  This lets us detect mismatches more safely.

  Returns false if version information is not present, or if the
                information could not be read.
  Returns  true if version information is present and successfully read.
*/
static bool qt_unix_query(const QString &library, uint *version, QLibraryPrivate *lib)
{
    QFile file(library);
    if (!file.open(QIODevice::ReadOnly)) {
        lib->errorString = file.errorString();
        if (qt_debug_component()) {
            qWarning("%s: %s", QFile::encodeName(library).data(),
                qPrintable(qt_error_string(errno)));
        }
        return false;
    }

    qint64 datalen = file.size();
    const char *filedata = reinterpret_cast<char*>(file.map(0, file.size()));
    if (filedata == 0) {
        // try reading the data into memory instead
        const QByteArray data = file.readAll();
        filedata = data.constData();
        datalen = data.size();
    }

    // basic ELF checks to avoid crashing
    if (datalen < (EI_CLASS + 1) || qstrncmp(filedata, ELFMAG, SELFMAG) != 0) {
        lib->errorString = QLibrary::tr("'%1' is not ELF file").arg(library);
        return false;
    } else if (filedata[EI_CLASS] != QT_ELF_CLASS_TYPE) {
        lib->errorString = QLibrary::tr("ELF class mismatch in '%1'").arg(library);
        return false;
    }

    // ELF binaries build with GNU or Clang have .ktplugin section
    bool ret = false;
    QT_ELF_EHDR_TYPE *ehdr = (QT_ELF_EHDR_TYPE*)(filedata);
    QT_ELF_SHDR_TYPE *shdr = (QT_ELF_SHDR_TYPE*)(filedata + ehdr->e_shoff);

    QT_ELF_SHDR_TYPE *sh_strtab = &shdr[ehdr->e_shstrndx];
    const char *const sh_strtab_p = filedata + sh_strtab->sh_offset;

    for (int i = 0; i < ehdr->e_shnum; ++i) {
        const char* sectioname = sh_strtab_p + shdr[i].sh_name;
        if (qstrcmp(sectioname, ".ktplugin") == 0) {
            ret = true;
            // compatiblity between releases is not guratneed thus no version matching is done
            *version = QT_VERSION;
            break;
        }
    }


    if (!ret)
        lib->errorString = QLibrary::tr("Plugin verification data mismatch in '%1'").arg(library);
    file.close();

    return ret;
}
#endif // QT_NO_PLUGIN_CHECK

typedef QMap<QString, QLibraryPrivate*> LibraryMap;

struct LibraryData {
    LibraryMap libraryMap;
    QSet<QLibraryPrivate*> loadedLibs;
};

Q_GLOBAL_STATIC(LibraryData, libraryData)

static LibraryMap *libraryMap()
{
    LibraryData *data = libraryData();
    return data ? &data->libraryMap : nullptr;
}

QLibraryPrivate::QLibraryPrivate(const QString &canonicalFileName)
    : did_load(false), pHnd(0), fileName(canonicalFileName),
     instance(0), qt_version(0), libraryRefCount(1), libraryUnloadCount(0),
     pluginState(MightBeAPlugin)
{
    libraryMap()->insert(canonicalFileName, this);
}

QLibraryPrivate *QLibraryPrivate::findOrCreate(const QString &fileName, const QString &version)
{
    QMutexLocker locker(qt_library_mutex());
    if (QLibraryPrivate *lib = libraryMap()->value(fileName)) {
        lib->libraryRefCount.ref();
        return lib;
    }

    QFileSystemEntry fsEntry(fileName);

    QString path = fsEntry.path();
    QString name = fsEntry.fileName();
    if (path == QLatin1String(".") && !fileName.startsWith(path))
        path.clear();
    else
        path += QLatin1Char('/');

    QStringList suffixes;
    suffixes << QLatin1String("");
    QStringList prefixes;
    prefixes << QLatin1String("") << QLatin1String("lib");

    if (path.isEmpty()) {
        foreach(const QString &libpath, QCoreApplication::libraryPaths()) {
            prefixes << (libpath + QLatin1Char('/')) << (libpath + QLatin1String("/lib"));
        }
    }

    if (!version.isEmpty()) {
        suffixes << QString::fromLatin1(".so.%1").arg(version);
    } else {
        suffixes << QLatin1String(".so");
    }

    foreach (const QString &prefix, prefixes) {
        foreach (const QString &suffix, suffixes) {
            if (!prefix.isEmpty() && name.startsWith(prefix))
                continue;
            if (!suffix.isEmpty() && name.endsWith(suffix))
                continue;
            const QString attempt = path + prefix + name + suffix;
            const QStatInfo statinfo(attempt);
            if (statinfo.isFile()) {
                return new QLibraryPrivate(attempt);
            }
        }
    }

    return new QLibraryPrivate(fileName);
}

QLibraryPrivate::~QLibraryPrivate()
{
    LibraryMap * const map = libraryMap();
    if (map) {
        QLibraryPrivate *that = map->take(fileName);
        Q_ASSERT(this == that);
        Q_UNUSED(that);
    }
}

void *QLibraryPrivate::resolve(const char *symbol)
{
    if (!pHnd)
        return nullptr;
    return resolve_sys(symbol);
}


bool QLibraryPrivate::load()
{
    libraryUnloadCount.ref();
    if (pHnd)
        return true;
    if (fileName.isEmpty())
        return false;

    bool ret = load_sys();
    if (ret) {
        //when loading a library we add a reference to it so that the QLibraryPrivate won't get deleted
        //this allows to unload the library at a later time
        if (LibraryData *lib = libraryData()) {
            lib->loadedLibs += this;
            libraryRefCount.ref();
        }
    }

    return ret;
}

bool QLibraryPrivate::unload()
{
    if (!pHnd)
        return false;
    if (!libraryUnloadCount.deref()) { // only unload if ALL QLibrary instance wanted to
        delete inst.data();
        if  (unload_sys()) {
            if (qt_debug_component())
                qWarning() << "QLibraryPrivate::unload succeeded on" << fileName;
            //when the library is unloaded, we release the reference on it so that 'this'
            //can get deleted
            if (LibraryData *lib = libraryData()) {
                if (lib->loadedLibs.remove(this))
                    libraryRefCount.deref();
            }
            pHnd = 0;
        }
    }

    return (pHnd == 0);
}

void QLibraryPrivate::release()
{
    QMutexLocker locker(qt_library_mutex());
    if (!libraryRefCount.deref())
        delete this;
}

bool QLibraryPrivate::loadPlugin()
{
    if (instance) {
        libraryUnloadCount.ref();
        return true;
    }
    if (pluginState == IsNotAPlugin)
        return false;
    if (load()) {
        instance = (QtPluginInstanceFunction)resolve("kt_plugin_instance");
        return instance;
    }
    if (qt_debug_component())
        qWarning() << "QLibraryPrivate::loadPlugin failed on" << fileName << ":" << errorString;
    pluginState = IsNotAPlugin;
    return false;
}

/*!
    Returns true if \a fileName has a valid suffix for a loadable
    library; otherwise returns false.

    \table
    \header \i Platform \i Valid suffixes
    \row \i Unix/Linux  \i \c .so
    \endtable

    Trailing versioning numbers on Unix are ignored.
 */
bool QLibrary::isLibrary(const QString &fileName)
{
    QString completeSuffix = QFileInfo(fileName).completeSuffix();
    if (completeSuffix.isEmpty())
        return false;

    // Examples of valid library names:
    //  libfoo.so
    //  libfoo.so.0
    //  libfoo.so.0.3
    //  libfoo-0.3.so
    //  libfoo-0.3.so.0.3.0

    foreach (const QString &suffix, completeSuffix.split(QLatin1Char('.'))) {
        if (suffix == QLatin1String("so")) {
            return true;
        };
    }
    return false;
}

bool QLibraryPrivate::isPlugin()
{
    errorString.clear();
    if (pluginState != MightBeAPlugin)
        return pluginState == IsAPlugin;

#ifndef QT_NO_PLUGIN_CHECK
    bool success = false;

    if (Q_UNLIKELY(fileName.endsWith(QLatin1String(".debug")))) {
        // refuse to load a file that ends in .debug
        // these are the debug symbols from the libraries
        // the problem is that they are valid shared library files
        // and dlopen is known to crash while opening them

        // pretend we didn't see the file
        errorString = QLibrary::tr("The shared library was not found.");
        pluginState = IsNotAPlugin;
        return false;
    }

    const QStatInfo statinfo(fileName);

#ifndef QT_NO_DATESTRING
    lastModified  = statinfo.lastModified().toString(Qt::ISODate);
#endif
    QString regkey = QString::fromLatin1("Katie Plugin Cache %1/%2")
                     .arg(QLatin1String(QT_VERSION_HEX_STR))
                     .arg(fileName);

#ifndef QT_NO_SETTINGS
    QSettings *settings = QCoreApplicationPrivate::staticConf();
    QStringList reg = settings->value(regkey).toStringList();
    if (reg.count() == 2 && lastModified == reg.at(1)) {
        qt_version = reg.at(0).toUInt();
        success = qt_version != 0;
    } else {
#endif
        // use unix shortcut to avoid loading the library
        success = qt_unix_query(fileName, &qt_version, this);

#ifndef QT_NO_SETTINGS
        QStringList queried;
        queried << QString::number(qt_version) << lastModified;
        settings->setValue(regkey, queried);
    }
#endif

    if (!success) {
        if (errorString.isEmpty()){
            if (fileName.isEmpty())
                errorString = QLibrary::tr("The shared library was not found.");
            else
                errorString = QLibrary::tr("The file '%1' is not a valid Katie plugin.").arg(fileName);
        }
        return false;
    }

    pluginState = IsNotAPlugin; // be pessimistic

    if (qt_version < QT_VERSION) {
        if (qt_debug_component()) {
            qWarning("Plugin uses incompatible Katie library: %s (%d, %d)\n",
                QFile::encodeName(fileName).data(), qt_version, QT_VERSION);
        }
        errorString = QLibrary::tr("The plugin uses incompatible Katie library: %1 (%2, %3)")
            .arg(fileName)
            .arg(qt_version)
            .arg(QT_VERSION);
    } else {
        pluginState = IsAPlugin;
    }

    return pluginState == IsAPlugin;
#else
    return pluginState == MightBeAPlugin;
#endif
}

/*!
    Loads the library and returns true if the library was loaded
    successfully; otherwise returns false. Since resolve() always
    calls this function before resolving any symbols it is not
    necessary to call it explicitly. In some situations you might want
    the library loaded in advance, in which case you would use this
    function.

    \sa unload()
*/
bool QLibrary::load()
{
    if (!d)
        return false;
    if (d->did_load)
        return d->pHnd;
    d->did_load = true;
    return d->load();
}

/*!
    Unloads the library and returns true if the library could be
    unloaded; otherwise returns false.

    This happens automatically on application termination, so you
    shouldn't normally need to call this function.

    If other instances of QLibrary are using the same library, the
    call will fail, and unloading will only happen when every instance
    has called unload().

    \sa resolve(), load()
*/
bool QLibrary::unload()
{
    if (d->did_load) {
        d->did_load = false;
        return d->unload();
    }
    return false;
}

/*!
    Returns true if the library is loaded; otherwise returns false.

    \sa load()
 */
bool QLibrary::isLoaded() const
{
    return d && d->pHnd;
}


/*!
    Constructs a library with the given \a parent.
 */
QLibrary::QLibrary(QObject *parent)
    :QObject(parent), d(0)
{
}


/*!
    Constructs a library object with the given \a parent that will
    load the library specified by \a fileName.

    We recommend omitting the file's suffix in \a fileName, since
    QLibrary will automatically look for the file with the appropriate
    suffix in accordance with the platform, e.g. ".so" on Unix,
    ".dylib" on Mac OS X, and ".dll" on Windows. (See \l{fileName}.)

 */
QLibrary::QLibrary(const QString& fileName, QObject *parent)
    :QObject(parent), d(0)
{
    setFileName(fileName);
}


/*!
    Constructs a library object with the given \a parent that will
    load the library specified by \a fileName and major version number \a verNum.
    Currently, the version number is ignored on Windows.

    We recommend omitting the file's suffix in \a fileName, since
    QLibrary will automatically look for the file with the appropriate
    suffix in accordance with the platform, e.g. ".so" on Unix,
    ".dylib" on Mac OS X, and ".dll" on Windows. (See \l{fileName}.)
*/
QLibrary::QLibrary(const QString& fileName, int verNum, QObject *parent)
    :QObject(parent), d(0)
{
    setFileNameAndVersion(fileName, verNum);
}

/*!
    Constructs a library object with the given \a parent that will
    load the library specified by \a fileName and full version number \a version.
    Currently, the version number is ignored on Windows and Symbian.

    We recommend omitting the file's suffix in \a fileName, since
    QLibrary will automatically look for the file with the appropriate
    suffix in accordance with the platform, e.g. ".so" on Unix,
    ".dylib" on Mac OS X, and ".dll" on Windows. (See \l{fileName}.)
 */
QLibrary::QLibrary(const QString& fileName, const QString &version, QObject *parent)
    :QObject(parent), d(0)
{
    setFileNameAndVersion(fileName, version);
}

/*!
    Destroys the QLibrary object.

    Unless unload() was called explicitly, the library stays in memory
    until the application terminates.

    \sa isLoaded(), unload()
*/
QLibrary::~QLibrary()
{
    if (d)
        d->release();
}


/*!
    \property QLibrary::fileName
    \brief the file name of the library

    We recommend omitting the file's suffix in the file name, since
    QLibrary will automatically look for the file with the appropriate
    suffix (see isLibrary()).

    When loading the library, QLibrary searches in all system-specific
    library locations (e.g. \c LD_LIBRARY_PATH on Unix), unless the
    file name has an absolute path. After loading the library
    successfully, fileName() returns the fully-qualified file name of
    the library, including the full path to the library if one was given
    in the constructor or passed to setFileName().

    For example, after successfully loading the "GL" library on Unix
    platforms, fileName() will return "libGL.so". If the file name was
    originally passed as "/usr/lib/libGL", fileName() will return
    "/usr/lib/libGL.so".
*/

void QLibrary::setFileName(const QString &fileName)
{
    QLibrary::LoadHints lh;
    if (d) {
        lh = d->loadHints;
        d->release();
        d = 0;
    }
    d = QLibraryPrivate::findOrCreate(fileName);
    d->loadHints = lh;
    d->did_load = false;
}

QString QLibrary::fileName() const
{
    if (d)
        return d->fileName;
    return QString();
}

/*!
    \fn void QLibrary::setFileNameAndVersion(const QString &fileName, int versionNumber)

    Sets the fileName property and major version number to \a fileName
    and \a versionNumber respectively.
    The \a versionNumber is ignored on Windows.

    \sa setFileName()
*/
void QLibrary::setFileNameAndVersion(const QString &fileName, int verNum)
{
    QLibrary::LoadHints lh;
    if (d) {
        lh = d->loadHints;
        d->release();
        d = 0;
    }
    d = QLibraryPrivate::findOrCreate(fileName, verNum >= 0 ? QString::number(verNum) : QString());
    d->loadHints = lh;
    d->did_load = false;
}

/*!
    \since 4.4

    Sets the fileName property and full version number to \a fileName
    and \a version respectively.
    The \a version parameter is ignored on Windows.

    \sa setFileName()
*/
void QLibrary::setFileNameAndVersion(const QString &fileName, const QString &version)
{
    QLibrary::LoadHints lh;
    if (d) {
        lh = d->loadHints;
        d->release();
        d = 0;
    }
    d = QLibraryPrivate::findOrCreate(fileName, version);
    d->loadHints = lh;
    d->did_load = false;
}

/*!
    Returns the address of the exported symbol \a symbol. The library is
    loaded if necessary. The function returns 0 if the symbol could
    not be resolved or if the library could not be loaded.

    Example:
    \snippet doc/src/snippets/code/src_corelib_plugin_qlibrary.cpp 2

    The symbol must be exported as a C function from the library. This
    means that the function must be wrapped in an \c{extern "C"} if
    the library is compiled with a C++ compiler. On Windows you must
    also explicitly export the function from the DLL using the
    \c{__declspec(dllexport)} compiler directive, for example:

    \snippet doc/src/snippets/code/src_corelib_plugin_qlibrary.cpp 3

    with \c MY_EXPORT defined as

    \snippet doc/src/snippets/code/src_corelib_plugin_qlibrary.cpp 4
*/
void *QLibrary::resolve(const char *symbol)
{
    if (!isLoaded() && !load())
        return 0;
    return d->resolve(symbol);
}

/*!
    \overload

    Loads the library \a fileName and returns the address of the
    exported symbol \a symbol. Note that \a fileName should not
    include the platform-specific file suffix; (see \l{fileName}). The
    library remains loaded until the application exits.

    The function returns 0 if the symbol could not be resolved or if
    the library could not be loaded.

    \sa resolve()
*/
void *QLibrary::resolve(const QString &fileName, const char *symbol)
{
    QLibrary library(fileName);
    return library.resolve(symbol);
}

/*!
    \overload

    Loads the library \a fileName with major version number \a verNum and
    returns the address of the exported symbol \a symbol.
    Note that \a fileName should not include the platform-specific file suffix;
    (see \l{fileName}). The library remains loaded until the application exits.
    \a verNum is ignored on Windows.

    The function returns 0 if the symbol could not be resolved or if
    the library could not be loaded.

    \sa resolve()
*/
void *QLibrary::resolve(const QString &fileName, int verNum, const char *symbol)
{
    QLibrary library(fileName, verNum);
    return library.resolve(symbol);
}

/*!
    \overload
    \since 4.4

    Loads the library \a fileName with full version number \a version and
    returns the address of the exported symbol \a symbol.
    Note that \a fileName should not include the platform-specific file suffix;
    (see \l{fileName}). The library remains loaded until the application exits.
    \a version is ignored on Windows.

    The function returns 0 if the symbol could not be resolved or if
    the library could not be loaded.

    \sa resolve()
*/
void *QLibrary::resolve(const QString &fileName, const QString &version, const char *symbol)
{
    QLibrary library(fileName, version);
    return library.resolve(symbol);
}

/*!
    \fn QString QLibrary::library() const

    Use fileName() instead.
*/

/*!
    \fn void QLibrary::setAutoUnload( bool b )

    Use load(), isLoaded(), and unload() as necessary instead.
*/

/*!
    \since 4.2

    Returns a text string with the description of the last error that occurred.
    Currently, errorString will only be set if load(), unload() or resolve() for some reason fails.
*/
QString QLibrary::errorString() const
{
    return (!d || d->errorString.isEmpty()) ? tr("Unknown error") : d->errorString;
}

/*!
    \property QLibrary::loadHints
    \brief Give the load() function some hints on how it should behave.

    You can give some hints on how the symbols are resolved. Usually,
    the symbols are not resolved at load time, but resolved lazily,
    (that is, when resolve() is called). If you set the loadHint to
    ResolveAllSymbolsHint, then all symbols will be resolved at load time
    if the platform supports it.

    Setting ExportExternalSymbolsHint will make the external symbols in the
    library available for resolution in subsequent loaded libraries.

    The interpretation of the load hints is platform dependent, and if
    you use it you are probably making some assumptions on which platform
    you are compiling for, so use them only if you understand the consequences
    of them.

    By default, none of these flags are set, so libraries will be loaded with
    lazy symbol resolution, and will not export external symbols for resolution
    in other dynamically-loaded libraries.
*/
void QLibrary::setLoadHints(LoadHints hints)
{
    if (!d) {
        d = QLibraryPrivate::findOrCreate(QString());   // ugly, but we need a d-ptr
        d->errorString.clear();
    }
    d->loadHints = hints;
}

QLibrary::LoadHints QLibrary::loadHints() const
{
    return d ? d->loadHints : (QLibrary::LoadHints)0;
}

/* Internal, for debugging */
bool qt_debug_component()
{
    static int debug_env = qgetenv("QT_DEBUG_PLUGINS").toInt();
    return debug_env != 0;
}

#include "moc_qlibrary.h"

QT_END_NAMESPACE

#endif // QT_NO_LIBRARY
