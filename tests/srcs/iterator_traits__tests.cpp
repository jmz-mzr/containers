/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   iterator_traits__tests.cpp                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmazoyer <jmazoyer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/31 14:37:37 by jmazoyer          #+#    #+#             */
/*   Updated: 2022/03/31 14:37:37 by jmazoyer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <vector>
#include <list>
#include <iterator>
#include "iterator_traits.hpp"

#ifndef STD
# define NMSP	ft
#else
# define NMSP	std
#endif

/******************************************************************************/
/*                            TEMPLATES / FUNCTIONS                           */
/******************************************************************************/

/*
** The right iterator_tag should be included in the iterator_traits in order
** for SFINAE to select the right helper function from the Overload Set
*/

template <typename RandomAccessIt>
typename NMSP::iterator_traits<RandomAccessIt>::difference_type
	myDistance(RandomAccessIt first, RandomAccessIt last,
				std::random_access_iterator_tag)
{
	return (last - first);
}
template <typename InputIt>
typename NMSP::iterator_traits<InputIt>::difference_type
	myDistance(InputIt first, InputIt last, std::input_iterator_tag)
{
	typename NMSP::iterator_traits<InputIt>::difference_type	n = 0;

	for ( ; first != last; ++n, ++first)
		;
	return (n);
}
template <typename Iter>
typename NMSP::iterator_traits<Iter>::difference_type
	myDistance(Iter first, Iter last)
{
	return (myDistance(first, last,
				typename NMSP::iterator_traits<Iter>::iterator_category()));
}

/*
** The "const T*" specialization should be implemented for the "::value_type"
** member not be const qualified
*/

template <typename BidirIt>
void	myReverse(BidirIt first, BidirIt last)
{
	typename NMSP::iterator_traits<BidirIt>::difference_type	n = 0;
	typename NMSP::iterator_traits<BidirIt>::value_type			tmp;

	if (first != last) {
		n = myDistance(first, last) / 2;
		--last;
	}
	for ( ; n != 0; --n, ++first, --last) {
		tmp = *first;
		*first = *last;
		*last = tmp;
	}
}
template <typename T>
void	myReverse(const T* first, const T* last)
{
	typename NMSP::iterator_traits<const T*>::value_type	tmp = *first;

	if (first != last)
		tmp = *(last - 1);
}

template <typename BidirIt>
void	print(BidirIt first, BidirIt last)
{
	typename NMSP::iterator_traits<BidirIt>::value_type	tmp = *first;

	if (first != last) {
		tmp = *--last;
		++last;
	}
	while (first != last)
		std::cout << *first++;
	std::cout << std::endl;
}
template <typename T>
void	print(const T* first, const T* last)
{
	typename NMSP::iterator_traits<const T*>::value_type	tmp = *first;

	if (first != last)
		tmp = *(last - 1);
	while (first != last)
		std::cout << *first++;
	std::cout << std::endl;
}

/******************************************************************************/
/*                                   TESTS                                    */
/******************************************************************************/

void	iterator_traits__tests(void)
{
	int						array1[5] = {0, 1, 2, 3, 4};
	std::vector<int>		vec(array1, array1 + 5);
	const char* const		array2[4] = {"Hello", ", ", "world", "!"};
	std::list<std::string>	lst(array2, array2 + 4);

	std::cout << "array1:           ";
	print((const int*)array1, (const int*)array1 + 5);
	std::cout << "myReverse array1: ";
	myReverse(array1, array1 + 5);
	print((const int*)array1, (const int*)array1 + 5); std::cout << std::endl;

	std::cout << "vec:           ";
	print(vec.begin(), vec.end());
	std::cout << "myReverse vec: ";
	myReverse(vec.begin(), vec.end());
	print(vec.begin(), vec.end()); std::cout << std::endl;

	std::cout << "const array2:     ";
	print(array2, array2 + 4);
	std::cout << "myReverse array2: ";
	myReverse(array2, array2 + 4);
	print(array2, array2 + 4); std::cout << std::endl;

	std::cout << "lst:           ";
	print(lst.begin(), lst.end());
	std::cout << "myReverse lst: ";
	myReverse(lst.begin(), lst.end());
	print(lst.begin(), lst.end());
}
