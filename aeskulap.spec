%global git 29caae2
%global github jenslody-aeskulap
%global checkout git%{git}
%global checkout_date 20151122

Name:           aeskulap
Version:        0.2.2
Release:        0.23.beta1%{?dist}
Summary:        Full open source replacement for commercially available DICOM viewers

License:        LGPLv2+
URL:            https://github.com/jenslody/aeskulap

Source0:        https://github.com/jenslody/%{name}/tarball/master/%{github}-%{git}.tar.gz
Source1:        %{name}-tutorials.pdf

BuildRequires:   dcmtk-devel
BuildRequires:   intltool
BuildRequires:   libpng-devel
BuildRequires:   libjpeg-turbo-devel
BuildRequires:   libtiff-devel
BuildRequires:   libtool
BuildRequires:   openssl-devel
BuildRequires:   gettext-devel
BuildRequires:   tcp_wrappers-devel
BuildRequires:   desktop-file-utils
BuildRequires:   pkgconfig(gtkmm-2.4)
BuildRequires:   pkgconfig(libglademm-2.4)
BuildRequires:   pkgconfig(gthread-2.0)
BuildRequires:   pkgconfig(gconfmm-2.6)
BuildRequires:   libappstream-glib
BuildRequires:   GConf2
Requires(pre):   GConf2
Requires(post):  GConf2
Requires(preun): GConf2

%description
Aeskulap is able to load a series of special images stored in the
DICOM format for review. Additionally Aeskulap is able to query
and fetch DICOM images from archive nodes (also called PACS) over
the network.

The goal of this project is to create a full open source replacement
for commercially available DICOM viewers.

Aeskulap is based on gtkmm, glademm and gconfmm and designed to run
under Linux. Ports of these packages are available for different
platforms. It should be quite easy to port Aeskulap to any platform
were these packages are available.

%package doc
Summary: Documentation for aeskulap
Requires:        %{name}%{?_isa} = %{version}-%{release}
BuildArch:       noarch

%description doc
Documentation for aeskulap.

%prep
%setup -q -n %{github}-%{git}
autoreconf --force --install
intltoolize --force --copy --automake

%build
# point to the correct lib version depending on the arch
sed -i 's/lib -ldcmjpeg/%{_lib} -ldcmjpeg/' configure configure.ac

export CPPFLAGS='-std=c++11'
%configure --disable-static --disable-schemas-install
make %{?_smp_mflags} 

%install
export GCONF_DISABLE_MAKEFILE_SCHEMA_INSTALL=1
%make_install

install -p -m 0644 %{SOURCE1} -t .

desktop-file-validate %{buildroot}/%{_datadir}/applications/%{name}.desktop
appstream-util validate-relax --nonet %{buildroot}/%{_datadir}/appdata/%{name}.appdata.xml

# remove .la files.
find %{buildroot} -name "*.la" -exec rm -fv '{}' \;

%find_lang %{name}

%pre
%gconf_schema_prepare %{name}

%post
%gconf_schema_upgrade %{name}
touch --no-create %{_datadir}/icons/hicolor &>/dev/null || :
/usr/bin/update-desktop-database &> /dev/null || :

%preun
%gconf_schema_remove %{name}

%postun
if [ $1 -eq 0 ] ; then
    touch --no-create %{_datadir}/icons/hicolor &>/dev/null
    gtk-update-icon-cache %{_datadir}/icons/hicolor &>/dev/null || :
fi
/usr/bin/update-desktop-database &> /dev/null || :

%posttrans
gtk-update-icon-cache %{_datadir}/icons/hicolor &>/dev/null || :

%files -f %{name}.lang
%{_bindir}/%{name}
%{_libdir}/%{name}/
%{_datadir}/%{name}/
%{_datadir}/icons/hicolor/64x64/apps/%{name}.png
%{_datadir}/icons/hicolor/scalable/apps/%{name}.svg
%{_datadir}/applications/%{name}.desktop
%{_sysconfdir}/gconf/schemas/%{name}.schemas
%doc AUTHORS ABOUT-NLS ChangeLog README
%license COPYING

%files doc
%doc %{name}-tutorials.pdf

%changelog
* Sun Nov 22 2015 Jens Lody <fedora@jenslody.de> - 0.2.2-0.23.beta1
- Use own fork of github-repo.
- Remove dcmtk-sources.
- Include all patches in source.
- Update autotools, to remove deprecated macros and reduce warnings.

* Fri Nov 20 2015 Jens Lody <fedora@jenslody.de> - 0.2.2-0.22.beta1
- Use github-repo,
- reduced amount of patches.

* Thu Oct 08 2015 Jens Lody <fedora@jenslody.de> - 0.2.2-0.21beta1
- Add '-std=c++11' flag, because (at least) recent glibmm24 requires it.
- Update desktop-database, as it is required by packaging guidelines.
- Added url for doc-file,
- Split off doc-package (~ 1MB).

* Sun Oct 04 2015 Jens Lody <fedora@jenslody.de> - 0.2.2-0.20beta1
- Add patch to work with dcmtk 3.6.1
- Add patch to fix a crash in seriesview.cpp
- Do not clean buildroot before install
- Do not use defattr
- More consisten macro-usage

* Fri Aug 15 2014 Fedora Release Engineering <rel-eng@lists.fedoraproject.org> - 0.2.2-0.19beta1
- Rebuilt for https://fedoraproject.org/wiki/Fedora_21_22_Mass_Rebuild

* Sat Jun 07 2014 Fedora Release Engineering <rel-eng@lists.fedoraproject.org> - 0.2.2-0.18beta1
- Rebuilt for https://fedoraproject.org/wiki/Fedora_21_Mass_Rebuild

* Sat Aug 03 2013 Fedora Release Engineering <rel-eng@lists.fedoraproject.org> - 0.2.2-0.17beta1
- Rebuilt for https://fedoraproject.org/wiki/Fedora_20_Mass_Rebuild

* Thu Mar 14 2013 Ankur Sinha <ankursinha AT fedoraproject DOT org> 0.2.2-0.16beta1
- Update patch

* Thu Mar 14 2013 Ankur Sinha <ankursinha AT fedoraproject DOT org> 0.2.2-0.15beta1
- Update desktop file patch to correct mimetype

* Wed Feb 13 2013 Fedora Release Engineering <rel-eng@lists.fedoraproject.org> - 0.2.2-0.14beta1
- Rebuilt for https://fedoraproject.org/wiki/Fedora_19_Mass_Rebuild

* Fri Jan 18 2013 Adam Tkac <atkac redhat com> - 0.2.2-0.13beta1
- rebuild due to "jpeg8-ABI" feature drop

* Fri Dec 21 2012 Adam Tkac <atkac redhat com> - 0.2.2-0.12beta1
- rebuild against new libjpeg

* Wed Jul 18 2012 Fedora Release Engineering <rel-eng@lists.fedoraproject.org> - 0.2.2-0.11beta1
- Rebuilt for https://fedoraproject.org/wiki/Fedora_18_Mass_Rebuild

* Mon May 07 2012 Ankur Sinha <ankursinha AT fedoraproject DOT org> - 0.2.2-0.10beta1
- Spec bump for libtiff update and rebuild

* Tue Feb 28 2012 Fedora Release Engineering <rel-eng@lists.fedoraproject.org> - 0.2.2-0.9beta1
- Rebuilt for c++ ABI breakage

* Thu Jan 12 2012 Fedora Release Engineering <rel-eng@lists.fedoraproject.org> - 0.2.2-0.8beta1
- Rebuilt for https://fedoraproject.org/wiki/Fedora_17_Mass_Rebuild

* Fri Jan 06 2012 Ankur Sinha <ankursinha AT fedoraproject DOT org> - 0.2.2-0.7beta1
- spec bump for gcc 4.7 rebuild

* Tue Dec 06 2011 Adam Jackson <ajax@redhat.com> - 0.2.2-0.6beta1
- Rebuild for new libpng

* Sat Jul 09 2011 Ankur Sinha <ankursinha AT fedoraproject DOT org> - 0.2.2-0.5beta1
- Update scriptlets

* Sat Jul 09 2011 Ankur Sinha <ankursinha AT fedoraproject DOT org> - 0.2.2-0.4beta1
- Update license

* Mon Jul 04 2011 Ankur Sinha <ankursinha AT fedoraproject DOT org> - 0.2.2-0.3beta1
- remove autoconf patch and call autoreconf in the spec

* Wed Jun 29 2011 Ankur Sinha <ankursinha AT fedoraproject DOT org> - 0.2.2-0.2beta1
- get rid of devel package
- add tutorials as additional documentation

* Mon Jun 06 2011 Ankur Sinha <ankursinha AT fedoraproject DOT org> - 0.2.2-0.1beta1
- initial rpm build
