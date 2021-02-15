# $NetBSD$

PKG_OPTIONS_VAR = PKG_OPTIONS.katie
PKG_SUPPORTED_OPTIONS = mysql pgsql
PKG_SUGGESTED_OPTIONS = 

.include "../../mk/bsd.options.mk"

PLIST_VARS += mysql pgsql

.if !empty(PKG_OPTIONS:Mmysql)
CMAKE_ARGS += -DWITH_MYSQL=TRUE
PLIST.mysql = yes
.  include "../../mk/mysql.buildlink3.mk"
.else
CMAKE_ARGS += -DWITH_MYSQL=FALSE
.endif

.if !empty(PKG_OPTIONS:Mpgsql)
CMAKE_ARGS += -DWITH_POSTGRESQL=TRUE
PLIST.pgsql = yes
.  include "../../mk/pgsql.buildlink3.mk"
.else
CMAKE_ARGS += -DWITH_POSTGRESQL=FALSE
.endif
