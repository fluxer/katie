# https://docs.fedoraproject.org/en-US/packaging-guidelines/

Name: katie
Version: 4.9.0
Release: 1%{?dist}
Summary: C++ toolkit derived from the Qt 4.8 framework
License: BSD and LGPLv2+
URL: https://github.com/fluxer/katie

BuildRequires: gcc-c++ cmake libicu-devel libzstd-devel zlib-devel libsq3-devel libpng-devel freetype-devel pcre-devel openssl-devel libX11-devel libXinerama-devel libXrandr-devel libXrender-devel libXfixes-devel libXcursor-devel libSM-devel libICE-devel dbus-devel libtiff-devel libjpeg-turbo-devel fontconfig-devel cups-devel libiodbc-devel libpq-devel mariadb-embedded-devel unifdef

%description
This package is pre-release version of the Katie toolkit

%build
%cmake -DKATIE_TOOLS_SUFFIX="-katie"
%cmake_build

%install
%cmake_install --strip
rm -v %{buildroot}/%{_sysconfdir}/profile.d/katie-*.sh

%files
%doc README
%license LGPL_EXCEPTION.txt
%{_bindir}/*
%{_includedir}/katie/*
%{_libdir}/katie/*
%{_libdir}/*.so
%{_libdir}/*.so.*
%{_libdir}/pkgconfig/*
%{_libdir}/cmake/Katie/*
%{_datadir}/applications/*
%{_datadir}/pixmaps/*
%{_mandir}/man1/*
%{_sysconfdir}/ld.so.conf.d/katie-*.conf

%changelog
* Wed Dec 16 2020 Ivailo Monev <xakepa10@gmail.com> - 4.9.0-1
- initial package
