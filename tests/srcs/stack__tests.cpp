#include <iostream>
//#include <stack>
#include "hackStdStack.hpp"
#include <vector>
#include <deque>
#include <list>
//#include "stack.hpp"
#include "hackFtStack.hpp"

#ifndef STD
# define NMSP	ft
#else
# define NMSP	std
#endif

/******************************************************************************/
/*                            TEMPLATES / FUNCTIONS                           */
/******************************************************************************/

template <typename T>
static void	print(const NMSP::stack<T, NMSP::vector<T> >& stack)
{
	NMSP::stack<T, NMSP::vector<T> >	tmp(stack);

	std::cout << " (top)";
	while (!tmp.empty()) {
		std::cout << " " << tmp.top();
		tmp.pop();
	}
	std::cout << " (bottom)" << std::endl;
}

/******************************************************************************/
/*                                   TESTS                                    */
/******************************************************************************/

static void	typedef__tests(void)
{
}

static void	constructors_destructors__tests(void)
{
}

static void	member_operators__tests(void)
{
}

static void	relational_operators__tests(void)
{
	NMSP::vector<int>										vec0;
	NMSP::stack<int, NMSP::vector<int> >					stack0(vec0);
	NMSP::vector<int>										vec1(2, 1);
	NMSP::stack<int, NMSP::vector<int> >					stack1(vec1);
	NMSP::vector<int>										vec2(4, 2);
	NMSP::stack<int, NMSP::vector<int> >					stack2(vec2);
	NMSP::vector<std::string>								vec3(1, "Hello!");
	NMSP::stack<std::string, NMSP::vector<std::string> >	stack3(vec3);
	NMSP::stack<NMSP::uniq, NMSP::vector<NMSP::uniq> >		stack4;

	std::cout << "stack0 ="; print(stack0);
	std::cout << "stack1 ="; print(stack1);
	std::cout << "stack2 ="; print(stack2);
	std::cout << "stack3 ="; print(stack3); std::cout << std::endl;

	std::cout << std::boolalpha;
	std::cout << "stack0 == stack1: " << (stack0 == stack1) << std::endl;
	std::cout << "stack1 != stack1: " << (stack1 != stack1) << std::endl;
	std::cout << "stack1 != stack2: " << (stack1 != stack2) << std::endl;
	std::cout << "stack2 < stack1: " << (stack2 < stack1) << std::endl;
	std::cout << "stack1 < stack2: " << (stack1 < stack2) << std::endl;
	std::cout << "stack2 <= stack2: " << (stack2 <= stack2) << std::endl;
	std::cout << "stack1 > stack2: " << (stack1 > stack2) << std::endl;
	std::cout << "stack1 > stack0: " << (stack1 > stack0) << std::endl;
	std::cout << "stack3 >= stack3: " << (stack3 <= stack3) << std::endl;
	std::cout << "stack4 == stack4: " << (stack4 == stack4) << std::endl;
	std::cout << std::noboolalpha << std::endl;
}

static void	member_functions__tests(void)
{
}

static void	speed__tests(void)
{
}

void	stack__tests(bool testSpeed)
{
	typedef__tests();
	std::cout << "   All typedefs are correctly defined\n" << std::endl;
	constructors_destructors__tests();
	std::cout << "   All constructors & destructors work correctly\n" << std::endl;
	member_operators__tests();
	std::cout << "   All member operators work correctly\n" << std::endl;
	relational_operators__tests();
	std::cout << "   All relational operators work correctly\n" << std::endl;
	member_functions__tests();
	std::cout << "   All member functions work correctly\n" << std::endl;
	if (testSpeed) {
		speed__tests();
		std::cout << "   Speed test is done\n" << std::endl;
	}
}
