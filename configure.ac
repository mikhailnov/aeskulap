AC_PREREQ([2.69])

m4_define([AESKULAP_VERSION], 0.2.2-beta1)

AC_INIT(aeskulap, AESKULAP_VERSION)

AC_CONFIG_SRCDIR([README])

AC_CANONICAL_TARGET

AM_CONFIG_HEADER(config.h)

AC_CONFIG_MACRO_DIR([m4])

AM_INIT_AUTOMAKE

AC_SUBST(ACLOCAL_AMFLAGS, "$ACLOCAL_FLAGS")

case "$target" in
    *cygwin* | *mingw32*)
	MINGW32="yes"
	;;
    *)
	MINGW32="no"
	;;
esac

dnl
dnl Require programs
dnl
IT_PROG_INTLTOOL
LT_INIT
AC_PROG_CC
AC_PROG_CXX
AC_PROG_INSTALL
if test x"$MINGW32" = xno ; then
AM_BINRELOC
else
GMSGFMT=msgfmt
AC_SUBST(GMSGFMT)
fi

AM_GCONF_SOURCE_2

dnl
dnl gettext package
dnl
GETTEXT_PACKAGE=aeskulap
AC_SUBST(GETTEXT_PACKAGE)
AC_DEFINE_UNQUOTED(GETTEXT_PACKAGE,"$GETTEXT_PACKAGE",[Translation domain used])

ALL_LINGUAS="`cat "$srcdir/po/LINGUAS" | grep -v '^#'`"
AC_SUBST([CONFIG_STATUS_DEPENDENCIES],['$(top_srcdir)/po/LINGUAS'])

AM_GLIB_GNU_GETTEXT

dnl
dnl Required headers
dnl
AC_HEADER_STDC

dnl
dnl Checks for typedefs, structures, and compiler characteristics.
dnl
AC_TYPE_MODE_T
AC_TYPE_PID_T
AC_HEADER_TIME

AC_CHECK_LIB(jpeg,jpeg_read_header)
AC_CHECK_LIB(tiff,TIFFClose)
AC_CHECK_LIB(png,png_create_read_struct)

PKG_CHECK_MODULES(GTKMM, gtkmm-2.4 >= 2.6.0)
AC_SUBST(GTKMM_CFLAGS)
AC_SUBST(GTKMM_LIBS)

PKG_CHECK_MODULES(GLADEMM, libglademm-2.4 >= 2.6.0)
AC_SUBST(GLADEMM_CFLAGS)
AC_SUBST(GLADEMM_LIBS)

PKG_CHECK_MODULES(GTHREAD, gthread-2.0)
AC_SUBST(GTHREAD_CFLAGS)
AC_SUBST(GTHREAD_LIBS)

if test x"$MINGW32" = xno ; then

PKG_CHECK_MODULES(GCONFMM, gconfmm-2.6 >= 2.10.0)
AC_SUBST(GCONFMM_CFLAGS)
AC_SUBST(GCONFMM_LIBS)

AC_PATH_PROG(GCONFTOOL, gconftool-2, no)
if test x"$GCONFTOOL" = xno ; then
	AC_MSG_ERROR([gconftool2 executable not found in your path - should be 
installed with GConf])
fi

fi


dnl use system dcmtk
dnl AC_CHECK_LIB(dcmimage)
AC_CHECK_HEADER(dcmtk/config/osconfig.h)

AC_SUBST(DCMTK_CFLAGS)
AC_SUBST(DCMTK_LIBS)

DCMTK_CFLAGS="-DHAVE_CONFIG_H -I\$(prefix)/include"
DCMTK_LIBS="-L\$(prefix)/lib -ldcmjpeg -lijg8 -lijg12 -lijg16 -ldcmimage -ldcmimgle -ldcmnet -ldcmdata -loflog -lofstd"

dnl set PREFIX
if test "x${prefix}" = "xNONE"; then
    packageprefix=${ac_default_prefix}
else
    packageprefix=${prefix}
fi

if test x"$MINGW32" = xno ; then
LDFLAGS_AESKULAP_LIBS="-avoid-version"
else
LDFLAGS_MINGW32="-mwindows"
LDFLAGS_AESKULAP_LIBS="-avoid-version -no-undefined"
fi

AC_SUBST(LDFLAGS_MINGW32)
AC_SUBST(LDFLAGS_AESKULAP_LIBS)

AM_CONDITIONAL(BUILD_NOT_MINGW32, test x$MINGW32 = xno)

AC_CONFIG_FILES([
po/Makefile.in
Makefile
configuration/Makefile
imagepool/Makefile
src/Makefile
widgets/Makefile
pixmaps/Makefile
packaging/Makefile
packaging/autopackage/Makefile
packaging/autopackage/default.apspec
packaging/rpm/Makefile
packaging/rpm/aeskulap.spec
packaging/win32/Makefile
packaging/win32/aeskulap.iss
])
AC_OUTPUT

