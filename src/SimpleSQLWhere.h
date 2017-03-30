
/**
@author dany
*/
#ifndef SIMPLE_SQL_WHERE_H
#define SIMPLE_SQL_WHERE_H


#include <iostream>
#include <boost/spirit/core.hpp>
#include <boost/spirit/iterator/position_iterator.hpp>
#include <boost/spirit/utility/confix.hpp>
#include <boost/spirit/utility/escape_char.hpp>
#include <list>
#include <vector>


using namespace boost::spirit;

typedef position_iterator<char const*> iterator_t;

typedef scanner < iterator_t > scanner_t;
typedef rule < scanner_t > rule_t;

typedef std::list<class Token*>			my_stack_t;
typedef my_stack_t::iterator			stack_iter_t;


enum token_type
{
	token_comment = 0,
	token_identifer = 1,
	token_const = 2,
	token_operator = 3,
	token_values_array = 4,
	token_criti = 5,
	token_limit  = 6,
};


class Token
{
public:
	Token(void *data,int type)
	{
		m_type=type;
		m_data=data;
	}
	~Token()
	{
		if(this->getType()==token_values_array || this->getType()==token_limit || this->getType()==token_criti)
		{
			my_stack_t *pst = (my_stack_t *)this->getData();
			for(stack_iter_t ist = pst->begin(); ist!=pst->end();ist++)
			{
				Token *tp = *ist;
				delete tp;
			}
			delete pst;
		}
		else
		{
			std::string *ps = (std::string*)this->getData();
			delete ps;
		}

	}
public:
	inline int   getType(){return m_type;}
	inline void  setType(int t){m_type=t;}
	inline void* getData(){return m_data;}
	inline void  setData(void *d){m_data=d;}
private:
	int m_type;
	void* m_data;
};



class SQLParser
{
public:
	static my_stack_t  m_stack;

	static void setup()
	{
		cleanup();
	}


	static void cleanup()
	{
		for(stack_iter_t it=m_stack.begin(); it!=m_stack.end();it++)
		{
			Token *t = *it;
			delete t;
		}
		m_stack.clear();
	}

	static void on_identifer(iterator_t first, iterator_t const &last)
	{

		std::string *s = new std::string(first,last);
		m_stack.push_front(new Token(s,token_identifer));
		//std::cout<<"identifer:"<< (*s) <<std::endl;
	}

	static void on_value(iterator_t first, iterator_t const &last)
	{
		std::string *s = new std::string(first,last);
		m_stack.push_front(new Token(s,token_const));
		//std::cout<<"value:"<<  (*s)<<std::endl;
	}

	static void on_array(iterator_t first, iterator_t const &last)
	{
		my_stack_t *list = new my_stack_t();
		while(true)
		{
			Token *t = m_stack.front();
			if(t->getType()!=token_const)
			{
				break;
			}

			m_stack.pop_front();
			list->push_front(t);
		}

		Token * ta = new Token(list,token_values_array);

		m_stack.push_front(ta);

	}

	static void on_operator(iterator_t first, iterator_t const &last)
	{
		std::string *s = new std::string(first,last);
		m_stack.push_front(new Token(s,token_operator));
		//std::cout<<"operator:"<<  (*s)<<std::endl;
	}

	static void on_criti(iterator_t first, iterator_t const &last)
	{
		
		my_stack_t *list = new my_stack_t();
		for(int i=0;i<3;i++)
		{
			Token *t = m_stack.front();
			m_stack.pop_front();
			list->push_front(t);
		}

		Token * tc = new Token(list,token_criti);
		m_stack.push_front(tc);

	}

	static void on_limit(iterator_t first, iterator_t const &last)
	{
		my_stack_t *list = new my_stack_t();
		for(int i=0;i<2;i++)
		{
			Token *t = m_stack.front();
			if(t->getType()!=token_const)
			{
				break;
			}

			m_stack.pop_front();
			list->push_front(t);
		}
		Token * tl = new Token(list,token_limit);
		m_stack.push_front(tl);
	}

};


struct SimpleSQLWhereGrammar: public grammar<SimpleSQLWhereGrammar>
{
	template <typename ScannerT>
	struct definition
	{
		definition(SimpleSQLWhereGrammar const& self)
		{
			first =
				( 
			   	SQL_SIMPLE_WHERE=*space_p>>sql_criti[&SQLParser::on_criti]
			   					>>*(+space_p>>and_operator_>>+space_p>>sql_criti[&SQLParser::on_criti]) >> !(+space_p>>sql_limit[&SQLParser::on_limit])>> (*space_p),

				string_value_ = confix_p('"', *c_escape_ch_p, '"') | confix_p('\'', *c_escape_ch_p, '\''),
				identifer_ =   ((alpha_p | '_' ) >> *(alpha_p| alnum_p | '_') ) ,
				assign_operate_ = ch_p('='),
				lt_operate_ = ch_p('<')>>!ch_p('='),
				gt_operate_ = ch_p('>')>>!ch_p('='),
				in_operate_=  (ch_p('I')|ch_p('i'))>>(ch_p('n')|ch_p('N')),
				entry_seperator_ =ch_p(','),
				array_d_start_ = ch_p('('),
				array_d_end_ = ch_p(')'),
				and_operator_= (ch_p('A')|ch_p('a'))>>(ch_p('n')|ch_p('N'))>>(ch_p('d')|ch_p('D')),
				hexvalue = ( ( str_p("0x") | str_p("0X")  )>>hex_p ),
				intvalue = hexvalue | int_p,
				number_ =  hexvalue |real_p,
				const_value= (number_|string_value_)[&SQLParser::on_value],
				simple_operate_ = (assign_operate_|lt_operate_|gt_operate_),
				array_values = array_d_start_>>*space_p>>const_value>>*(*space_p>> entry_seperator_>>*space_p>>const_value)>>*space_p>> array_d_end_,
				columnName = (identifer_[&SQLParser::on_identifer])|('`'>>identifer_[&SQLParser::on_identifer]>>'`'),
				sql_criti = columnName>>(
					(*space_p>>simple_operate_[&SQLParser::on_operator]>>*space_p>>const_value)|
					(+space_p>>in_operate_[&SQLParser::on_operator]>>*space_p>>array_values[&SQLParser::on_array])
					),

				sql_limit = (str_p("limit") | str_p("LIMIT")  )>>+space_p>>intvalue[&SQLParser::on_value]
						>>!(*space_p>>entry_seperator_>>*space_p>>intvalue[&SQLParser::on_value]) 
				);

		}


		subrule<0> SQL_SIMPLE_WHERE ;
		

		subrule<2>		string_value_;

		subrule<4>		identifer_;

		subrule<6>		assign_operate_;
		subrule<7>		lt_operate_ ;

		subrule<9>		gt_operate_ ;

		subrule<11>		in_operate_;
		subrule<12>		entry_seperator_ ;
		subrule<13>		array_d_start_ ;
		subrule<14>		array_d_end_ ;
		subrule<15>		and_operator_;
		subrule<16>		number_ ;

		subrule<18>		const_value;

		subrule<24>		simple_operate_ ;
		subrule<26>		array_values ;
		subrule<27>		columnName;
		subrule<30>		sql_criti ;
		subrule<31>     sql_limit;

		subrule<32>      hexvalue;
		subrule<33>		 intvalue;
		rule<ScannerT> first;
		rule<ScannerT> const&
			start() const { 
				return first; 
		}
	};
};

#endif
