Name:       capi-media-radio
Summary:    A Radio library in Tizen Native API
Version:    0.1.2
Release:    13
Group:      API/C API
License:    Apache-2.0
Source0:    %{name}-%{version}.tar.gz
Source1001: 	capi-media-radio.manifest
BuildRequires:  pkgconfig(dlog)
BuildRequires:  pkgconfig(mm-radio)
BuildRequires:  pkgconfig(capi-base-common)
BuildRequires:  pkgconfig(capi-system-info)
BuildRequires:  cmake

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
cp %{SOURCE1001} .

%build
MAJORVER=`echo %{version} | awk 'BEGIN {FS="."}{print $1}'`
%cmake .  -DFULLVER=%{version} -DMAJORVER=${MAJORVER}

make %{?jobs:-j%jobs}

%install
rm -rf %{buildroot}
%make_install
mkdir -p %{buildroot}/usr/share/license
mkdir -p %{buildroot}/usr/bin
cp LICENSE.APLv2 %{buildroot}/usr/share/license/%{name}
cp test/radio_test %{buildroot}/usr/bin


%post -p /sbin/ldconfig

%postun -p /sbin/ldconfig

%files
%manifest %{name}.manifest
%defattr(-,root,root,-)
%{_libdir}/libcapi-media-radio.so.*
%{_datadir}/license/%{name}
%{_bindir}/*

%files devel
%manifest %{name}.manifest
%defattr(-,root,root,-)
/usr/include/media/radio.h
%{_libdir}/pkgconfig/capi-media-radio.pc
%{_libdir}/libcapi-media-radio.so
