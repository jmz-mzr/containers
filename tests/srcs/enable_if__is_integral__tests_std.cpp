#include <iostream>
#include "enable_if.hpp"
#include "is_integral.hpp"

#define HEADER1		"*****************************************************\n*"
#define HEADER2		"*\n*****************************************************\n"
#define SPACE15		"               "

template <typename T>
typename std::enable_if<std::is_integral<T>::value, bool>::type	isOdd(T i)
{
	return (i % 2);
}

template <typename T>
bool	isEven(T i, typename std::enable_if
									<std::is_integral<T>::value>::type* = NULL)
//									<std::is_integral<T>::value, int>::type = 0)
{
	return (!(i % 2));
}

template <typename T>
typename std::enable_if<(sizeof(T) <= sizeof(int))>::type	cmpToInt(void)
{
	std::cout << "is <= int" << std::endl;
}

template <typename T>
typename std::enable_if<(sizeof(T) > sizeof(int))>::type	cmpToInt(void)
{
	std::cout << "is > int" << std::endl;
}

template <typename T>
void	isIntegral(void)
{
	std::cout << "is an integral type: ";
	if (std::is_integral<T>::value)
		std::cout << "true" << std::endl;
	else if (!std::is_integral<T>())
		std::cout << "false" << std::endl;
}

void	enable_if__is_integral__tests_std(void)
{
	std::cout << HEADER1 << SPACE15 << "      ENABLE_IF      " << SPACE15
		<< "*\n*       (if not done right it doesn't compile)      "
		<< HEADER2 << std::endl;

	std::cout << std::boolalpha;
	std::cout << "isOdd(char(2): " << isOdd(char(2)) << std::endl;
	std::cout << "isEven(short(2): " << isEven(short(2)) << std::endl;
	std::cout << "isOdd(int(2): " << isOdd(int(2)) << std::endl;
	std::cout << "isEven(long(2): " << isEven(long(2)) << "\n" << std::endl;
//	std::cout << isOdd(float(2)) << std::endl; // Shouldn't compile!
//	std::cout << isEven((double(2)) << std::endl; // Shouldn't compile!
	std::cout << std::noboolalpha;

	std::cout << "char "; cmpToInt<char>();
	std::cout << "int "; cmpToInt<int>();
	std::cout << "float "; cmpToInt<float>();
	std::cout << "long "; cmpToInt<long>();
	std::cout << "double "; cmpToInt<double>(); std::cout << std::endl;

	std::cout << HEADER1 << SPACE15 << "     IS_INTEGRAL     " << SPACE15
		<< HEADER2 << std::endl;

	std::cout << "wchar_t "; isIntegral<wchar_t>();
	std::cout << "const short int "; isIntegral<const short int>();
	std::cout << "const signed int "; isIntegral<const signed int>();
	std::cout << "volatile unsigned "; isIntegral<volatile unsigned>();
	std::cout << "long long "; isIntegral<long long>();
	std::cout << "size_t "; isIntegral<size_t>();
	std::cout << "float "; isIntegral<float>();
	std::cout << "char* "; isIntegral<char*>();
	std::cout << "std::string "; isIntegral<std::string>();
}
