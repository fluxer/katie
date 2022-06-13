
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

#include "qplatformdefs.h"
#include "qlibrary.h"

#ifndef QT_NO_LIBRARY

#include "qlibrary_p.h"
#include "qstringlist.h"
#include "qfile.h"
#include "qfileinfo.h"
#include "qmutex.h"
#include "qmap.h"
#include "qcoreapplication_p.h"
#include "qdebug.h"
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
    compiler. For convenience, there is a static resolve() function
    which you can use if you just want to call a function in a library
    without explicitly loading the library first:

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
  for the plugin section. The advantage of this approach is that
  we can get the verification data without have to actually load the library.
  This lets us detect mismatches more safely.

  Returns false if the information could not be read.
  Returns true if successfully read.
*/
static bool qt_unix_query(const QString &library, QLibraryPrivate *lib)
{
    QFile file(library);
    if (!file.open(QIODevice::ReadOnly)) {
        lib->errorString = file.errorString();
        if (qt_debug_component()) {
            qWarning("%s: %s", QFile::encodeName(library).data(),
                qPrintable(lib->errorString));
        }
        return false;
    }

    QByteArray data;
    qint64 datalen = file.size();
    const char *filedata = reinterpret_cast<char*>(file.map(0, file.size()));
    if (filedata == 0) {
        // try reading the data into memory instead
        data = file.readAll();
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
    const char *const sh_strtab_p = filedata + shdr[ehdr->e_shstrndx].sh_offset;
    for (int i = 0; i < ehdr->e_shnum; ++i) {
        const char* sectioname = sh_strtab_p + shdr[i].sh_name;
        if (qstrcmp(sectioname, ".ktplugin") == 0) {
            ret = true;
            // compatiblity between releases is not guaranteed thus no version matching is done
            break;
        }
    }

    if (!ret) {
        lib->errorString = QLibrary::tr("Plugin verification data mismatch in '%1'").arg(library);
    }
    file.close();

    return ret;
}
#endif // QT_NO_PLUGIN_CHECK


static QString qt_find_library(const QString &fileName)
{
    if (qt_debug_component()) {
        qWarning() << "QLibrary: looking for" << fileName;
    }

    if (Q_UNLIKELY(fileName.isEmpty())) {
        return fileName;
    }

    // absolute path
    if (fileName.startsWith(QLatin1Char('/'))) {
        return fileName;
    }

    const QStringList attempts = QStringList()
        << QString::fromLatin1("/%1").arg(fileName)
        << QString::fromLatin1("/%1.so").arg(fileName)
        << QString::fromLatin1("/lib%1").arg(fileName)
        << QString::fromLatin1("/lib%1.so").arg(fileName);
    foreach(const QString &libpath, QCoreApplication::libraryPaths()) {
        foreach(const QString &attempt, attempts) {
            const QString fullpath = libpath + attempt;
            if (qt_debug_component()) {
                qDebug() << "QLibrary: checking if library exists" << fullpath;
            }
            const QStatInfo statinfo(fullpath);
            if (statinfo.isFile()) {
                if (qt_debug_component()) {
                    qDebug() << "QLibrary: found library" << fullpath;
                }
                return fullpath;
            }
        }
    }

    return fileName;
}


class QLibraryCleanup : public QList<QLibraryPrivate*>
{
public:
    ~QLibraryCleanup();
};

QLibraryCleanup::~QLibraryCleanup()
{
    while (!isEmpty()) {
        QLibraryPrivate* library = takeLast();
        library->unload();
        delete library;
    }
}

Q_GLOBAL_STATIC(QMutex, qGlobalLibraryMutex);
Q_GLOBAL_STATIC(QLibraryCleanup, qGlobalLibraryList);

QLibraryPrivate::QLibraryPrivate()
    : pHnd(nullptr), fileName(),
     instance(nullptr), pluginState(QLibraryPrivate::MightBeAPlugin)
{
}

QLibraryPrivate::~QLibraryPrivate()
{
}

bool QLibraryPrivate::load()
{
    if (pHnd) {
        return true;
    }
    if (fileName.isEmpty()) {
        return false;
    }

    return load_sys();
}

bool QLibraryPrivate::unload()
{
    if (!pHnd) {
        return false;
    }
    delete inst.data();
    if (unload_sys()) {
        if (qt_debug_component()) {
            qWarning() << "QLibraryPrivate::unload succeeded on" << fileName;
        }
        pHnd = 0;
    }
    return (pHnd == 0);
}

void *QLibraryPrivate::resolve(const char *symbol)
{
    if (!pHnd) {
        return nullptr;
    }
    return resolve_sys(symbol);
}


bool QLibraryPrivate::loadPlugin()
{
    if (instance) {
        return true;
    }
    if (pluginState == QLibraryPrivate::IsNotAPlugin) {
        return false;
    }
    if (load()) {
        instance = (QtPluginInstanceFunction)resolve("kt_plugin_instance");
        return instance;
    }
    if (qt_debug_component()) {
        qWarning() << "QLibraryPrivate::loadPlugin failed on" << fileName << ":" << errorString;
    }
    pluginState = QLibraryPrivate::IsNotAPlugin;
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
    if (pluginState != QLibraryPrivate::MightBeAPlugin) {
        return (pluginState == QLibraryPrivate::IsAPlugin);
    }

#ifndef QT_NO_PLUGIN_CHECK
    if (Q_UNLIKELY(fileName.endsWith(QLatin1String(".debug")))) {
        // refuse to load a file that ends in .debug
        // these are the debug symbols from the libraries
        // the problem is that they are valid shared library files
        // and dlopen is known to crash while opening them

        // pretend we didn't see the file
        errorString = QLibrary::tr("The shared library was not found.");
        pluginState = QLibraryPrivate::IsNotAPlugin;
        return false;
    }

    // use unix shortcut to avoid loading the library
    const bool success = qt_unix_query(fileName, this);
    if (!success) {
        if (errorString.isEmpty()){
            if (fileName.isEmpty()) {
                errorString = QLibrary::tr("The shared library was not found.");
            } else {
                errorString = QLibrary::tr("The file '%1' is not a valid Katie plugin.").arg(fileName);
            }
        }
        return false;
    }

    pluginState = QLibraryPrivate::IsAPlugin;

    return (pluginState == QLibraryPrivate::IsAPlugin);
#else
    return (pluginState == QLibraryPrivate::MightBeAPlugin);
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
    if (!d_ptr->pHnd) {
        return d_ptr->load();
    }
    return d_ptr->pHnd;
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
    if (!d_ptr->pHnd) {
        return false;
    }
    return d_ptr->unload();
}

/*!
    Returns true if the library is loaded; otherwise returns false.

    \sa load()
 */
bool QLibrary::isLoaded() const
{
    return d_ptr->pHnd;
}

/*!
    Constructs a library with the given \a parent.
 */
QLibrary::QLibrary(QObject *parent)
    : QObject(parent),
    d_ptr(new QLibraryPrivate())
{
}

/*!
    Constructs a library object with the given \a parent that will
    load the library specified by \a fileName.

    We recommend omitting the file's suffix in \a fileName, since
    QLibrary will automatically look for the file with the appropriate
    suffix in accordance with the platform, e.g. ".so" on Unix.

    \sa fileName()
 */
QLibrary::QLibrary(const QString &fileName, QObject *parent)
    : QObject(parent),
    d_ptr(new QLibraryPrivate())
{
    setFileName(fileName);
}

/*!
    Destroys the QLibrary object.

    Unless unload() was called explicitly, the library stays in memory
    until the application terminates.

    \sa isLoaded(), unload()
*/
QLibrary::~QLibrary()
{
    if (!d_ptr->pHnd) {
        delete d_ptr;
    } else {
        QMutexLocker locker(qGlobalLibraryMutex());
        qGlobalLibraryList()->append(d_ptr);
    }
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
    // if the library path is not the same it may very well be library with different symbols
    const QString librarymatch = qt_find_library(fileName);
    QMutexLocker locker(qGlobalLibraryMutex());
    QLibraryCleanup* loadedlibraries = qGlobalLibraryList();
    for (int i = 0; i < loadedlibraries->size(); i++) {
        QLibraryPrivate* loadedlibrary = loadedlibraries->at(i);
        if (loadedlibrary->fileName == librarymatch) {
            if (qt_debug_component()) {
                qDebug() << "QLibrary: reusing library" << librarymatch;
            }
            if (!d_ptr->pHnd) {
                delete d_ptr;
                d_ptr = loadedlibrary;
                loadedlibraries->removeAt(i);
            } else {
                QLibraryPrivate* x_ptr = d_ptr;
                d_ptr = loadedlibrary;
                loadedlibraries->removeAt(i);
                loadedlibraries->append(x_ptr);
            }
            return;
        }
    }
    d_ptr->fileName = librarymatch;
}

QString QLibrary::fileName() const
{
    return d_ptr->fileName;
}

/*!
    Returns the address of the exported symbol \a symbol. The library is
    loaded if necessary. The function returns 0 if the symbol could
    not be resolved or if the library could not be loaded.

    Example:
    \snippet doc/src/snippets/code/src_corelib_plugin_qlibrary.cpp 2

    The symbol must be exported as a C function from the library. This
    means that the function must be wrapped in an \c{extern "C"} if
    the library is compiled with a C++ compiler.
*/
void *QLibrary::resolve(const char *symbol)
{
    if (!isLoaded() && !load()) {
        return nullptr;
    }
    return d_ptr->resolve(symbol);
}

/*!
    \since 4.2

    Returns a text string with the description of the last error that occurred.
    Currently, errorString will only be set if load(), unload() or resolve() for some reason fails.
*/
QString QLibrary::errorString() const
{
    return (d_ptr->errorString.isEmpty() ? QLibrary::tr("Unknown error") : d_ptr->errorString);
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
    d_ptr->loadHints = hints;
}

QLibrary::LoadHints QLibrary::loadHints() const
{
    return d_ptr->loadHints;
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
