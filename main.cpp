#include <iostream>
#include "tests.hpp"

#define B_CYAN		"\033[0;96m"
#define B_GREEN		"\033[0;92m"
#define NO_COLOR	"\033[m"

void	printHeader(const std::string& str)
{
	int				nbStars = 78 - (str.size() % 2);
	std::string		stars(nbStars, '*');
	int				nbSpaces = nbStars - 2 - str.size();
	std::string		spaces(nbSpaces / 2, ' ');
	static bool		addNewline = false;

	if (addNewline)
		std::cout << "\n";
	else
		addNewline = true;
	std::cout << B_CYAN << stars << "\n*"
		<< NO_COLOR << spaces << B_GREEN << str << NO_COLOR << spaces
		<< B_CYAN << "*\n" << stars << NO_COLOR << "\n" << std::endl;
}

int	main(void)
{
	printHeader("ENABLE_IF / IS_INTEGRAL");
	enable_if__is_integral__tests();
	printHeader("EQUAL");
	equal__tests();
	printHeader("LEXICOGRAPHICAL_COMPARE");
	lexicographical_compare__tests();
	printHeader("PAIR");
	pair__tests();
	printHeader("MAKE_PAIR");
	make_pair__tests();
	printHeader("ITERATOR_TRAITS");
	iterator_traits__tests();
	printHeader("REVERSE_ITERATOR");
	reverse_iterator__tests();
	return (0);
}
