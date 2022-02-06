#include <iostream>
#include <list>
#include "lexicographical_compare.hpp"

#define HEADER1		"*****************************************************\n*"
#define HEADER2		"*\n*****************************************************\n"
#define SPACE10		"          "

bool	cmp1(char c1, char c2)
{
	return (std::tolower(c1) < c2);
}

bool	cmp2(char c1, char c2)
{
	if (c1 != 'O')
		c1 = std::tolower(c1);
	return (c1 < c2);
}

void	lexicographical_compare__tests(void)
{
	std::string		s1 = "hello";
	std::string		s2 = "HELLOOOO";
	long			array[10] = {127, 128, 129, 130};
	std::list<int>	list(array, array + 4);

	std::cout << HEADER1 << SPACE10 << "    LEXICOGRAPHICAL_COMPARE    "
		<< SPACE10 << HEADER2 << std::endl;

	std::cout << std::boolalpha;
	std::cout << "array<long>[10] =";
	for (int i = 0; i != 4; ++i)
		std::cout << " " << array[i];
	std::cout << "\nlist<int>       =";
	for (std::list<int>::iterator it = list.begin(); it != list.end(); ++it)
		std::cout << " " << *it;
	std::cout << "\n   lex_cmp(array, array + 4, list.begin(), list.end()): "
		<< ft::lexicographical_compare(array, array + 4,
				list.begin(), list.end());
	std::cout << "\n   lex_cmp(array, array + 3, list.begin(), list.end()): "
		<< ft::lexicographical_compare(array, array + 3,
				list.begin(), list.end()) << std::endl << std::endl;

	std::cout << "lex_cmp(s1.begin(), s1.begin(), s2.end(), s2.end()): "
		<< ft::lexicographical_compare(s1.begin(), s1.begin(),
				s2.end(), s2.end()) << std::endl;
	std::cout << "lex_cmp(s1.end() + 10, s1.end(), s2.end(), s2.end()): "
		<< ft::lexicographical_compare(s1.end() + 10, s1.end(),
				s2.end(), s2.end()) << std::endl;
	std::cout << "lex_cmp(s1.end(), s1.end(), s2.end() + 10, s2.end()): "
		<< ft::lexicographical_compare(s1.end(), s1.end(),
				s2.end() + 10, s2.end()) << std::endl << std::endl;

	std::cout << "lex_cmp(s1.begin(), s1.end(), s2.begin(), s2.end()): "
		<< ft::lexicographical_compare(s1.begin(), s1.end(),
				s2.begin(), s2.end()) << std::endl;
	std::cout << "lex_cmp(s1.begin(), s1.end(), s2.begin(), s2.end(), cmp1): "
		<< ft::lexicographical_compare(s1.begin(), s1.end(),
				s2.begin(), s2.end(), cmp1) << std::endl;
	std::cout << "lex_cmp(s1.begin(), s1.end(), s2.begin(), s2.end(), cmp2): "
		<< ft::lexicographical_compare(s1.begin(), s1.end(),
				s2.begin(), s2.end(), cmp2) << std::endl;
	std::cout << std::noboolalpha;
}
