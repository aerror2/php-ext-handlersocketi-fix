

#include <boost/spirit/core.hpp>

#include <boost/spirit/utility/confix.hpp>
#include <boost/spirit/utility/escape_char.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include "SimpleSQLWhere.h"
#include "php.h"


my_stack_t SQLParser::m_stack;

extern "C" int parse_simple_mysql_where(const char * where)
{
	
	try
	{

		SimpleSQLWhereGrammar g;	 
		SQLParser::setup();
		
	

    	iterator_t begin(where, where+strlen(where));
    	iterator_t end;


		parse_info < iterator_t > info2 = parse (begin, end, g);




		if (info2.full)
		{
			std::cout << "\nParse succeeded! "<< where <<"\n";
			
			for(stack_iter_t it= SQLParser::m_stack.begin(); it != SQLParser::m_stack.end(); it++)
			{
				Token *t = *it;

				if(t->getType()==token_criti)
				{
					std::cout << "criti: ";
					my_stack_t *list = (my_stack_t*) t->getData();
					int elemNum = 0;
					for(stack_iter_t ev = list->begin();ev!=list->end();ev++)
					{
						Token *pv = *ev;
						if(pv->getType()==token_identifer)
						{
							std::cout <<  ((std::string *)pv->getData())->c_str() << " ";
						}
						else if(pv->getType()==token_operator)
						{
							std::cout <<  ((std::string *)pv->getData())->c_str() << " ";
						}
						else if(pv->getType()==token_const)
						{
							std::cout <<  ((std::string *)pv->getData())->c_str() << " ";
						}
						else if(pv->getType()==token_values_array)
						{
							my_stack_t *pvls = (my_stack_t *)pv->getData();
							std::cout << " (";
							bool isFirst = true;
							for(stack_iter_t pvli = pvls->begin();pvli!=pvls->end();pvli++)
							{
								Token *pplv = *pvli;
								if(pplv->getType()==token_const)
								{
									if(isFirst)
									{
										isFirst=false;
										std::cout <<  ((std::string *)pplv->getData())->c_str() ;
									}
									else
									{
										std::cout << "," <<  ((std::string *)pplv->getData())->c_str() ;
									}
								}
								else
								{
									std::cout << "--wrong token type"<< ((int)pplv->getType()) << "---";
								}
							}


							std::cout << ") ";
						}

					}

					std::cout << "\n";
				}
				else if(t->getType()==token_limit)
				{
					std::cout << "limit: ";

					my_stack_t *pvls = (my_stack_t *)t->getData();
					bool isFirst = true;
					for(stack_iter_t pvli = pvls->begin();pvli!=pvls->end();pvli++)
					{
						Token *pplv = *pvli;
						if(pplv->getType()==token_const)
						{
							if(isFirst)
							{
								isFirst=false;
								std::cout <<  ((std::string *)pplv->getData())->c_str() ;
							}
							else
							{
								std::cout << "," <<  ((std::string *)pplv->getData())->c_str() ;
							}
						}
						else
						{
							std::cout << "--wrong token type"<< ((int)pplv->getType()) << "---";
						}
					}

					std::cout << "\n";
				}
				else
				{
					std::cout << "\nParse result should be criti or limit:" << where << "\n";
				}
			}

			SQLParser::cleanup();
			return 0;
		}
		else
		{
			SQLParser::cleanup();
			std::cout << "\nParse failed:" << where << "\n";
			file_position pos = info2.stop.get_position();
			std::cout << (where + pos.column) << std::endl;
			//std::cout <<"("<< info2.stop <<")"<< " : " << " this line parse failed"<<std::endl;
			return -1;
		}
	}
	catch(std::exception &e)
	{
		return -1;
	}
	catch(...)
	{
		return -2;
	}

	return 0;
}
/*
int main(int argc, char ** argv)
{
	
	if(argc <2)
		return -1;

	parse_simple_mysql_where(argv[1]);
	return 0;
}*/

