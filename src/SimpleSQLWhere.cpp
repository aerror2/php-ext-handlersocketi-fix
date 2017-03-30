/**
@author dany
*/

#include <boost/spirit/core.hpp>

#include <boost/spirit/utility/confix.hpp>
#include <boost/spirit/utility/escape_char.hpp>
#include <string>
#include "SimpleSQLWhere.h"

#include "php.h"
#include "php_ini.h"
#include "php_streams.h"
#include "ext/standard/php_smart_str.h"



#define SSW_RETURN_LONG(X)  SQLParser::cleanup(); RETVAL_LONG(X); return



my_stack_t SQLParser::m_stack;

static inline zval
*ssw_array_add(zval *return_value TSRMLS_DC)
{
    zval *value;
    MAKE_STD_ZVAL(value);
    array_init(value);
    add_next_index_zval(return_value, value);
    return value;
}



extern "C" void parse_simple_mysql_where(const char * where , int   whereLen, zval *return_value TSRMLS_DC)
{
	
	try
	{

		SQLParser::setup();
		SimpleSQLWhereGrammar g;	 
		
		
    	iterator_t begin(where, where+whereLen);
    	iterator_t end;

		parse_info < iterator_t > info2 = parse (begin, end, g);

		if (info2.full)
		{
		
			//std::cout << "\nParse succeeded! "<< where <<"\n";
			//std::list<plain_result_t> critiList;
			
			array_init(return_value);
			zval *ret_criti_arr = ssw_array_add(return_value TSRMLS_CC);

    		int  limitNum = 0;
			for(stack_iter_t it= SQLParser::m_stack.begin(); it != SQLParser::m_stack.end(); it++)
			{
				Token *t = *it;

				if(t->getType()==token_criti)
				{
					//std::cout << "criti: ";
					//plain_result_t  rcriti;

					zval * ret_criti = ssw_array_add(ret_criti_arr TSRMLS_CC);

					my_stack_t *list = (my_stack_t*) t->getData();
					int elemNum = 0;
					for(stack_iter_t ev = list->begin();ev!=list->end();ev++)
					{
						Token *pv = *ev;
						if(pv->getType()==token_identifer)
						{
							//std::cout <<  ((std::string *)pv->getData())->c_str() << " ";
							std::string *lpsz = (std::string *)pv->getData();
							add_next_index_stringl(ret_criti, lpsz->c_str(),lpsz->length(), 1);
						}
						else if(pv->getType()==token_operator)
						{
							//std::cout <<  ((std::string *)pv->getData())->c_str() << " ";
							std::string *lpsz = (std::string *)pv->getData();
							add_next_index_stringl(ret_criti, lpsz->c_str(),lpsz->length(), 1);
						}
						else if(pv->getType()==token_const)
						{
							//std::cout <<  ((std::string *)pv->getData())->c_str() << " ";
							std::string *lpsz = (std::string *)pv->getData();
							add_next_index_stringl(ret_criti, lpsz->c_str(),lpsz->length(), 1);
						}
						else if(pv->getType()==token_values_array)
						{
							my_stack_t *pvls = (my_stack_t *)pv->getData();
							zval * inarr = ssw_array_add(ret_criti TSRMLS_CC);

							for(stack_iter_t pvli = pvls->begin();pvli!=pvls->end();pvli++)
							{
								Token *pplv = *pvli;
								if(pplv->getType()==token_const)
								{
									//std::cout << "," <<  ((std::string *)pplv->getData())->c_str() ;
									std::string *lpsz = (std::string *)pplv->getData();
									add_next_index_stringl(inarr, lpsz->c_str(),lpsz->length(), 1);
								}
								else
								{
									//std::cout << "--wrong token type"<< ((int)pplv->getType()) << "---";
									
									SSW_RETURN_LONG(-1);
								}
							}

						}
						else
						{
							//std::cout << "--wrong token type"<< ((int)pv->getType()) << "---";
						
							SSW_RETURN_LONG(-2);
						}

					}

					////std::cout << "\n";
				}
				else if(t->getType()==token_limit)
				{
					//std::cout << "limit: ";
					if(limitNum>0)
					{
						SSW_RETURN_LONG(-9);
					}
					zval *ret_limit = ssw_array_add(return_value TSRMLS_CC);

					my_stack_t *pvls = (my_stack_t *)t->getData();

					for(stack_iter_t pvli = pvls->begin();pvli!=pvls->end();pvli++)
					{

						Token *pplv = *pvli;
						if(pplv->getType()==token_const)
						{
							//std::cout << "," <<  ((std::string *)pplv->getData())->c_str() ;
							std::string *lpsz = (std::string *)pplv->getData();
							add_next_index_stringl(ret_limit, lpsz->c_str(),lpsz->length(), 1);
						}
						else
						{
							//std::cout << "--wrong token type"<< ((int)pplv->getType()) << "---";
							SSW_RETURN_LONG(-3);
						}
					}

					//std::cout << "\n";
				}
				else
				{
					//std::cout << "\nParse result should be criti or limit:" << where << "\n";
					SSW_RETURN_LONG(-4);
				}
			}

			SQLParser::cleanup();
			return ;
		}
		else
		{
			//std::cout << "\nParse failed:" << where << "\n";
			//file_position pos = info2.stop.get_position();
			//std::cout << (where + pos.column) << std::endl;
			////std::cout <<"("<< info2.stop <<")"<< " : " << " this line parse failed"<<std::endl;
			SSW_RETURN_LONG(-5);
		}
	}
	catch(std::exception &e)
	{
		SSW_RETURN_LONG(-6);
	}
	catch(...)
	{
		SSW_RETURN_LONG(-7);
	}

	SSW_RETURN_LONG(-8);
}

