# https://www.freebsd.org/doc/en_US.ISO8859-1/books/porters-handbook/

PORTNAME = katie
DISTVERSION = 4.11.1.d20201231
CATEGORIES = devel x11 ftp net www
MAINTAINER = xakepa10@gmail.com
COMMENT = C++ toolkit derived from the Qt 4.8 framework
LICENSE = BSD3CLAUSE LGPL21+
LICENSE_COMB = multi

MASTER_SITES = https://github.com/fluxer/katie/archive/
DISTFILES = master.tar.gz
DIST_SUBDIR = ${PORTNAME}
WRKSRC = ${WRKDIR}/katie-master

USES = compiler:c++11-lang pkgconfig cmake ssl xorg jpeg desktop-file-utils
USE_XORG = x11 xinerama xrandr xrender xfixes xcursor xext sm ice
USE_LDCONFIG = yes
RUN_DEPENDS = xdg-open:devel/xdg-utils
LIB_DEPENDS = libzstd.so:archivers/zstd libicuuc.so:devel/icu \
	libicui18n.so:devel/icu libjansson.so:devel/jansson \
	libpcre.so:devel/pcre libpng.so:graphics/png \
	libtiff.so:graphics/tiff libfreetype.so:print/freetype2 \
	libfontconfig.so:x11-fonts/fontconfig libdbus-1.so:devel/dbus \
	libcups.so:print/cups
CMAKE_ARGS = -DKATIE_TOOLS_SUFFIX="-katie" -Wno-dev

OPTIONS_DEFINE = NLS
OPTIONS_SUB = yes
NLS_DESC = Build translator and translations
NLS_USES = gettext
NLS_CMAKE_ON = -DWITH_INTL=TRUE
NLS_CMAKE_OFF = -DWITH_INTL=FALSE

.include <bsd.port.mk>
