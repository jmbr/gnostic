#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

###############################################################################
#
# configure.ac -- Automagick configuration script
#
###############################################################################

AC_PREREQ(2.59)
define([gnostic_version], [0.2.x])
AC_INIT([gnostic],[gnostic_version],[rwx+gnostic@synnergy.net])
undefine([gnostic_version])

AC_REVISION($Revision$)

AC_CONFIG_SRCDIR([gnostic.c])
AC_CONFIG_HEADER([config.h])

PACKAGE="gnostic"
VERSION=`$srcdir/shtool version -l c -d short version.h`
VERSION_L=`$srcdir/shtool version -l c -d long version.h`

B=`$srcdir/shtool echo -e %B`
b=`$srcdir/shtool echo -e %b`
echo "Configuring $B$PACKAGE$b version $B$VERSION_L$b"
echo "Copyright (C) 2002, 2003, 2004 Juan M. Bello Rivas <rwx+gnostic@synnergy.net>"
echo ""

AC_SUBST(PACKAGE)
AC_SUBST(VERSION)

# Checks for programs.
AC_PROG_CC
AC_PROG_CXX
if test -n "$GCC"; then
  CFLAGS="-Wall -Wshadow -std=gnu99 $CFLAGS"
fi
AC_PROG_LEX
AC_PROG_YACC
AC_PROG_INSTALL
AC_PROG_MAKE_SET
AC_PATH_PROG(DOXYGEN, doxygen)
AC_PATH_PROG(MAKEINFO, makeinfo)
AC_PATH_PROG(TEXI2PDF, texi2pdf)

# Checks for libraries.

# Checks for header files.
INCLUDES="-I$srcdir"
AC_SUBST(INCLUDES)
AC_HEADER_STDC
AC_HEADER_TIME
AC_HEADER_STDBOOL
AC_HEADER_SYS_WAIT
AC_CHECK_HEADERS([limits.h memory.h stddef.h stdlib.h string.h strings.h unistd.h signal.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST
AC_C_INLINE
AC_TYPE_PID_T
AC_TYPE_SIZE_T
AC_TYPE_SIGNAL

# Checks for library functions.
AC_FUNC_FORK
AC_FUNC_MALLOC
AC_FUNC_REALLOC
AC_FUNC_VPRINTF
AC_CHECK_FUNCS([memset strdup strerror putenv])

AC_CONFIG_FILES([GNUmakefile tests/GNUmakefile doc/GNUmakefile doxy.cfg])
AC_CONFIG_COMMANDS([stamp-h], [echo timestamp > stamp-h])
AC_OUTPUT

echo ""
echo "Configuration finished. You can now run gmake."
