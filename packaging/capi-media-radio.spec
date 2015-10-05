Name:       capi-media-radio
Summary:    A Radio library in Tizen Native API
Version:    0.1.2
Release:    0
Group:      Multimedia/API
License:    Apache-2.0
Source0:    %{name}-%{version}.tar.gz
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
%make_install


%post -p /sbin/ldconfig

%postun -p /sbin/ldconfig

%files
%manifest %{name}.manifest
%defattr(-,root,root,-)
%{_libdir}/libcapi-media-radio.so.*

%files devel
%manifest %{name}.manifest
%defattr(-,root,root,-)
/usr/include/media/radio.h
%{_libdir}/pkgconfig/capi-media-radio.pc
%{_libdir}/libcapi-media-radio.so
