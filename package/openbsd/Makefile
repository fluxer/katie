# $OpenBSD$

# https://www.openbsd.org/faq/ports/

DISTNAME = katie
PKGNAME = katie-4.11.1.d20201231
CATEGORIES = devel x11 net www
MAINTAINER = Ivailo Monev <xakepa10@gmail.com>
COMMENT = C++ toolkit derived from the Qt 4.8 framework
# BSD LGPLv2+
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
LIB_DEPENDS = archivers/zstd textproc/icu4c devel/jansson devel/pcre \
	graphics/png graphics/jpeg graphics/tiff x11/dbus print/cups,-libs \
	devel/gettext,-runtime
WANTLIB = ${COMPILER_LIBCXX} ICE SM X11 Xcursor Xext Xfixes Xinerama Xrandr \
	Xrender fontconfig freetype zstd icui18n icuuc pcre png jpeg tiff \
	dbus-1 cups intl ssl z c crypto m
SEPARATE_BUILD = Yes
CONFIGURE_ARGS = -DKATIE_TOOLS_SUFFIX="-katie" -Wno-dev

SHARED_LIBS = \
	KtCore                4.11 \
	KtDBus                4.11 \
	KtDeclarative         4.11 \
	KtDesigner            4.11 \
	KtDesignerComponents  4.11 \
	KtGui                 4.11 \
	KtNetwork             4.11 \
	KtScript              4.11 \
	KtScriptTools         4.11 \
	KtSvg                 4.11 \
	KtTest                4.11 \
	KtUiTools             4.11 \
	KtXml                 4.11

.include <bsd.port.mk>
