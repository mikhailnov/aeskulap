%global git ce82f85
%global github jenslody-aeskulap
%global checkout git%{git}
%global checkout_date 20180219

Name:           aeskulap
Version:        0.2.2
Release:        0.35.beta2%{?dist}
Summary:        Full open source replacement for commercially available DICOM viewers
# The sources of the (internal) libraries are LGPLv2+, the rest of the sources are GPLv2+,
# except binreloc.{c,h} and the documentation, which are in the public domain
License:        GPLv2+ and LGPLv2+ and Public Domain
URL:            https://github.com/jenslody/aeskulap

Source0:        https://github.com/jenslody/%{name}/tarball/master/%{github}-%{git}.tar.gz

BuildRequires:   gcc-c++
BuildRequires:   dcmtk-devel
BuildRequires:   intltool
BuildRequires:   libpng-devel
BuildRequires:   libjpeg-turbo-devel
BuildRequires:   libtiff-devel
BuildRequires:   libtool
BuildRequires:   openssl-devel
BuildRequires:   gettext-devel
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

desktop-file-validate %{buildroot}/%{_datadir}/applications/%{name}.desktop
appstream-util validate-relax --nonet %{buildroot}/%{_datadir}/appdata/%{name}.appdata.xml

%find_lang %{name}

%files -f %{name}.lang
%{_bindir}/%{name}
%{_libdir}/%{name}/
%{_datadir}/%{name}/
%{_datadir}/icons/hicolor/64x64/apps/%{name}.png
%{_datadir}/icons/hicolor/scalable/apps/%{name}.svg
%{_datadir}/applications/%{name}.desktop
%{_datadir}/appdata/%{name}.appdata.xml
%{_datadir}/glib-2.0/schemas/org.gnu.aeskulap.gschema.xml
%exclude %{_libdir}/%{name}/*.la
%doc AUTHORS ABOUT-NLS ChangeLog README
%license COPYING COPYING.LIB

%files doc
%doc doc/%{name}-tutorials.pdf
%license COPYING.DOC

%changelog
* Mon Feb 19 2018 Jens Lody <fedora@jenslody.de> - 0.2.2-0.35.beta2
- New beta release (switch to gsettings).
- Add BuildRequires for c++.
- Fix build on Rawhide.

* Wed Feb 07 2018 Fedora Release Engineering <releng@fedoraproject.org> - 0.2.2-0.34.beta1
- Rebuilt for https://fedoraproject.org/wiki/Fedora_28_Mass_Rebuild

* Sun Jan 07 2018 Igor Gnatenko <ignatenkobrain@fedoraproject.org> - 0.2.2-0.33.beta1
- Remove obsolete scriptlets

* Tue Jan 02 2018 Igor Gnatenko <ignatenko@redhat.com> - 0.2.2-0.32.beta1
- Rebuild for dcmtk

* Tue Jan 02 2018 Jens Lody <fedora@jenslody.de> - 0.2.2-0.31.beta1
- Rebuilt for dcmtk-update.

* Tue Dec 12 2017 Jens Lody <fedora@jenslody.de> - 0.2.2-0.30.beta1
- Build fixes for dcmtk >= 3.6.2.
- Don't link with deprecated tcp-wrapper, fixes rhbz #1518750.

* Wed Aug 02 2017 Fedora Release Engineering <releng@fedoraproject.org> - 0.2.2-0.29.beta1
- Rebuilt for https://fedoraproject.org/wiki/Fedora_27_Binutils_Mass_Rebuild

* Wed Jul 26 2017 Fedora Release Engineering <releng@fedoraproject.org> - 0.2.2-0.28.beta1
- Rebuilt for https://fedoraproject.org/wiki/Fedora_27_Mass_Rebuild

* Fri Feb 10 2017 Fedora Release Engineering <releng@fedoraproject.org> - 0.2.2-0.27.beta1
- Rebuilt for https://fedoraproject.org/wiki/Fedora_26_Mass_Rebuild

* Wed Feb 03 2016 Fedora Release Engineering <releng@fedoraproject.org> - 0.2.2-0.26.beta1
- Rebuilt for https://fedoraproject.org/wiki/Fedora_24_Mass_Rebuild

* Tue Dec 01 2015 Jens Lody <fedora@jenslody.de> - 0.2.2-0.25.beta1
- Add license file, to doc-package.

* Tue Nov 24 2015 Jens Lody <fedora@jenslody.de> - 0.2.2-0.24.beta1
- Fix dependency for doc- and debuginfo-package.
- Fix license strings in spec-file and appdata.
- Enable hardened build for F22
- Added COPYING.LIB.

* Sun Nov 22 2015 Jens Lody <fedora@jenslody.de> - 0.2.2-0.23.beta1
- Use own fork of github-repo.
- Remove dcmtk-sources.
- Include all patches in source.
- Update autotools, to remove deprecated macros and reduce warnings.
- Add appdata-file.

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
