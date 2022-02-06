#include <iostream>
#include <algorithm>
#include <list>

#define HEADER1		"*****************************************************\n*"
#define HEADER2		"*\n*****************************************************\n"
#define SPACE15		"               "

template <typename T1, typename T2>
struct	isEqualFunctor {
	bool	operator()(const T1& a, const T2& b) { return (a == b); }
};

template <>
struct	isEqualFunctor<int, char> {
	bool	operator()(const int& a, const char& b) {
		return (std::tolower(a) == b);
	}
};

bool	isEqual(int a, int b)
{
	return (a == b);
}

bool	isPalindrome(const std::string& str)
{
	if (std::equal(str.begin(), str.begin() + (str.size() / 2), str.rbegin()))
		return (true);
	if (!std::equal(str.begin(), str.begin() + (str.size() / 2), str.rbegin(),
				isEqualFunctor<int, char>()))
		return (false);
	return (42);
}

void	equal__tests_std(void)
{
	long			array[10] = {0, 2, 4, 6, 8, 10, 12};
	std::list<int>	list(array, array + 7);

	std::cout << HEADER1 << SPACE15 << "        EQUAL        "
		<< SPACE15 << HEADER2 << std::endl;

	std::cout << std::boolalpha;
	std::cout << "array<long>[10] =";
	for (int i = 0; i != 7; ++i)
		std::cout << " " << array[i];
	std::cout << "\nlist<int>       =";
	for (std::list<int>::iterator it = list.begin(); it != list.end(); ++it)
		std::cout << " " << *it;
	std::cout << "\n   equal(array, array + 7, list.begin()): "
		<< std::equal(array, array + 7, list.begin()) << std::endl;
	list.reverse();
	std::cout << "list.reverse();\n"
		"   equal(array, array + 7, list.rbegin(), isEqual): "
		<< std::equal(array, array + 7, list.rbegin(), isEqual) << std::endl;
	list.push_front(14);
	std::cout << "list.push_front(14);\n"
		"   equal(array, array + 7, list.rbegin()): "
		<< std::equal(array, array + 7, list.rbegin())
		<< "\n   equal(array, array + 8, list.rbegin()): "
		<< std::equal(array, array + 8, list.rbegin()) << "\n" << std::endl;

	std::cout << "\"level\" is a palindrome: ";
	std::cout << isPalindrome("level") << std::endl;
	std::cout << "\"step on no pets\" is a palindrome: ";
	std::cout << isPalindrome("step on no pets") << std::endl;
	std::cout << "\"Kayak\" is a palindrome: ";
	std::cout << isPalindrome("Kayak") << std::endl;
	std::cout << std::noboolalpha;
}
