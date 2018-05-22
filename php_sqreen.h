#ifndef PHP_SQREEN_H
#define PHP_SQREEN_H 1

#define PHP_SQREEN_VERSION "0.1"
#define PHP_SQREEN_EXTNAME "sqreen"

#define SQREEN_HOOK_ACTIVE 1
#define SQREEN_HOOK_INACTIVE 0

#ifdef ZTS
#include "TSRM.h"
#endif

PHP_FUNCTION(sqreenon);
PHP_FUNCTION(sqreenoff);
PHP_FUNCTION(sqreen_fopen);
PHP_FUNCTION(__save_sqreen_fopen);

ZEND_BEGIN_MODULE_GLOBALS(sqreen)
	void (*orig_fopen_handler)(INTERNAL_FUNCTION_PARAMETERS);
ZEND_END_MODULE_GLOBALS(sqreen)

#ifdef ZTS
#define SQREEN_G(v) TSRMG(sqreen_globals_id, zend_sqreen_globals *, v)
#else
#define SQREEN_G(v) (sqreen_globals.v)
#endif

extern zend_module_entry sqreen_module_entry;
#define phpext_sqreen_ptr &sqreen_module_entry

#endif
