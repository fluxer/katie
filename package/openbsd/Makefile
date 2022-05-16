# $OpenBSD$

# https://www.openbsd.org/faq/ports/

DISTNAME = katie
PKGNAME = katie-4.12.1.d20201231
CATEGORIES = devel x11 net
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

COMPILER = base-gcc ports-gcc
MODULES = devel/cmake
BUILD_DEPENDS = devel/gettext,-tools
RUN_DEPENDS = devel/desktop-file-utils devel/xdg-utils fonts/freefont-ttf
LIB_DEPENDS = textproc/icu4c devel/jansson graphics/png x11/dbus \
	print/cups,-libs archivers/libdeflate devel/gettext,-runtime
WANTLIB = ${COMPILER_LIBCXX} ICE SM X11 Xcursor Xext Xfixes Xinerama Xrandr \
	Xrender fontconfig freetype icui18n icuuc png dbus-1 cups deflate \
	intl c m
SEPARATE_BUILD = Yes
CONFIGURE_ARGS = -DKATIE_TOOLS_SUFFIX="-katie" -Wno-dev

SHARED_LIBS = \
	KtCore                4.12 \
	KtDBus                4.12 \
	KtDeclarative         4.12 \
	KtDesigner            4.12 \
	KtDesignerComponents  4.12 \
	KtGui                 4.12 \
	KtNetwork             4.12 \
	KtScript              4.12 \
	KtScriptTools         4.12 \
	KtSvg                 4.12 \
	KtTest                4.12 \
	KtUiTools             4.12 \
	KtXml                 4.12

.include <bsd.port.mk>
