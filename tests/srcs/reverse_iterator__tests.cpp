#include <iostream>
#include <vector>
#include <list>
#include <string>
#include <iterator>
#include "reverse_iterator.hpp"

#ifndef STD
# define NMSP	ft
#else
# define NMSP	std
#endif

/******************************************************************************/
/*                                   TESTS                                    */
/******************************************************************************/

void	reverse_iterator__tests(void)
{
	int						array[5] = {0, 1, 2, 3, 4};
	std::vector<int>		vec(array, array + 5);
	const char*				arr[3] = {"Hello", ", ", "world"};
	std::list<std::string>	lst(arr, arr + 3);
	NMSP::reverse_iterator<int*>
							revIt;
	NMSP::reverse_iterator<const int*>
							revIt2(revIt);
	NMSP::reverse_iterator<std::vector<int>::iterator>
							revItVec1(vec.end());
	NMSP::reverse_iterator<std::vector<int>::iterator>
							revItVec2(vec.end());
	NMSP::reverse_iterator<std::list<std::string>::iterator>
							revItLst1(lst.end());
	NMSP::reverse_iterator<std::list<std::string>::iterator>
							revItLst2(lst.begin());

	revIt2 = NMSP::reverse_iterator<int*>();
	revItVec2 = NMSP::reverse_iterator<std::vector<int>::iterator>(vec.begin());
	std::cout << "Constructors and assignment operators work as expected\n"
		<< std::endl;

	if (revItVec2.base() == vec.begin() && revItVec1.base() == vec.end())
		std::cout << "Base() function works as expected" << std::endl;
	if ((*revItLst1).size() == lst.back().size()
			|| revItLst1->size() != lst.back().size())
		std::cout << "Operator* and Operator-> work as expected" << std::endl;
	if (revItVec1[2] == vec[2])
		std::cout << "Operator[] work as expected\n" << std::endl;

	revItVec1++; revItVec1--;
	revItVec1 = ++revItVec1;
	revItVec1 = --revItVec1;
	if (revItVec1.base() == vec.end())
		std::cout << "Pre/Post Operator++/-- work as expected" << std::endl;
	revItVec1 += 5;
	revItVec1 = revItVec1 - 1;
	revItVec1 = revItVec1 + 2;
	revItVec1 -= 1;
	if (revItVec1.base() == vec.begin())
		std::cout << "Operator+/-/+=/-= all work as expected\n" << std::endl;

	revItVec1 = NMSP::reverse_iterator<std::vector<int>::iterator>(vec.end());
	revItVec1 = (5 + revItVec1);
	if (revItVec1.base() == vec.begin())
		std::cout << "Operator+(n, revIt) work as expected" << std::endl;
	revItVec1 = NMSP::reverse_iterator<std::vector<int>::iterator>(vec.end());
	if (revItVec1 - revItVec2 == -5 && revItVec2 - revItVec1 == 5)
		std::cout << "Operator-(revIt1, revIt2) work as expected\n"
			<< std::endl;

	std::cout << std::boolalpha;
	std::cout << "lstRbegin == lstRend: " << (revItLst1 == revItLst2)
		<< "\nlstRbegin != lstRend: " << (revItLst1 != revItLst2)
		<< "\nvecRbegin < vecRend: " << (revItVec1 < revItVec2)
		<< "\nvecRbegin <= vecRend: " << (revItVec1 <= revItVec2)
		<< "\nvecRbegin <= vecRbegin: " << (revItVec1 <= revItVec1)
		<< "\nvecRbegin > vecRend: " << (revItVec1 > revItVec2)
		<< "\nvecRbegin >= vecRend: " << (revItVec1 >= revItVec2)
		<< "\nvecRend >= vecRend: " << (revItVec2 >= revItVec2)
		<< std::noboolalpha << std::endl;
}
