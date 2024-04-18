#include <iostream>
#include <iomanip>
#include <stack>
#include "stack.hpp"
#include "hackStdStack.hpp"
#include "hackFtStack.hpp"
#include "colors.hpp"

void	printHeader(const std::string& str, bool addNewline = true)
{
	int				nbStars = 78 - (str.size() % 2);
	std::string		stars(nbStars, '*');
	int				nbSpaces = nbStars - 2 - str.size();
	std::string		spaces(nbSpaces / 2, ' ');

	if (addNewline)
		std::cout << "\n";
	std::cout << B_CYAN << stars << "\n*"
		<< NO_COLOR << spaces << B_GREEN << str << NO_COLOR << spaces
		<< B_CYAN << "*\n" << stars << NO_COLOR << "\n" << std::endl;
}

template <typename T, class Container>
static void	print(const std::stack<T, Container>& stack)
{
	std::stack<T, Container>	tmp(stack);

	std::cout << " (top)";
	if (!tmp.empty())
		std::cout << " ";
	while (!tmp.empty()) {
		std::cout << std::fixed << std::setprecision(1) << tmp.top();
		tmp.pop();
		if (!tmp.empty())
			std::cout << " -> ";
	}
	std::cout << " (bottom)" << std::endl;
}
template <typename T, class Container>
static void	print(const ft::stack<T, Container>& stack)
{
	ft::stack<T, Container>	tmp(stack);

	std::cout << " (top)";
	if (!tmp.empty())
		std::cout << " ";
	while (!tmp.empty()) {
		std::cout << std::fixed << std::setprecision(1) << tmp.top();
		tmp.pop();
		if (!tmp.empty())
			std::cout << " -> ";
	}
	std::cout << " (bottom)" << std::endl;
}

namespace	std {
	std::stack<int>	stackI;
}
namespace	ft
{
	ft::stack<int>	stackI;
}

int	main(int argc, char** argv)
{
    std::stack<double>	stack0;
    ft::stack<double>	stack1;

	(void)argc;
	(void)argv;
	printHeader("LET'S HACK STACK!", false);

	stack0.push(42); stack0.push(77); stack0.push(11);
	stack1.push(18); stack1.push(50); stack1.push(34);
	std::cout << B_WHITE << "Here is the content of 'std::stack0' and 'ft::stack1',"
		" containing <double>:\n\n" << NO_COLOR;
	std::cout << "    stack0: "; print(stack0);
	std::cout << "    stack1: "; print(stack1); std::cout << std::endl;

	ft::stackI.push(52); ft::stackI.push(95);
	std::stackI.push(52); std::stackI.push(95);
	std::cout << B_WHITE
		<< "We have declared in both namespaces 'stackI' containing <int>, here it is:\n\n" << NO_COLOR;
	std::cout << "    std::stackI: "; print(std::stackI);
	std::cout << "     ft::stackI: "; print(ft::stackI); std::cout << std::endl;

	std::cout << B_WHITE << "Using the broad friendship (wrongly) given to all"
		" instantiations of\n the operator templates (in this case the '=='),"
		" and its specialization\n introduced in the 'hackStack.hpp' header,"
		" although they are not of\n the same type (<double> vs <int>),"
		" when we compare the stacks of <double>,\n the broad friendship given to the"
		" '==' operator grants us access to the\n private member 'c' of stackI containing <int>,"
		" and we can corrupt it!\n" << NO_COLOR << std::endl;
	
	std::cout << std::boolalpha;
    std::cout << "    stack0 == stack0: " << (stack0 == stack0) << std::endl;
    std::cout << "    stack1 == stack1: " << (stack1 == stack1) << std::endl;
	std::cout << std::noboolalpha << std::endl;

	std::cout << B_WHITE << "Now, here is the content of 'stackI', where the"
		" previous (seemingly harmless)\n comparison operator called between"
		" two stacks of <double> has injected a\n value in the middle of"
		" our global stack of <int>!\n\n" << NO_COLOR;
	std::cout << "    std::stackI: "; print(std::stackI);
	std::cout << "     ft::stackI: "; print(ft::stackI); std::cout << std::endl;

	std::cout << B_WHITE << "This vulnerability might be hard to exploit, however we know"
		" that people get\n smart when trying to hack stuff, so...\n\n"
		"The funny thing is that, when we go back in the commit history of"
		" LIBSTDCPP,\n the proper way to declare this friendship"
		" was there! And then someone\n deleted it -"
		" but that's another story ;)\n" << NO_COLOR << std::endl;

	printHeader("THX FOR WATCHING!", false);
    return (0);
}
