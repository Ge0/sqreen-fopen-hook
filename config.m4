PHP_ARG_ENABLE(sqreen, Enable Sqreen extension or not,
               [--enable-sqreen Enablr Sqreen support])
if test "$PHP_SQREEN" = "yes"; then
  AC_DEFINE(HAVE_SQREEN, 1, [Whether you have Sqreen or not])
  PHP_NEW_EXTENSION(sqreen, sqreen.c, $ext_shared)
fi
