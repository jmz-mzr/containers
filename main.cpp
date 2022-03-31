/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmazoyer <jmazoyer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/31 14:37:37 by jmazoyer          #+#    #+#             */
/*   Updated: 2022/03/31 14:37:37 by jmazoyer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include "colors.hpp"
#include "tests.hpp"

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

void	printSuccess(const std::string& flags)
{
	std::cout << B_GREEN << "[OK]" << NO_COLOR
		<< " All tests ran successfully!";
	if (flags != "-v" && flags != "-a" && flags != "-s"
			&& flags != "-m" && flags != "-set")
		std::cout << "\nTo compare the \"ft::\" containers' speed with the"
			" \"std::\" ones, re-run these test with\nthe corresponding flag:"
			" -v (vector), -s (stack), -m (map), -set (set), or -a (all)";
	std::cout << std::endl;
}

int	main(int argc, char** argv)
{
	std::string		flags;

	if (argc > 1)
		flags = argv[1];
	printHeader("ENABLE_IF / IS_INTEGRAL", false);
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
	printHeader("VECTOR");
	vector__tests(flags == "-v" || flags == "-a");
	printHeader("STACK", false);
	stack__tests(flags == "-s" || flags == "-a");
	printHeader("MAP", false);
	map__tests(flags == "-m" || flags == "-a");
	printHeader("SET", false);
	set__tests(flags == "-set" || flags == "-a");
	printSuccess(flags);
	return (0);
}
