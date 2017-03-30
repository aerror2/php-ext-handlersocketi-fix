
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"

#include "php_verdep.h"
#include "php_handlersocketi.h"
#include "handlersocketi_exception.h"
#include "handlersocketi_class.h"
#include "handlersocketi_index.h"

ZEND_DECLARE_MODULE_GLOBALS(handlersocketi)

static void
hs_init_globals(zend_handlersocketi_globals *hs_globals)
{
    hs_globals->id = 1;
}




ZEND_MINIT_FUNCTION(handlersocketi)
{
    ZEND_INIT_MODULE_GLOBALS(handlersocketi, hs_init_globals, NULL);
    /*
    REGISTER_INI_ENTRIES();
    */

    handlersocketi_register_exception(TSRMLS_C);
    handlersocketi_register_class(TSRMLS_C);
    handlersocketi_register_index(TSRMLS_C);

    /*
    REGISTER_LONG_CONSTANT("MSGPACKI_MODE_ORIGIN", PHP_MSGPACKI_MODE_ORIGIN,
                           CONST_CS | CONST_PERSISTENT);

    REGISTER_STRING_CONSTANT("MSGPACKI_FILTER_REGISTER",
                             PHP_MSGPACKI_FILTER_REGISTER,
                             CONST_CS | CONST_PERSISTENT);
    */


/* constant
#if ZEND_MODULE_API_NO < 20050922
    REGISTER_STRING_CONSTANT(
        "HANDLERSOCKETI_PRIMARY", HS_PRIMARY, CONST_CS | CONST_PERSISTENT);
    REGISTER_STRING_CONSTANT(
        "HANDLERSOCKETI_UPDATE", HS_MODIFY_UPDATE, CONST_CS | CONST_PERSISTENT);
    REGISTER_STRING_CONSTANT(
        "HANDLERSOCKETI_DELETE", HS_MODIFY_REMOVE, CONST_CS | CONST_PERSISTENT);
#else
    zend_declare_class_constant_string(
        hs_ce, "PRIMARY", strlen("PRIMARY"), HS_PRIMARY TSRMLS_CC);
    zend_declare_class_constant_string(
        hs_ce, "UPDATE", strlen("UPDATE"), HS_MODIFY_UPDATE TSRMLS_CC);
    zend_declare_class_constant_string(
        hs_ce, "DELETE", strlen("DELETE"), HS_MODIFY_REMOVE TSRMLS_CC);
#endif
*/

    return SUCCESS;
}

ZEND_MSHUTDOWN_FUNCTION(handlersocketi)
{
    /*
    UNREGISTER_INI_ENTRIES();
    */

    return SUCCESS;
}

/*
ZEND_RINIT_FUNCTION(handlersocketi)
{
    return SUCCESS;
}

ZEND_RSHUTDOWN_FUNCTION(handlersocketi)
{
    return SUCCESS;
}
*/

ZEND_MINFO_FUNCTION(handlersocketi)
{
    php_info_print_table_start();
    php_info_print_table_header(2, "handlersocketi support", "enabled");
    php_info_print_table_header(2, "extension version",
                                HANDLERSOCKETI_EXT_VERSION);
    php_info_print_table_end();

    DISPLAY_INI_ENTRIES();
}


ZEND_BEGIN_ARG_INFO_EX(arginfo_hsi_parse_sql_where, 0, 0, 1)
    ZEND_ARG_INFO(0, query)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_hsi_startswith, 0, 0, 2)
    ZEND_ARG_INFO(0, haystack)
    ZEND_ARG_INFO(0, needle)
ZEND_END_ARG_INFO()


PHP_FUNCTION(hsi_parse_sql_where);

PHP_FUNCTION(hsi_startswith);

const zend_function_entry handlersocketi_functions[] = {
    PHP_FE(hsi_parse_sql_where, arginfo_hsi_parse_sql_where)
    PHP_FE(hsi_startswith, arginfo_hsi_startswith)
    ZEND_FE_END
};

zend_module_entry handlersocketi_module_entry = {
#if ZEND_MODULE_API_NO >= 20010901
    STANDARD_MODULE_HEADER,
#endif
    "handlersocketi",
    handlersocketi_functions,
    ZEND_MINIT(handlersocketi),
    ZEND_MSHUTDOWN(handlersocketi),
    NULL, /* ZEND_RINIT(handlersocketi), */
    NULL, /* ZEND_RSHUTDOWN(handlersocketi), */
    ZEND_MINFO(handlersocketi),
#if ZEND_MODULE_API_NO >= 20010901
    HANDLERSOCKETI_EXT_VERSION,
#endif
    STANDARD_MODULE_PROPERTIES
};

#ifdef COMPILE_DL_HANDLERSOCKETI
ZEND_GET_MODULE(handlersocketi)
#endif





void parse_simple_mysql_where(const char * where , int   whereLen,  zval *return_value TSRMLS_DC);

PHP_FUNCTION(hsi_parse_sql_where)
{
    char *szWhere= NULL;
    int   szWhereLen = 0;


    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s",
                              &szWhere, &szWhereLen) == FAILURE) {
         RETURN_FALSE;
    }
    parse_simple_mysql_where(szWhere,szWhereLen,return_value TSRMLS_CC );
}



PHP_FUNCTION(hsi_startswith)
{
  
  char *main_string = NULL;
  char *needle = NULL;

  int main_string_len  = 0;
  int needle_len  =0;
 

  if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ss",
                              &main_string, &main_string_len,&needle,&needle_len) == FAILURE) {
         RETURN_FALSE;
  }

  if(main_string_len<needle_len)
  {
     RETURN_FALSE;
  }
  
  
  if(memcmp(main_string,needle,needle_len)==0)
  {
         RETURN_TRUE;
  }
  else
  {
        RETURN_FALSE;
  }
}


