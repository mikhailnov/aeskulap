if BUILD_NOT_MINGW32
pkglib_LTLIBRARIES = libconfiguration.la
else
lib_LTLIBRARIES = libconfiguration.la
endif

if USE_MINGW
extra_SOURCES=aconfiguration-win32.cpp
else
if USE_GSETTINGS
  extra_SOURCES=aconfiguration-gsettings.cpp
else
  extra_SOURCES=aconfiguration-gconf.cpp
endif
endif

libconfiguration_la_SOURCES = \
   awindowlevel.h \
   aconfiguration.h \
   aconfiguration.cpp \
   aconfigclient.h \
   aconfigclient.cpp \
   $(extra_SOURCES)

libconfiguration_la_LIBADD = \
   $(GCONFMM_LIBS) \
   $(GTKMM_LIBS)

libconfiguration_la_LDFLAGS = \
   $(LDFLAGS_AESKULAP_LIBS)

AM_CPPFLAGS = \
   -I$(top_srcdir) \
   $(GCONFMM_CFLAGS) \
   $(GTKMM_CFLAGS)

EXTRA_DIST = \
   aconfiguration-gsettings.cpp \
   aconfiguration-gconf.cpp
