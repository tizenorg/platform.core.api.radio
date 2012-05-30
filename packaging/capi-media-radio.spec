Name:       capi-media-radio
Summary:    A Radio library in Tizen Native API
Version:    0.1.0
Release:    8
Group:      TO_BE/FILLED_IN
License:    Apache-2.0
Source0:    %{name}-%{version}.tar.gz
Source1001: packaging/capi-media-radio.manifest 
BuildRequires:  pkgconfig(dbus-glib-1)
BuildRequires:  pkgconfig(dlog)
BuildRequires:  pkgconfig(vconf)
BuildRequires:  pkgconfig(mm-radio)
BuildRequires:  pkgconfig(capi-base-common)
BuildRequires:  cmake
BuildRequires:  gettext-devel

%description
A Radio library in Tizen Native API


%package devel 
Summary:    A Radio library in Tizen Native API (Developement)
Group:      TO_BE_FILLED 
Requires:   %{name} = %{version}-%{release}

%description devel
A Radio library in Tizen Native API (Developement)

%prep
%setup -q

%build
cp %{SOURCE1001} .
MAJORVER=`echo %{version} | awk 'BEGIN {FS="."}{print $1}'`
cmake . -DCMAKE_INSTALL_PREFIX=/usr -DFULLVER=%{version} -DMAJORVER=${MAJORVER}


make %{?jobs:-j%jobs}

%install
rm -rf %{buildroot}
%make_install

%post

%postun


%files
%manifest capi-media-radio.manifest
%defattr(-,root,root,-)
/usr/lib/libcapi-media-radio.so.*

%files devel 
%manifest capi-media-radio.manifest
%defattr(-,root,root,-)
/usr/include/media/radio.h
/usr/lib/pkgconfig/capi-media-radio.pc
/usr/lib/libcapi-media-radio.so
