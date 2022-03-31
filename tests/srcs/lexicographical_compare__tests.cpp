/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexicographical_compare__tests.cpp                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmazoyer <jmazoyer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/31 14:37:37 by jmazoyer          #+#    #+#             */
/*   Updated: 2022/03/31 14:37:37 by jmazoyer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <list>
#include <algorithm>
#include "lexicographical_compare.hpp"

#ifndef STD
# define NMSP	ft
#else
# define NMSP	std
#endif

/******************************************************************************/
/*                            TEMPLATES / FUNCTIONS                           */
/******************************************************************************/

/*
** The lexicographical_compare function should be able to use custom compare
** functions, and however weird they are it should not be tricked
*/

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

/******************************************************************************/
/*                                   TESTS                                    */
/******************************************************************************/

void	lexicographical_compare__tests(void)
{
	std::string		s1 = "hello";
	std::string		s2 = "HELLOOOO";
	long			array[10] = {127, 128, 129, 130};
	std::list<int>	list(array, array + 4);

	std::cout << std::boolalpha;
	std::cout << "array<long>[10] =";
	for (int i = 0; i != 4; ++i)
		std::cout << " " << array[i];
	std::cout << "\nlist<int>       =";
	for (std::list<int>::iterator it = list.begin(); it != list.end(); ++it)
		std::cout << " " << *it;
	std::cout << "\n   lex_cmp(array, array + 4, list.begin(), list.end()): "
		<< NMSP::lexicographical_compare(array, array + 4,
				list.begin(), list.end());
	std::cout << "\n   lex_cmp(array, array + 3, list.begin(), list.end()): "
		<< NMSP::lexicographical_compare(array, array + 3,
				list.begin(), list.end()) << std::endl << std::endl;

	std::cout << "lex_cmp(s1.begin(), s1.begin(), s2.end(), s2.end()): "
		<< NMSP::lexicographical_compare(s1.begin(), s1.begin(),
				s2.end(), s2.end()) << std::endl;
/*
	// Will crash on libstdc++
	std::cout << "lex_cmp(s1.end() + 10, s1.end(), s2.end(), s2.end()): "
		<< NMSP::lexicographical_compare(s1.end() + 10, s1.end(),
				s2.end(), s2.end()) << std::endl;
	std::cout << "lex_cmp(s1.end(), s1.end(), s2.end() + 10, s2.end()): "
		<< NMSP::lexicographical_compare(s1.end(), s1.end(),
				s2.end() + 10, s2.end()) << std::endl << std::endl;
*/
	std::cout << "lex_cmp(s1.begin(), s1.end(), s2.begin(), s2.end()): "
		<< NMSP::lexicographical_compare(s1.begin(), s1.end(),
				s2.begin(), s2.end()) << std::endl;
	std::cout << "lex_cmp(s1.begin(), s1.end(), s2.begin(), s2.end(), cmp1): "
		<< NMSP::lexicographical_compare(s1.begin(), s1.end(),
				s2.begin(), s2.end(), cmp1) << std::endl;
	std::cout << "lex_cmp(s1.begin(), s1.end(), s2.begin(), s2.end(), cmp2): "
		<< NMSP::lexicographical_compare(s1.begin(), s1.end(),
				s2.begin(), s2.end(), cmp2) << std::endl;
	std::cout << std::noboolalpha;
}
