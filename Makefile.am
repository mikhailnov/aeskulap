SUBDIRS = \
	po \
	configuration \
	imagepool \
	widgets \
	src \
	packaging \
	pixmaps

DIST_SUBDIRS = \
	po \
	configuration \
	imagepool \
	widgets \
	src \
	packaging \
	pixmaps

EXTRA_DIST = \
	AUTHORS \
	COPYING \
	COPYING.LIB \
	INSTALL \
	README \
	aeskulap.cbp \
	aeskulap.workspace \
	gettext.h \
	intltool-extract.in \
	intltool-merge.in \
	intltool-update.in \
	configure.mingw32

DISTCLEANFILES = \
	intltool-extract \
	intltool-merge \
	intltoo-update

@INTLTOOL_DESKTOP_RULE@

distclean: distclean-recursive
	-rm -f $(am__CONFIG_DISTCLEAN_FILES)
	-rm -f Makefile

rpm: dist
	mkdir -p rpmbuild
	cp $(PACKAGE)-$(VERSION).tar.gz rpmbuild
	rpmbuild --buildroot rpmbuild -ba packaging/rpm/$(PACKAGE).spec

dist-win32:
	-rm -Rf /tmp/$(PACKAGE)-$(VERSION)
	make prefix=/tmp/$(PACKAGE)-$(VERSION)$(prefix) install-strip
	-cp -R -f /tmp/$(PACKAGE)-$(VERSION)$(prefix)/* /tmp/$(PACKAGE)-$(VERSION)
	-rm -Rf /tmp/$(PACKAGE)-$(VERSION)$(prefix)
	-rm -Rf /tmp/$(PACKAGE)-$(VERSION)/usr
	if [ -f /tmp/$(PACKAGE)-$(VERSION)/bin/$(target_alias)-aeskulap.exe ] ; then \
	    mv /tmp/$(PACKAGE)-$(VERSION)/bin/$(target_alias)-aeskulap.exe /tmp/$(PACKAGE)-$(VERSION)/bin/aeskulap.exe ; \
	fi
	for i in ofstd ijg8 ijg12 ijg16 dcmjpeg dcmdata dcmnet dcmimgle dcmimage dcmtls ; do \
	    cp $(prefix)/bin/lib$$i.dll /tmp/$(PACKAGE)-$(VERSION)/bin ; \
	done
	-cp -f $(prefix)/lib/dicom.dic /tmp/$(PACKAGE)-$(VERSION)
	-cp -f $(top_srcdir)/COPYING /tmp/$(PACKAGE)-$(VERSION)/license.txt
	-cp -f $(top_srcdir)/packaging/win32/aeskulap.iss /tmp/$(PACKAGE)-$(VERSION)
	-cp -f $(top_srcdir)/packaging/win32/aeskulap-setup-small.bmp /tmp/$(PACKAGE)-$(VERSION)
	-cp -f $(top_srcdir)/packaging/win32/aeskulap-setup-large.bmp /tmp/$(PACKAGE)-$(VERSION)
	-cp -f /tmp/$(PACKAGE)-$(VERSION)/share/icons/hicolor/48x48/apps/aeskulap.png  /tmp/$(PACKAGE)-$(VERSION)/share/aeskulap/images
	$(shell) $(STRIP) -g -S -d --strip-debug --strip-unneeded -x -X --remove-section=.comment --remove-section=.note /tmp/$(PACKAGE)-$(VERSION)/bin/*.exe
	$(shell) $(STRIP) -g -S -d --strip-debug --strip-unneeded -x -X --remove-section=.comment --remove-section=.note /tmp/$(PACKAGE)-$(VERSION)/bin/*.dll
	-mv /tmp/$(PACKAGE)-$(VERSION)/bin/* /tmp/$(PACKAGE)-$(VERSION)
	-rm -Rf /tmp/$(PACKAGE)-$(VERSION)/bin
	-cp -R -f /tmp/$(PACKAGE)-$(VERSION)/lib/locale /tmp/$(PACKAGE)-$(VERSION)/share
	-rm -Rf /tmp/$(PACKAGE)-$(VERSION)/lib
	-rm -f /tmp/$(PACKAGE)-$(VERSION)/share/icons/hicolor/icon-theme.cache
	-rm -Rf /tmp/$(PACKAGE)-$(VERSION)/share/applications
	cd /tmp && zip -r -9 $(PACKAGE)-$(VERSION).zip $(PACKAGE)-$(VERSION)
	-rm -Rf /tmp/$(PACKAGE)-$(VERSION)
	mv /tmp/$(PACKAGE)-$(VERSION).zip .

push-dist: dist
	scp $(PACKAGE)-$(VERSION).tar.gz pipelka@www.bms-austria.com:/home/pipelka/public_html/aeskulap

push-dist-win32: dist-win32
	scp $(PACKAGE)-$(VERSION).zip pipelka@www.bms-austria.com:/home/pipelka/public_html/aeskulap/win32
