#include <iostream>
//#include <stack>
#include "hackStdStack.hpp"
#include <vector>
#include <deque>
#include <list>
//#include "stack.hpp"
#include "hackFtStack.hpp"
#include "enable_if.hpp"
#include "is_integral.hpp"
#include "iterator_traits.hpp"
#include "colors.hpp"

#ifndef STD
# define NMSP	ft
#else
# define NMSP	std
#endif

#ifdef __APPLE__
# define SPEED_STRLEN		3000
# define SPEED_VEC_SIZE		1500
#elif defined(__linux__)
# define SPEED_STRLEN		5000
# define SPEED_VEC_SIZE		2000
#endif

/******************************************************************************/
/*                            TEMPLATES / FUNCTIONS                           */
/******************************************************************************/

static std::string	success(const char* str)
{
	std::string		output("  All ");

	output += str;
	output += " work correctly  ";
	output += B_GREEN;
	output += "✔︎";
	output += NO_COLOR;
	output += "\n";
	return (output);
}

/*
** Self-explanatory: it prints a stack's content
*/

template <typename T, class Container>
static void	print(const NMSP::stack<T, Container>& stack)
{
	NMSP::stack<T, Container>	tmp(stack);

	std::cout << " (top)";
	while (!tmp.empty()) {
		std::cout << " " << tmp.top();
		tmp.pop();
	}
	std::cout << " (bottom)" << std::endl;
}

/*
** Implement "is_const/is_const_pointer<T>" to check wether a type/pointer
** is indeed related to a const type
*/

template <typename T>
struct	is_const: public ft::false_type {
};

template <typename T>
struct	is_const<const T>: public ft::true_type {
};

template <typename T>
struct	is_const_pointer: public ft::false_type {
};

template <typename T>
struct	is_const_pointer<const T*>: public ft::true_type {
};

/*
** Create a type that cannot be default constructed to test if the stack
** can still be empty created
*/

class	priv {
	priv(void);
};

/*
** Build a heavy class to test the performance
** (the defined values depend on your machine: here my Linux VM cannot
** handle the same tests as my native MacOS)
*/

class	HeavyStack {
public:
	HeavyStack(void): str(SPEED_STRLEN, '-'), strVec(SPEED_VEC_SIZE, str) { }
	HeavyStack(char c): str(SPEED_STRLEN, c), strVec(SPEED_VEC_SIZE, str) { }
private:
	std::string					str;
	NMSP::vector<std::string>	strVec;
};

/******************************************************************************/
/*                                   TESTS                                    */
/******************************************************************************/

static void	typedef__tests(void)
{
	typedef NMSP::stack<int, NMSP::vector<int> >				stack;
	typedef ft::iterator_traits<stack::container_type
									::const_iterator>			it_traits;
	typedef ft::iterator_traits<stack::container_type
									::const_reverse_iterator>	rev_it_traits;

	int													x = 42;
	NMSP::vector<int>									vec1(1, 1);
	ft::enable_if<true, stack::container_type>::type	vec2(vec1);
	ft::enable_if<true, stack::value_type>::type		y = x;
	ft::enable_if<true, stack::size_type>::type			size = vec2.size();

	if (!is_const_pointer<it_traits::pointer>::value
			|| !is_const_pointer<rev_it_traits::pointer>::value)
		std::cout << "Error: stack const_iterators aren't const!\n"
			<< std::endl;

	(void)x; (void)vec1; (void)vec2; (void)y; (void)size;
}

static void	constructors_destructors__tests(void)
{
	NMSP::stack<priv>										stack00;
	NMSP::stack<int>										stack;
	NMSP::stack<std::string>								stack0;
	NMSP::vector<std::string>								vec0(3, "Hello!");
	std::vector<std::string>								vec1(5, "Hi!");
	NMSP::stack<std::string, NMSP::vector<std::string> >	stack1(vec0);
	NMSP::stack<std::string, std::vector<std::string> >		stack2(vec1);
	NMSP::stack<std::string, NMSP::vector<std::string> >	stack3(stack1);
	std::list<int>											list(2, 21);
	NMSP::stack<int, std::list<int> >						stack4(list);
	std::deque<float>										deque(7, 42.21f);
	NMSP::stack<float, std::deque<float> >					stack5(deque);
//	NMSP::stack<std::string, NMSP::vector<std::string> >	stack6 = vec0;
//	explicit constructor shouldn't allow this to compile!

	(void)stack; (void)stack0;
	std::cout << "stack1 ="; print(stack1);
	std::cout << "stack2 ="; print(stack2);
	std::cout << "stack3 ="; print(stack3);
	std::cout << "stack4 ="; print(stack4);
	std::cout << "stack5 ="; print(stack5); std::cout << std::endl;
}

static void	member_operators__tests(void)
{
	std::list<int>							list1(2, 21);
	NMSP::stack<int, std::list<int> >		stack1(list1);
	std::list<int>							list2(1, 42);
	NMSP::stack<int, std::list<int> >		stack2(list2);

	std::cout << "stack1 ="; print(stack1);
	std::cout << "stack2 ="; print(stack2);
	std::cout << "stack1 = stack2;" << std::endl;
	stack1 = stack2;
	std::cout << "stack1 ="; print(stack1); std::cout << std::endl;
}

namespace	std {
	std::stack<int>	stackI;
}
namespace	ft
{
	ft::stack<int>	stackI;
}

static void	relational_operators__tests(void)
{
	NMSP::vector<int>										vec0;
	const NMSP::stack<int, NMSP::vector<int> >				stack0(vec0);
	NMSP::vector<int>										vec1(2, 1);
	NMSP::stack<int, NMSP::vector<int> >					stack1(vec1);
	NMSP::vector<int>										vec2(4, 2);
	const NMSP::stack<int, NMSP::vector<int> >				stack2(vec2);
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
	const NMSP::stack<int>									stack0;
	NMSP::vector<std::string>								vec(3, "Hello!");
	NMSP::stack<std::string, NMSP::vector<std::string> >	stack1(vec);
	NMSP::stack<std::string, NMSP::vector<std::string> >	stack2(stack1);
	std::list<int>											list(2, 21);
	const NMSP::stack<int, std::list<int> >					stack3(list);
	const int&												x = stack3.top();
	std::deque<float>										deque(1, 42.21f);
	NMSP::stack<float, std::deque<float> >					stack4(deque);

	std::cout << "stack0 (const) ="; print(stack0);
	std::cout << "stack1 ="; print(stack1);
	std::cout << "stack2 ="; print(stack2);
	std::cout << "stack3 (const) ="; print(stack3);
	std::cout << "stack4 ="; print(stack4); std::cout << std::endl;

	std::cout << std::boolalpha;
	std::cout << "stack0.empty() = " << stack0.empty() << std::endl;
	std::cout << "stack1.empty() = " << stack1.empty() << std::endl;
	std::cout << std::noboolalpha;
	std::cout << "stack0.size() = " << stack0.size() << std::endl;
	std::cout << "stack2.size() = " << stack2.size() << std::endl;
	std::cout << "stack2.top() = " << stack2.top() << std::endl;
	std::cout << "x = stack3.top() = " << x << "\n" << std::endl;

	std::cout << "stack2.push(\"Hello, world! -\");" << std::endl;
	stack2.push("Hello, world! -");
	std::cout << "stack2 ="; print(stack2);
	std::cout << "stack2.pop();" << std::endl;
	stack2.pop();
	std::cout << "stack2 ="; print(stack2); std::cout << std::endl;
	std::cout << "stack4 ="; print(stack4);
	std::cout << "stack4.pop();" << std::endl;
	stack4.pop();
	std::cout << "stack4.push(0.0f);" << std::endl;
	stack4.push(0.0f);
	std::cout << "stack4 ="; print(stack4); std::cout << std::endl;
}

static void	speed__tests(void)
{
	NMSP::vector<HeavyStack>							vec(100);
	NMSP::stack<HeavyStack, NMSP::vector<HeavyStack> >	stack(vec);
	NMSP::stack<HeavyStack>::size_type					size;
	HeavyStack											tmp;

	while (!stack.empty()) {
		size = stack.size();
		(void)size;
		tmp = stack.top();
		for (int i = 0; i != 41; ++i)
			stack.push(tmp);
		for (int i = 0; i != 42; ++i)
			stack.pop();
	}
}

void	stack__tests(bool testSpeed)
{
	typedef__tests();
	std::cout << success("typedefs") << std::endl;
	constructors_destructors__tests();
	std::cout << success("constructors & destructors") << std::endl;
	member_operators__tests();
	std::cout << success("member operators") << std::endl;
	relational_operators__tests();
	std::cout << success("relational operators") << std::endl;
	member_functions__tests();
	std::cout << success("member functions") << std::endl;
	if (testSpeed) {
		speed__tests();
		std::cout << success("speed tests") << std::endl;
	}
}
