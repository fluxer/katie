# $OpenBSD$

# https://www.openbsd.org/faq/ports/

COMMENT = C++ toolkit derived from the Qt 4.8 framework
DISTNAME = katie
PKGNAME = katie-4.9.0.20200114
CATEGORIES = devel x11 net www
HOMEPAGE = https://github.com/fluxer/katie
MAINTAINER = Ivailo Monev <xakepa10@gmail.com>
# BSD LGPLv2+ custom
PERMIT_PACKAGE = Yes
PERMIT_DISTFILES = Yes

MASTER_SITES = https://github.com/fluxer/katie/archive/
DISTFILES = master.tar.gz
DIST_SUBDIR = ${DISTNAME}
WRKSRC = ${WRKDIR}/katie-master

COMPILER = base-clang ports-gcc
MODULES += devel/cmake
BUILD_DEPENDS = devel/gettext,-tools
RUN_DEPENDS += devel/desktop-file-utils
LIB_DEPENDS += print/cups,-libs x11/dbus textproc/icu4c graphics/jpeg devel/pcre
LIB_DEPENDS += graphics/png databases/sqlite3 graphics/tiff archivers/zstd
LIB_DEPENDS += devel/gettext,-runtime
WANTLIB += ${COMPILER_LIBCXX} ICE SM X11 Xcursor Xext Xfixes Xinerama
WANTLIB += Xrandr Xrender c crypto fontconfig freetype m ssl z
WANTLIB += cups dbus-1 icui18n icuuc intl jpeg pcre png sqlite3
WANTLIB += tiff zstd
SEPARATE_BUILD = Yes
CONFIGURE_ARGS = -DKATIE_TOOLS_SUFFIX="-katie"

SHARED_LIBS +=  KtCore                4.9
SHARED_LIBS +=  KtDBus                4.9
SHARED_LIBS +=  KtDeclarative         4.9
SHARED_LIBS +=  KtDesigner            4.9
SHARED_LIBS +=  KtDesignerComponents  4.9
SHARED_LIBS +=  KtGui                 4.9
SHARED_LIBS +=  KtNetwork             4.9
SHARED_LIBS +=  KtScript              4.9
SHARED_LIBS +=  KtScriptTools         4.9
SHARED_LIBS +=  KtSql                 4.9
SHARED_LIBS +=  KtSvg                 4.9
SHARED_LIBS +=  KtTest                4.9
SHARED_LIBS +=  KtUiTools             4.9
SHARED_LIBS +=  KtXml                 4.9

.include <bsd.port.mk>
