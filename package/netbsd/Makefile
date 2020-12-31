# $NetBSD$

# https://wiki.netbsd.org/pkgsrc/intro_to_packaging/
# https://www.netbsd.org/docs/pkgsrc/

DISTNAME = master
PKGNAME = katie-4.9.2.d20201231
CATEGORIES = devel x11 net www
MAINTAINER = xakepa10@gmail.com
COMMENT = C++ toolkit deived from the Qt 4.8 framework
LICENSE = modified-bsd AND gnu-lgpl-v2.1
HOMEPAGE = https://github.com/fluxer/katie

MASTER_SITES = https://github.com/fluxer/katie/archive/
DIST_SUBDIR = katie
WRKSRC = ${WRKDIR}/katie-master

USE_CMAKE = yes
CMAKE_ARGS = -DKATIE_TOOLS_SUFFIX="-katie"
USE_LANGUAGES = c c++11
USE_TOOLS = pkg-config msgfmt

.include "../../sysutils/desktop-file-utils/desktopdb.mk"
.include "../../archivers/zstd/buildlink3.mk"
.include "../../textproc/icu/buildlink3.mk"
.include "../../devel/pcre/buildlink3.mk"
.include "../../devel/zlib/buildlink3.mk"
.include "../../security/openssl/buildlink3.mk"
.include "../../x11/libXinerama/buildlink3.mk"
.include "../../x11/libXcursor/buildlink3.mk"
.include "../../x11/libXext/buildlink3.mk"
.include "../../x11/libXfixes/buildlink3.mk"
.include "../../x11/libXrandr/buildlink3.mk"
.include "../../x11/libXrender/buildlink3.mk"
.include "../../x11/libX11/buildlink3.mk"
.include "../../x11/libICE/buildlink3.mk"
.include "../../x11/libSM/buildlink3.mk"
.include "../../fonts/fontconfig/buildlink3.mk"
.include "../../graphics/freetype2/buildlink3.mk"
.include "../../graphics/png/buildlink3.mk"
.include "../../graphics/tiff/buildlink3.mk"
.include "../../print/cups-base/buildlink3.mk"
.include "../../sysutils/dbus/buildlink3.mk"
.include "../../databases/sqlite3/buildlink3.mk"
.include "../../databases/iodbc/buildlink3.mk"
.include "../../mk/pthread.buildlink3.mk"
.include "../../mk/jpeg.buildlink3.mk"
.include "../../mk/bsd.pkg.mk"
