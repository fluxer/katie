# $FreeBSD$

PORTNAME = katie
DISTVERSION = 4.9.0.20200114
CATEGORIES = devel x11
MAINTAINER = xakepa10@gmail.com
COMMENT = C++ toolkit derived from the Qt 4.8 framework
LICENSE = BSD3CLAUSE LGPL21+ GFDL
LICENSE_COMB = multi
LICENSE_FILE = ${WRKSRC}/LGPL_EXCEPTION.txt

MASTER_SITES = https://github.com/fluxer/katie/archive/
DISTFILES = master.tar.gz
DIST_SUBDIR = ${PORTNAME}
WRKSRC = ${WRKDIR}/katie-master

USES = cmake xorg sqlite:3
USE_XORG = xi xt xinerama xrandr xrender xcursor
# BUILD_DEPENDS = pkgconf zstd icu pcre png jpeg-turbo tiff libmng freetype2 fontconfig dbus cups mysql pgsql libiodbc
CMAKE_ARGS = -DKATIE_TOOLS_SUFFIX="-katie"

.include <bsd.port.mk> 
