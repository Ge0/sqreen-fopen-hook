#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_sqreen.h"
#include "zend.h"

ZEND_DECLARE_MODULE_GLOBALS(sqreen)

static zend_function_entry sqreen_functions[] = {
	PHP_FE(sqreenon, NULL)
	PHP_FE(sqreenoff, NULL)
	PHP_FE(sqreen_fopen, NULL)
	{NULL, NULL, NULL}
};

void restore_real_fopen()
{
	zend_string *str_fopen = zend_string_init(
		"fopen",
		sizeof("fopen") - 1,
		0
	);

	// Get the initial fopen function
	zend_function* zend_fopen = zend_hash_find_ptr(
		CG(function_table),
		str_fopen);
	zend_fopen->internal_function.handler =
		SQREEN_G(orig_fopen_handler);
}

PHP_MINIT_FUNCTION(sqreen)
{
}

PHP_MSHUTDOWN_FUNCTION(sqreen)
{
}

void hook_fopen()
{

	zend_string *str_fopen = zend_string_init(
		"fopen",
		sizeof("fopen") - 1,
		0
	);

	// Get the initial fopen function
	zend_function* zend_fopen = zend_hash_find_ptr(
		CG(function_table),
		str_fopen);

	// Save the original handler and set the hook
	SQREEN_G(orig_fopen_handler) =
		zend_fopen->internal_function.handler;
	zend_fopen->internal_function.handler = zif_sqreen_fopen;
}


/*
 * fopen() wrapper function. Log a message to stdout and call the real
 * fopen() PHP function.
 */
ZEND_FUNCTION(sqreen_fopen)
{
	zend_string* filename;
	zend_string* mode;
	zend_bool use_include_path;
	zval* context;

	ZEND_PARSE_PARAMETERS_START(2, 4)
		Z_PARAM_STR(filename)
		Z_PARAM_STR(mode)
		Z_PARAM_OPTIONAL
		Z_PARAM_BOOL(use_include_path)
		Z_PARAM_RESOURCE(context)
	ZEND_PARSE_PARAMETERS_END();

	zend_printf(
		"[!] fopen('%s','%s');\n",
		ZSTR_VAL(filename),
		ZSTR_VAL(mode)
	);

	zend_string *str_fopen = zend_string_init(
		"fopen",
		sizeof("fopen") - 1,
		0
	);

	// Convert zval properly
	zval function_name;
	
	ZVAL_STRING(&function_name, "fopen");

	zval args[4];
	ZVAL_STRING(&args[0], ZSTR_VAL(filename));
	ZVAL_STRING(&args[1], ZSTR_VAL(mode));
	ZVAL_BOOL(&args[2], &use_include_path);
	ZVAL_COPY(&args[3], context);

	// Forward the effective number of arguments passed.
	unsigned int nargs = ZEND_NUM_ARGS();

	// Return value
	zval retval;

	// Restore fopen
	restore_real_fopen();
	
	call_user_function(
		CG(function_table),
		NULL,
		&function_name,
		&retval,
		nargs,
		args TSRMLS_CC
	);

	// Restore the hook
	hook_fopen();

	RETURN_ZVAL(&retval, 1, 0);
}

zend_module_entry sqreen_module_entry = {
#if ZEND_MODULE_API_NO >= 20010901
	STANDARD_MODULE_HEADER,
#endif
	PHP_SQREEN_EXTNAME,
	sqreen_functions,
	PHP_MINIT(sqreen),
	PHP_MSHUTDOWN(sqreen),
	NULL,
	NULL,
	NULL,
#if ZEND_MODULE_API_NO >= 20010901
	PHP_SQREEN_VERSION,
#endif
	STANDARD_MODULE_PROPERTIES
};

#ifdef COMPILE_DL_SQREEN
ZEND_GET_MODULE(sqreen)
#endif


/*
 * Set up the hook on fopen standard PHP function.
 */
PHP_FUNCTION(sqreenon)
{
	zend_printf("[°] Activate Sqreen hook.\n");
	hook_fopen();
}

/*
 * Remove the hook on fopen standard PHP function.
 */
PHP_FUNCTION(sqreenoff)
{
	// Disable fopen hook
	zend_printf("[°] Disable Sqreen hook.\n");
	restore_real_fopen();
}
