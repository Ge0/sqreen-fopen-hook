// Those are just random notes. Do not compile.

void test_fopen_user_call() {
	zval fname, args[2], retval;

	ZVAL_NEW_STR(
		&fname,
		zend_string_init("fopen", sizeof("fopen") - 1, 0)
	);

	ZVAL_NEW_STR(
		&args[0],
		zend_string_init("/tmp/tototo", sizeof("/tmp/tototo") - 1, 0)
	);

	ZVAL_NEW_STR(
		&args[1],
		zend_string_init("w", sizeof("w") - 1, 0)
	);

	printf(
		"values %s %s %s\n",
		Z_STR(fname)->val,
		Z_STR(args[0])->val,
		Z_STR(args[1])->val
	);

	if (call_user_function(
		EG(function_table),
		NULL,
		&fname,
		&retval,
		2,
		args TSRMLS_CC) == SUCCESS) 
	{
		zend_printf("Success with fopen()\n");
		zval_ptr_dtor(&retval);
		zval_ptr_dtor(&args[1]);
		zval_ptr_dtor(&args[0]);
	}
}

void debug_info() {
	zend_string* str_fopen = zend_string_alloc(
		strlen("fopen"),
		0
	);
	zend_str_tolower_copy(
		ZSTR_VAL(str_fopen),
		"fopen",
		strlen("fopen")
	);

	zend_printf("Str = %s\n", ZSTR_VAL(str_fopen));

	zend_internal_function* fopen_function = zend_hash_find_ptr(
		CG(function_table),
		str_fopen);
	zend_printf("fopen is at %p\n", fopen_function);
	zend_printf("name is %s\n", ZSTR_VAL(fopen_function->function_name));
}

