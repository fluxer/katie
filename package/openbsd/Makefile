# $OpenBSD$

# https://www.openbsd.org/faq/ports/

DISTNAME = katie
PKGNAME = katie-4.10.0.d20201231
CATEGORIES = devel x11 net www
MAINTAINER = Ivailo Monev <xakepa10@gmail.com>
COMMENT = C++ toolkit derived from the Qt 4.8 framework
# BSD LGPLv2+ custom
PERMIT_PACKAGE = Yes
PERMIT_DISTFILES = Yes
HOMEPAGE = https://github.com/fluxer/katie

MASTER_SITES = https://github.com/fluxer/katie/archive/
DISTFILES = master.tar.gz
DIST_SUBDIR = ${DISTNAME}
WRKSRC = ${WRKDIR}/katie-master

COMPILER = base-clang ports-gcc
MODULES = devel/cmake
BUILD_DEPENDS = devel/gettext,-tools
RUN_DEPENDS = devel/desktop-file-utils devel/xdg-utils
LIB_DEPENDS = archivers/zstd textproc/icu4c devel/pcre graphics/png \
	graphics/jpeg graphics/tiff x11/dbus print/cups,-libs databases/iodbc \
	databases/sqlite3 devel/gettext,-runtime
WANTLIB = ${COMPILER_LIBCXX} ICE SM X11 Xcursor Xext Xfixes Xinerama Xrandr \
	Xrender fontconfig freetype zstd icui18n icuuc pcre png jpeg tiff \
	dbus-1 cups iodbc sqlite3 intl ssl z c crypto m
SEPARATE_BUILD = Yes
CONFIGURE_ARGS = -DKATIE_TOOLS_SUFFIX="-katie"

SHARED_LIBS = \
	KtCore                4.10 \
	KtDBus                4.10 \
	KtDeclarative         4.10 \
	KtDesigner            4.10 \
	KtDesignerComponents  4.10 \
	KtGui                 4.10 \
	KtNetwork             4.10 \
	KtScript              4.10 \
	KtScriptTools         4.10 \
	KtSql                 4.10 \
	KtSvg                 4.10 \
	KtTest                4.10 \
	KtUiTools             4.10 \
	KtXml                 4.10

.include <bsd.port.mk>
