# https://docs.freebsd.org/en/books/porters-handbook/

PORTNAME = katie
DISTVERSION = 4.14.0.d20230809
CATEGORIES = devel x11 net
MAINTAINER = xakepa10@gmail.com
COMMENT = C++ toolkit derived from the Qt 4.8 framework
LICENSE = BSD3CLAUSE LGPL21+
LICENSE_COMB = multi

MASTER_SITES = https://google.com/
DISTFILES = index.html
DIST_SUBDIR = ${PORTNAME}
WRKSRC = ${WRKDIR}/katie.git

USES = compiler:c++11-lang pkgconfig cmake xorg desktop-file-utils
USE_XORG = x11 xinerama xrandr xrender xfixes xcursor xext sm ice
USE_LDCONFIG = yes
USE_GETTEXT_TOOLS = build run
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
NLS_USES = gettext-tools
NLS_CMAKE_ON = -DWITH_GETTEXTPO=TRUE
NLS_CMAKE_OFF = -DWITH_GETTEXTPO=FALSE

do-extract:
	@rm -vrf ${WRKSRC}
	# @git clone --depth=1 https://scm.osdn.net/gitroot/kde/Katie.git ${WRKSRC}
	@git clone --depth=1 https://bitbucket.org/smil3y/Katie.git ${WRKSRC}
	# @git clone --depth=1 https://github.com/fluxer/katie.git ${WRKSRC}

.include <bsd.port.mk>
