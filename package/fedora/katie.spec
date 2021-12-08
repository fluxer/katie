# https://docs.fedoraproject.org/en-US/packaging-guidelines/

Name: katie
Version: 4.11.1
Release: 1%{?dist}
Summary: C++ toolkit derived from the Qt 4.8 framework
License: BSD and LGPLv2+
URL: https://github.com/fluxer/katie

BuildRequires: gcc-c++ cmake libicu-devel libzstd-devel jansson-devel zlib-devel libpng-devel freetype-devel pcre-devel openssl-devel libX11-devel libXinerama-devel libXrandr-devel libXrender-devel libXfixes-devel libXcursor-devel libSM-devel libICE-devel dbus-devel libtiff-devel libjpeg-turbo-devel fontconfig-devel cups-devel unifdef
Requires: xdg-utils
Requires(post): /sbin/ldconfig
Requires(postun): /sbin/ldconfig
Requires(post): desktop-file-utils
Requires(postun): desktop-file-utils

%description
Katie is continuation of the Qt4 C++ toolkit with the goal to keep it alive,
clean it up, fix some bugs and backport some features from Qt5. It is based
on the latest Git revision of Qt v4.8.

This package includes libraries, tools and their documentation.

%build
%cmake -DKATIE_TOOLS_SUFFIX="-katie"
%cmake_build

%install
%cmake_install --strip
rm -v %{buildroot}/%{_sysconfdir}/profile.d/katie-*.sh

%files
%doc README
%{_bindir}/*
%{_libdir}/katie/*
%{_libdir}/*.so.*
%{_datadir}/applications/*
%{_datadir}/pixmaps/*
%{_mandir}/man1/*
%{_sysconfdir}/ld.so.conf.d/katie-*.conf

%post
/sbin/ldconfig
/usr/bin/update-desktop-database &> /dev/null || :
%postun
/sbin/ldconfig
/usr/bin/update-desktop-database &> /dev/null || :

%package devel
Summary: C++ toolkit derived from the Qt 4.8 framework (development package)

Requires: katie >= %{version}

%description devel
Katie is continuation of the Qt4 C++ toolkit with the goal to keep it alive,
clean it up, fix some bugs and backport some features from Qt5. It is based
on the latest Git revision of Qt v4.8.

This package includes headers, pkg-config and CMake files.

%files devel
%doc README
%{_includedir}/katie/*
%{_libdir}/*.so
%{_libdir}/pkgconfig/*
%{_libdir}/cmake/Katie/*

%changelog
* Tue Nov 23 2021 Ivailo Monev <xakepa10@gmail.com> - 4.11.1-1
- package update

* Fri Aug 13 2021 Ivailo Monev <xakepa10@gmail.com> - 4.11.0-1
- package update

* Mon Feb 8 2020 Ivailo Monev <xakepa10@gmail.com> - 4.10.0-1
- package update

* Mon Dec 21 2020 Ivailo Monev <xakepa10@gmail.com> - 4.9.2-1
- package update

* Wed Dec 17 2020 Ivailo Monev <xakepa10@gmail.com> - 4.9.1-1
- package update

* Wed Dec 16 2020 Ivailo Monev <xakepa10@gmail.com> - 4.9.0-1
- initial package
