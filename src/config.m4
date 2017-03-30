dnl config.m4 for extension handlersocketi

dnl Check PHP version:
AC_MSG_CHECKING(PHP version)
if test ! -z "$phpincludedir"; then
    PHP_VERSION=`grep 'PHP_VERSION ' $phpincludedir/main/php_version.h | sed -e 's/.*"\([[0-9\.]]*\)".*/\1/g' 2>/dev/null`
elif test ! -z "$PHP_CONFIG"; then
    PHP_VERSION=`$PHP_CONFIG --version 2>/dev/null`
fi

if test x"$PHP_VERSION" = "x"; then
    AC_MSG_WARN([none])
else
    PHP_MAJOR_VERSION=`echo $PHP_VERSION | sed -e 's/\([[0-9]]*\)\.\([[0-9]]*\)\.\([[0-9]]*\).*/\1/g' 2>/dev/null`
    PHP_MINOR_VERSION=`echo $PHP_VERSION | sed -e 's/\([[0-9]]*\)\.\([[0-9]]*\)\.\([[0-9]]*\).*/\2/g' 2>/dev/null`
    PHP_RELEASE_VERSION=`echo $PHP_VERSION | sed -e 's/\([[0-9]]*\)\.\([[0-9]]*\)\.\([[0-9]]*\).*/\3/g' 2>/dev/null`
    AC_MSG_RESULT([$PHP_VERSION])
fi

if test $PHP_MAJOR_VERSION -lt 5; then
    AC_MSG_ERROR([need at least PHP 5 or newer])
fi

PHP_ARG_ENABLE(handlersocketi, whether to enable handlersocketi support,
[  --enable-handlersocketi           Enable handlersocketi support])







if test "$PHP_HANDLERSOCKETI" != "no"; then

  PHP_REQUIRE_CXX()
  PHP_ADD_LIBRARY_WITH_PATH(stdc++, "", HANDLERSOCKETI_SHARED_LIBADD)
  PHP_SUBST(HANDLERSOCKETI_SHARED_LIBADD)

  PHP_NEW_EXTENSION(handlersocketi, handlersocketi.c handlersocketi_exception.c handlersocketi_class.c handlersocketi_index.c util/request.c util/response.c SimpleSQLWhere.cpp, $ext_shared)

  ifdef([PHP_INSTALL_HEADERS],
  [
    PHP_INSTALL_HEADERS([ext/handlersocketi/], [php_handlersocketi.h handlersocketi_exception.h handlersocketi_class.h handlersocketi_index.h])
  ], [
    PHP_ADD_MAKEFILE_FRAGMENT
  ])
fi
