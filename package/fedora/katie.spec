# https://docs.fedoraproject.org/en-US/packaging-guidelines/

Name: katie
Version: 20201216git
Release: 0.1%{?dist}
Summary: C++ toolkit derived from the Qt 4.8 framework
License: BSD LGPL
URL: https://github.com/fluxer/katie

BuildRequires: gcc-c++ cmake libicu-devel libzstd-devel zlib-devel libsq3-devel libpng-devel freetype-devel pcre-devel openssl-devel libX11-devel libXinerama-devel libXrandr-devel libXrender-devel libXfixes-devel libXcursor-devel libSM-devel libICE-devel dbus-devel libtiff-devel libjpeg-turbo-devel fontconfig-devel cups-devel libiodbc-devel libpq-devel mariadb-devel unifdef

%description
This package is pre-release version of the Katie toolkit

%license
The Qt Company Qt LGPL Exception version 1.1

As an additional permission to the GNU Lesser General Public License version
2.1, the object code form of a "work that uses the Library" may incorporate
material from a header file that is part of the Library.  You may distribute
such object code under terms of your choice, provided that:
    (i)   the header files of the Library have not been modified; and
    (ii)  the incorporated material is limited to numerical parameters, data
          structure layouts, accessors, macros, inline functions and
          templates; and
    (iii) you comply with the terms of Section 6 of the GNU Lesser General
          Public License version 2.1.

Moreover, you may apply this exception to a modified version of the Library,
provided that such modification does not involve copying material from the
Library into the modified Library's header files unless such material is
limited to (i) numerical parameters; (ii) data structure layouts;
(iii) accessors; and (iv) small macros, templates and inline functions of
five lines or less in length.

Furthermore, you are not required to apply this additional permission to a
modified version of the Library.

%build
%cmake -DKATIE_TOOLS_SUFFIX="-katie"
%cmake_build

%install
%cmake_install
rm -v %{buildroot}/%{_sysconfdir}/profile.d/katie-*.sh

%files
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
* Wed Dec 16 2020 Ivailo Monev <xakepa10@gmail.com> - 20201216git-0.1
- initial package
