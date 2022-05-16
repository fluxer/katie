# https://docs.freebsd.org/en/books/porters-handbook/

PORTNAME = katie
DISTVERSION = 4.12.0.d20201231
CATEGORIES = devel x11 net
MAINTAINER = xakepa10@gmail.com
COMMENT = C++ toolkit derived from the Qt 4.8 framework
LICENSE = BSD3CLAUSE LGPL21+
LICENSE_COMB = multi

MASTER_SITES = https://github.com/fluxer/katie/archive/
DISTFILES = master.tar.gz
DIST_SUBDIR = ${PORTNAME}
WRKSRC = ${WRKDIR}/katie-master

USES = compiler:c++11-lang pkgconfig cmake xorg desktop-file-utils
USE_XORG = x11 xinerama xrandr xrender xfixes xcursor xext sm ice
USE_LDCONFIG = yes
RUN_DEPENDS = xdg-open:devel/xdg-utils \
	${LOCALBASE}/share/fonts/freefont-ttf/FreeSans.ttf:x11-fonts/freefont-ttf
LIB_DEPENDS = libdeflate.so:archivers/libdeflate libicuuc.so:devel/icu \
	libicui18n.so:devel/icu libjansson.so:devel/jansson \
	libpng.so:graphics/png libfreetype.so:print/freetype2 \
	libfontconfig.so:x11-fonts/fontconfig libdbus-1.so:devel/dbus \
	libcups.so:print/cups
CMAKE_ARGS = -DKATIE_TOOLS_SUFFIX="-katie" -Wno-dev

OPTIONS_DEFINE = NLS
OPTIONS_SUB = yes
NLS_DESC = Build translator and translations
NLS_USES = gettext
NLS_CMAKE_ON = -DWITH_INTL=TRUE
NLS_CMAKE_OFF = -DWITH_INTL=FALSE

# workaround for Plasma desktop crashes, possible miss-optimization since it
# does not crash when build for debugging
.include <bsd.port.options.mk>
.if ${OPSYS} == FreeBSD && ${OSVERSION} > 1300000
USE_GCC = yes
.endif

.include <bsd.port.mk>
