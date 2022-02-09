#include <iostream>
#include <vector>
#include <list>
#include <iterator>

#define HEADER1		"*****************************************************\n*"
#define HEADER2		"*\n*****************************************************\n"
#define SPACE15		"               "

template <typename RandomAccessIt>
typename std::iterator_traits<RandomAccessIt>::difference_type
	myDistance(RandomAccessIt first, RandomAccessIt last,
				std::random_access_iterator_tag)
{
	return (last - first);
}
template <typename InputIt>
typename std::iterator_traits<InputIt>::difference_type
	myDistance(InputIt first, InputIt last, std::input_iterator_tag)
{
	typename std::iterator_traits<InputIt>::difference_type	n = 0;

	for ( ; first != last; ++n, ++first)
		;
	return (n);
}
template <typename Iter>
typename std::iterator_traits<Iter>::difference_type
	myDistance(Iter first, Iter last)
{
	return (myDistance(first, last,
				typename std::iterator_traits<Iter>::iterator_category()));
}

template <typename BidirIt>
void	myReverse(BidirIt first, BidirIt last)
{
	typename std::iterator_traits<BidirIt>::difference_type	n = 0;
	typename std::iterator_traits<BidirIt>::value_type		tmp;

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
	typename std::iterator_traits<const T*>::value_type	tmp = *first;

	if (first != last)
		tmp = *(last - 1);
}

template <typename BidirIt>
void	print(BidirIt first, BidirIt last)
{
	typename std::iterator_traits<BidirIt>::value_type	tmp = *first;

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
	typename std::iterator_traits<const T*>::value_type	tmp = *first;

	if (first != last)
		tmp = *(last - 1);
	while (first != last)
		std::cout << *first++;
	std::cout << std::endl;
}

void	iterator_traits__tests_std(void)
{
	std::cout << HEADER1 << SPACE15 << "   ITERATOR_TRAITS   " << SPACE15
		<< HEADER2 << std::endl;

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
