# $FreeBSD$

# https://www.freebsd.org/doc/en_US.ISO8859-1/books/porters-handbook/

PORTNAME = katie
DISTVERSION = 4.9.0.20200114
CATEGORIES = devel x11 ftp net www
MAINTAINER = xakepa10@gmail.com
COMMENT = C++ toolkit derived from the Qt 4.8 framework
LICENSE = KATIE BSD3CLAUSE LGPL21+
LICENSE_COMB = multi
LICENSE_NAME_KATIE = The Qt Company Qt LGPL Exception version 1.1
LICENSE_FILE_KATIE = ${WRKDIR}/katie-master/LGPL_EXCEPTION.txt
LICENSE_PERMS_KATIE = dist-mirror pkg-mirror auto-accept

MASTER_SITES = https://github.com/fluxer/katie/archive/
DISTFILES = master.tar.gz
DIST_SUBDIR = ${PORTNAME}
WRKSRC = ${WRKDIR}/katie-master

USES = compiler:c++11-lang pkgconfig cmake gettext ssl xorg sqlite:3 desktop-file-utils
USE_XORG = xinerama xrandr xrender xfixes xcursor xext sm ice
USE_LDCONFIG = yes
LIB_DEPENDS = libzstd.so:archivers/zstd libicuuc.so:devel/icu libicui18n.so:devel/icu \
	libpcre.so:devel/pcre libpng.so:graphics/png libjpeg.so:graphics/jpeg-turbo \
	libtiff.so:graphics/tiff libfreetype.so:print/freetype2 \
	libfontconfig.so:x11-fonts/fontconfig libdbus-1.so:devel/dbus libcups.so:print/cups \
	libiodbc.so:databases/libiodbc
CMAKE_ARGS = -DKATIE_TOOLS_SUFFIX="-katie"

OPTIONS_DEFINE = MYSQL PGSQL
OPTIONS_SUB = yes
MYSQL_DESC = Build MySQL database plugin
MYSQL_USES = mysql
MYSQL_CMAKE_ON = -DWITH_MYSQL=TRUE
MYSQL_CMAKE_OFF = -DWITH_MYSQL=FALSE
PGSQL_DESC = Build PostgreSQL database plugin
PGSQL_USES = pgsql
WANT_PGSQL = client
PGSQL_CMAKE_ON = -DWITH_POSTGRESQL=TRUE
PGSQL_CMAKE_OFF = -DWITH_POSTGRESQL=FALSE

.include <bsd.port.mk>
