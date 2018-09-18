bin_PROGRAMS = aeskulap

aeskulap_SOURCES = \
	binreloc.h \
	binreloc.c \
	abusycursor.h \
	abusycursor.cpp \
	astockids.cpp \
	astockids.h \
	aiconfactory.cpp \
	aiconfactory.h \
	mainwindow.cpp \
	mainwindow.h \
	studymanager.cpp \
	studymanager.h \
	settings.cpp \
	settings.h \
	prescandialog.cpp \
	prescandialog.h \
	aboutdialog.h \
	aboutdialog.cpp \
	windowleveldialog.h \
	windowleveldialog.cpp \
	main.cpp

if BUILD_NOT_MINGW32
ICONOBJ =
else
ICONOBJ = icon.o
endif

icon.rc:
	echo "IDI_ICON1 ICON DISCARDABLE \"$(top_srcdir)/packaging/win32/aeskulap.ico\"" > icon.rc

icon.o: icon.rc
	$(target_alias)-windres -J rc -i icon.rc -o icon.o

aeskulap_LDADD = \
	$(ICONOBJ) \
	../widgets/libgtkdicom.la \
	../imagepool/libimagepool.la \
	../configuration/libconfiguration.la \
	$(GTKMM_LIBS) \
	$(GLADEMM_LIBS) \
	$(GTHREAD_LIBS) \
	$(GCONFMM_LIBS) \
	$(DCMTK_LIBS)

if BUILD_NOT_MINGW32

if  USE_GSETTINGS
# gsettings_SCHEMAS is a list of all the schemas you want to install
gsettings_SCHEMAS = org.gnu.aeskulap.gschema.xml

# include the appropriate makefile rules for schema handling
@GSETTINGS_RULES@

else 

schemadir = @GCONF_SCHEMA_FILE_DIR@
schema_DATA = aeskulap.schemas

@INTLTOOL_SCHEMAS_RULE@

if GCONF_SCHEMAS_INSTALL
install-data-local:
	GCONF_CONFIG_SOURCE=$(GCONF_SCHEMA_CONFIG_SOURCE) $(GCONFTOOL) --makefile-install-rule $(srcdir)/$(schema_DATA)
endif
endif 
endif

AM_CPPFLAGS = \
	-DAESKULAP_DATADIR=\""$(datadir)"\" \
	-DLOCALEDIR=\""$(datadir)/locale"\" \
	$(GTKMM_CFLAGS) \
	$(GLADEMM_CFLAGS) \
	$(GCONFMM_CFLAGS) \
	$(DCMTK_CFLAGS) \
	-I$(top_srcdir) \
	-I$(top_srcdir)/configuration \
	-I$(top_srcdir)/widgets \
	-I$(top_srcdir)/imagepool

gladedir = $(datadir)/aeskulap/glade
glade_DATA = aeskulap.glade

desktopdir = $(datadir)/applications
desktop_DATA = aeskulap.desktop

appdatadir = $(datadir)/appdata
appdata_DATA = aeskulap.appdata.xml

EXTRA_DIST = \
	aeskulap.glade \
	aeskulap.desktop \
	$(schema_DATA)
