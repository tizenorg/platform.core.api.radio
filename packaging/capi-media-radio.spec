Name:       capi-media-radio
Summary:    A Radio library in Tizen Native API
Version:    0.1.0
Release:    10
Group:      API/C API
License:    Apache-2.0
Source0:    %{name}-%{version}.tar.gz
BuildRequires:  pkgconfig(dbus-glib-1)
BuildRequires:  pkgconfig(dlog)
BuildRequires:  pkgconfig(vconf)
BuildRequires:  pkgconfig(mm-radio)
BuildRequires:  pkgconfig(capi-base-common)
BuildRequires:  cmake
BuildRequires:  gettext-devel

%description
A Radio library in Tizen Native API.


%package devel 
Summary:    A Radio library in Tizen Native API (Development)
Group:      Development/API
Requires:   %{name} = %{version}-%{release}

%description devel
A Radio library in Tizen Native API (Development).

%prep
%setup -q

%build
MAJORVER=`echo %{version} | awk 'BEGIN {FS="."}{print $1}'`
%cmake .  -DFULLVER=%{version} -DMAJORVER=${MAJORVER}

make %{?jobs:-j%jobs}

%install
%make_install


%post -p /sbin/ldconfig

%postun -p /sbin/ldconfig

%files
%manifest capi-media-radio.manifest
%defattr(-,root,root,-)
%{_libdir}/libcapi-media-radio.so.*

%files devel 
%defattr(-,root,root,-)
/usr/include/media/radio.h
%{_libdir}/pkgconfig/capi-media-radio.pc
%{_libdir}/libcapi-media-radio.so
