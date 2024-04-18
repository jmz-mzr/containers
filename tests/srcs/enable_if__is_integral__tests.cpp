#include <iostream>
#ifdef __cplusplus
# if __cplusplus >= 201103L
#  include <type_traits>
# elif defined(STD)
#  undef STD
# endif
#endif
#include "enable_if.hpp"
#include "is_integral.hpp"

#ifndef STD
# define NMSP	ft
#else
# define NMSP	std
#endif

/******************************************************************************/
/*                            TEMPLATES / FUNCTIONS                           */
/******************************************************************************/

/*
** If the template type parameter passed to these functions
** is not an integral type, the compilation should fail
*/

template <typename T>
typename NMSP::enable_if<NMSP::is_integral<T>::value, bool>::type	isOdd(T i)
{
	return (i % 2);
}

template <typename T>
bool	isEven(T i, typename NMSP::enable_if
								<NMSP::is_integral<T>::value>::type* = NULL)
//								<NMSP::is_integral<T>::value, int>::type = 0)
{
	return (!(i % 2));
}

/*
** Leveraging SFINAE, the right function should be selected from
** the Overload Set without any compilation error
*/

template <typename T>
typename NMSP::enable_if<(sizeof(T) <= sizeof(int))>::type	cmpToInt(void)
{
	std::cout << "is <= int" << std::endl;
}

template <typename T>
typename NMSP::enable_if<(sizeof(T) > sizeof(int))>::type	cmpToInt(void)
{
	std::cout << "is > int" << std::endl;
}

/*
** If implemented correctly, isIntegral calls should be valid with
** either the "::value" member or the "()" type operator, and ignore
** the topmost "const" and "volatile" qualifiers
*/

template <typename T>
void	isIntegral(void)
{
	std::cout << "is an integral type: ";
	if (NMSP::is_integral<T>::value)
		std::cout << "true" << std::endl;
	else if (!NMSP::is_integral<T>())
		std::cout << "false" << std::endl;
}

/******************************************************************************/
/*                                   TESTS                                    */
/******************************************************************************/

void	enable_if__is_integral__tests(void)
{
	std::cout << std::boolalpha;
	std::cout << "isOdd(char(2): " << isOdd(char(2)) << std::endl;
	std::cout << "isEven(short(2): " << isEven(short(2)) << std::endl;
	std::cout << "isOdd(int(2): " << isOdd(int(2)) << std::endl;
	std::cout << "isEven(long(2): " << isEven(long(2)) << "\n" << std::endl;
//	std::cout << isOdd(float(2)) << std::endl; // Shouldn't compile!
//	std::cout << isEven(double(2)) << std::endl; // Shouldn't compile!
	std::cout << std::noboolalpha;

	std::cout << "char "; cmpToInt<char>();
	std::cout << "int "; cmpToInt<int>();
	std::cout << "float "; cmpToInt<float>();
	std::cout << "long "; cmpToInt<long>();
	std::cout << "double "; cmpToInt<double>(); std::cout << std::endl;

	std::cout << "wchar_t "; isIntegral<wchar_t>();
	std::cout << "const short int "; isIntegral<const short int>();
	std::cout << "const signed int "; isIntegral<const signed int>();
	std::cout << "volatile unsigned "; isIntegral<volatile unsigned>();
	std::cout << "const volatile long "; isIntegral<const volatile long>();
	std::cout << "long long "; isIntegral<long long>();
	std::cout << "size_t "; isIntegral<size_t>();
	std::cout << "float "; isIntegral<float>();
	std::cout << "char* "; isIntegral<char*>();
	std::cout << "std::string "; isIntegral<std::string>();
}
