/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector__tests.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmazoyer <jmazoyer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/31 14:37:37 by jmazoyer          #+#    #+#             */
/*   Updated: 2022/03/31 17:53:03 by jmazoyer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <vector>
#include <climits>
#include <exception>
#include <string>
#include <sstream>
#include <iterator>
#include <memory>
#include <cstdio>
#include "vector.hpp"
#include "enable_if.hpp"
#include "is_integral.hpp"
#include "iterator_traits.hpp"
#include "colors.hpp"

#ifndef STD
# define NMSP	ft
#else
# define NMSP	std
#endif

#ifdef __APPLE__
# define SPEED_STRLEN		6000
# define SPEED_VEC_SIZE		2000
#elif defined(__linux__)
# define SPEED_STRLEN		5000
# define SPEED_VEC_SIZE		1750
#endif

/******************************************************************************/
/*                            TEMPLATES / FUNCTIONS                           */
/******************************************************************************/

static std::string	success(const char* str)
{
	std::string		output("  All ");

	output += str;
	output += " work correctly  ";
	output += B_GREEN;
	output += "✔︎";
	output += NO_COLOR;
	output += "\n";
	return (output);
}

/*
** Self-explanatory: it prints a vector's content
*/

template <typename T>
static void	print(const NMSP::vector<T>& vec)
{
	for (typename NMSP::vector<T>::const_iterator it = vec.begin();
			it != vec.end(); ++it)
		std::cout << " " << *it;
	std::cout << std::endl;
}
template <>
void	print(const NMSP::vector<char>& vec)
{
	std::cout << " ";
	for (NMSP::vector<char>::const_iterator it = vec.begin();
			it != vec.end(); ++it)
		std::cout << *it;
	std::cout << std::endl;
}

/*
** Implement "is_const/is_const_pointer<T>" to check wether a type/pointer
** is indeed related to a const type
*/

template <typename T>
struct	is_const: public ft::false_type {
};

template <typename T>
struct	is_const<const T>: public ft::true_type {
};

template <typename T>
struct	is_const_pointer: public ft::false_type {
};

template <typename T>
struct	is_const_pointer<const T*>: public ft::true_type {
};

/*
** Create a type that cannot be default constructed to test if the vector
** can still be empty created
*/

class	priv {
	priv(void);
};

/*
** Create a custom allocator to test with the vector class
*/

template <typename T>
class	myAlloc: public std::allocator<T> {
public:
	typedef size_t		size_type;
	typedef T*			pointer;
	typedef const T*	const_pointer;

	myAlloc(): std::allocator<T>() { /* fprintf(stderr, "myAlloc!\n"); */ }
	myAlloc(const myAlloc &a): std::allocator<T>(a) { }
	template <class U>
	myAlloc(const myAlloc<U>& alloc): std::allocator<T>(alloc) { }
	~myAlloc() { }

	pointer allocate(size_type n, const void* hint = NULL)
	{
		//fprintf(stderr, "Alloc %lu bytes.\n", n * sizeof(T));
		return (std::allocator<T>::allocate(n, hint));
	}

	void deallocate(pointer p, size_type n)
	{
		//fprintf(stderr, "Dealloc %lu bytes (%p).\n", n * sizeof(T), p);
		return (std::allocator<T>::deallocate(p, n));
	}
};

/*
** Build a heavy class to test the performance
** (the defined values depend on your machine: here my Linux VM cannot
** handle the same tests as my native MacOS)
*/

class	HeavyVec {
public:
	HeavyVec(void): str(SPEED_STRLEN, '-'), strVec(SPEED_VEC_SIZE, str) { }
	HeavyVec(char c): str(SPEED_STRLEN, c), strVec(SPEED_VEC_SIZE, str) { }
private:
	std::string					str;
	NMSP::vector<std::string>	strVec;
};

/******************************************************************************/
/*                                   TESTS                                    */
/******************************************************************************/

static void	typedef__tests(void)
{
	typedef NMSP::vector<int>									vec;
	typedef ft::iterator_traits<vec::const_iterator>			it_traits;
	typedef ft::iterator_traits<vec::const_reverse_iterator>	rev_it_traits;

	int															x;
	ft::enable_if<true, vec::value_type>::type					a;
	ft::enable_if<true, vec::allocator_type>::type				b;
	ft::enable_if<true, vec::reference>::type					c = x;
	ft::enable_if<true, vec::const_reference>::type				d = x;
	ft::enable_if<true, vec::pointer>::type						e;
	ft::enable_if<true, vec::const_pointer>::type				f;
	ft::enable_if<true, vec::iterator>::type					g;
	ft::enable_if<true, vec::const_iterator>::type				h;
	ft::enable_if<true, vec::reverse_iterator>::type			i;
	ft::enable_if<true, vec::const_reverse_iterator>::type		j;
	ft::enable_if<true, vec::difference_type>::type				k;
	ft::enable_if<true, vec::size_type>::type					l;

	if (!is_const_pointer<it_traits::pointer>::value
			|| !is_const_pointer<rev_it_traits::pointer>::value)
		std::cout << "Error: vector const_iterators aren't const!\n"
			<< std::endl;

	(void)a; (void)b; (void)c; (void)d; (void)e; (void)f; (void)g; (void)h;
	(void)i; (void)j; (void)k; (void)l;
}

static void	constructors_destructors__tests(void)
{
	NMSP::vector<priv>					vec00;
	try {
		NMSP::vector<std::string>		vec_2(LONG_MAX, "!");
	} catch (const std::exception& e) {
		std::cout << "* Exception: " << e.what() << std::endl;
	} try {
		NMSP::vector<int>				vec_1(-1, 10);
	} catch (const std::exception& e) {
		std::cout << "* Exception: " << e.what() << std::endl;
	}
	NMSP::vector<int>					vec0(0, 10);
	NMSP::vector<int>					vec1;
	NMSP::vector<int, myAlloc<int> >	vec2((myAlloc<int>()));
	NMSP::vector<std::string>			vec3(1, "Hello!");
	std::istringstream					iStr("Hello, world!");
	std::istream_iterator<char>			firstChar(iStr);
	std::istream_iterator<char>			lastChar;
	NMSP::vector<char>					vec4(firstChar, lastChar);
	iStr.clear(); iStr.seekg(0);
	std::istream_iterator<std::string>	firstStr(iStr);
	std::istream_iterator<std::string>	lastStr;
	NMSP::vector<std::string>			vec5(firstStr, lastStr);
	size_t								offset = vec4.max_size()
													- (long)&vec4[0];
	try {
		NMSP::vector<int>				vec6((long)&vec4[0] + offset,
												(long)&vec4[13]);
		std::cout << "vec6.size() = " << vec6.size() << std::endl;
	} catch (const std::exception& e) {
		std::cout << "* Exception: " << e.what() << std::endl;
	}

	std::cout << "vec0 ="; print(vec0);
	std::cout << "vec3 ="; print(vec3);
	std::cout << "vec4 ="; print(vec4);
	std::cout << "vec5 ="; print(vec5);
	std::cout << "vec3 = vec5;" << std::endl; vec3 = vec5;
	std::cout << "vec3 ="; print(vec3);
	std::cout << std::endl;
}

static void	member_operators__tests(void)
{
	NMSP::vector<int>		vec0;
	const NMSP::vector<int>	vec1(2, 1);
	NMSP::vector<int>		vec2(5, 2, myAlloc<int>());

	std::cout << "vec0 ="; print(vec0);
	std::cout << "vec1 ="; print(vec1);
	std::cout << "vec2 ="; print(vec2); std::cout << std::endl;

	std::cout << "vec1[0] = " << vec1[0]
		<< ", vec2[4] = " << vec2[4] << std::endl << std::endl;

	std::cout << "vec0.capacity() = " << vec0.capacity() << std::endl;
	std::cout << "vec0 = vec1;" << std::endl; vec0 = vec1;
	std::cout << "vec0 ="; print(vec0);
	std::cout << "vec0.capacity() = " << vec0.capacity() << std::endl;
	std::cout << "vec0 = vec2;" << std::endl; vec0 = vec2;
	std::cout << "vec0 ="; print(vec0);
	std::cout << "vec0.capacity() = " << vec0.capacity() << std::endl;
	std::cout << "vec0 = vec1;" << std::endl; vec0 = vec1;
	std::cout << "vec0 ="; print(vec0);
	std::cout << "vec0.capacity() = " << vec0.capacity() << std::endl;
	std::cout << std::endl;
}

static void	relational_operators__tests(void)
{
	NMSP::vector<std::string>		vec0;
	const NMSP::vector<std::string>	vec1(1, "Hi");
	NMSP::vector<std::string>		vec2(1, "Hi!");
	NMSP::vector<std::string>		vec3(2, "Hi!");
	NMSP::vector<std::string>		vec4(3, "Hi");

	std::cout << std::boolalpha;
	std::cout << "vec0 ="; print(vec0);
	std::cout << "vec1 ="; print(vec1);
	std::cout << "vec2 ="; print(vec2);
	std::cout << "vec3 ="; print(vec3);
	std::cout << "vec4 ="; print(vec4); std::cout << std::endl;

	std::cout << "vec0 == vec1: " << (vec0 == vec1) << std::endl;
	std::cout << "vec1 != vec1: " << (vec1 != vec1) << std::endl;
	std::cout << "vec1 != vec2: " << (vec1 != vec2) << std::endl;
	std::cout << "vec2 < vec1: " << (vec2 < vec1) << std::endl;
	std::cout << "vec1 < vec2: " << (vec1 < vec2) << std::endl;
	std::cout << "vec2 <= vec2: " << (vec2 <= vec2) << std::endl;
	std::cout << "vec3 <= vec1: " << (vec3 <= vec1) << std::endl;
	std::cout << "vec3 > vec4: " << (vec3 > vec4) << std::endl;
	std::cout << "vec4 >= vec3: " << (vec4 >= vec3) << std::endl;
	std::cout << std::endl << std::noboolalpha;
}

static void	swap__tests(void)
{
	NMSP::vector<int>					vec1(2, 1);
	NMSP::vector<int>					vec2(5, 2);
	NMSP::vector<int>::iterator			it1 = vec1.begin();
	NMSP::vector<int>::const_iterator	it2 = vec2.begin();

	std::cout << "it1 = vec1.begin(); it2 = vec2.begin();" << std::endl;
	std::cout << "vec1 ="; print(vec1);
	std::cout << "vec2 ="; print(vec2);
	std::cout << std::endl;

	std::cout << "vec1.swap(vec2);" << std::endl; vec1.swap(vec2);
	std::cout << "vec1 ="; print(vec1);
	std::cout << "vec2 ="; print(vec2);
	std::cout << "vec2.swap(vec1);" << std::endl; vec2.swap(vec1);
	std::cout << "vec1 ="; print(vec1);
	std::cout << "vec2 ="; print(vec2);
	std::cout << std::endl;

	std::cout << "NMSP::swap(vec1, vec2);" << std::endl; NMSP::swap(vec1, vec2);
	std::cout << "vec1 ="; print(vec1);
	std::cout << "vec2 ="; print(vec2);
	std::cout << "Now through initial it1 and it2 iterators:" << std::endl;
	std::cout << "it1 =";
	for ( ; it1 != vec2.end(); ++it1)
		std::cout << " " << *it1;
	std::cout << std::endl << "it2 =";
	for ( ; it2 != vec1.end(); ++it2)
		std::cout << " " << *it2;
	std::cout << std::endl << std::endl;
}

static void	iterators__tests(void)
{
	NMSP::vector<int>					vec1(3, 1);
	const NMSP::vector<int>				vec2(5, 2);
	NMSP::vector<int>::iterator			it1 = vec1.begin();
	NMSP::vector<int>::const_iterator	it2 = vec2.begin();

	*++it1 = *it2; *it1-- = 1; --it2; it2++;
	std::cout << "vec1 =";
	for ( ; it1 != vec1.rbegin().base(); ++it1)
		std::cout << " " << *it1;
	std::cout << std::endl << "vec2 =";
	for ( ; it2 != vec2.rbegin().base(); ++it2)
		std::cout << " " << *it2;
	std::cout << std::endl << std::endl;
}

static void	capacity__tests(void)
{
	const NMSP::vector<int>	vec0;
	NMSP::vector<int>		vec1(2, 1);
	const NMSP::vector<int>	vec2(5, 2);
	NMSP::vector<int>		vec3(vec0);

	std::cout << "vec0 = "; if (vec0.empty()) std::cout << "(empty)";
	std::cout << std::endl;
	std::cout << "vec1 ="; if (!vec1.empty()) print(vec1);
	std::cout << "vec2 ="; if (vec2.size() == 5) print(vec2);
	std::cout << "vec3 = "; if (vec3.empty()) std::cout << "(empty)\n";
	std::cout << std::endl;

	std::cout << "vec3.capacity() = " << vec3.capacity() << std::endl;
	std::cout << "vec3.reserve(0);" << std::endl; vec3.reserve(0);
	std::cout << "vec3.capacity() = " << vec3.capacity() << "\n" << std::endl;

	std::cout << "vec0.max_size() = " << vec0.max_size() << std::endl;
	std::cout << "vec0.capacity() = " << vec0.capacity() << std::endl;
	std::cout << "vec1.capacity() = " << vec1.capacity() << std::endl;
	std::cout << "vec1.reserve(vec1.max_size() + 1): ";
	try { vec1.reserve(vec1.max_size() + 1);
	} catch (const std::exception& e) { std::cout << "* Exception: "
		<< e.what() << std::endl; }
	std::cout << "vec1 ="; print(vec1);
	std::cout << "vec1.resize(vec1.max_size() + 1): ";
	try { vec1.resize(vec1.max_size() + 1);
	} catch (const std::exception& e) { std::cout << "* Exception: "
		<< e.what() << std::endl; }
	std::cout << "vec1 ="; print(vec1); std::cout << std::endl;

	std::cout << "vec1.reserve(2);" << std::endl; vec1.reserve(2);
	std::cout << "vec1.reserve(0);" << std::endl; vec1.reserve(0);
	std::cout << "vec1.capacity() = " << vec1.capacity() << std::endl;
	std::cout << "vec1.resize(2, -1);" << std::endl; vec1.resize(2, -1);
	std::cout << "vec1 ="; print(vec1); std::cout << std::endl;

	std::cout << "vec1.resize(1, -1);" << std::endl; vec1.resize(1, -1);
	std::cout << "vec1 ="; print(vec1);
	std::cout << "vec1.resize(4);" << std::endl; vec1.resize(4);
	std::cout << "vec1.capacity() = " << vec1.capacity() << std::endl;
	std::cout << "vec1 ="; print(vec1); std::cout << std::endl;

	std::cout << "vec1.resize(7, -1);" << std::endl; vec1.resize(7, -1);
	std::cout << "vec1.capacity() = " << vec1.capacity() << std::endl;
	std::cout << "vec1 ="; print(vec1);
	std::cout << "vec1.resize(8, 0);" << std::endl; vec1.resize(8, 0);
	std::cout << "vec1.capacity() = " << vec1.capacity() << std::endl;
	std::cout << "vec1 ="; print(vec1);
	std::cout << std::endl;
}

static void	element_access__tests(void)
{
	const NMSP::vector<int>	vec0;
	const NMSP::vector<int>	vec1(1, 1);
	NMSP::vector<int>		vec2(2, 2);

	vec2[1] = 3;
	std::cout << "vec0 ="; print(vec0);
	std::cout << "vec1 ="; print(vec1);
	std::cout << "vec2 ="; print(vec2); std::cout << std::endl;

//	std::cout << "vec0.front = " << vec0.front() << std::endl;	// Should crash!
	std::cout << "vec1.front = " << vec1.front() << std::endl;
	std::cout << "vec2.front = " << vec2.front() << std::endl;
	std::cout << "vec1.back = " << vec1.back() << std::endl;
	std::cout << "vec2.back = " << vec2.back() << std::endl;
	std::cout << "vec0.at(0): "; try { std::cout << vec0.at(0) << std::endl;
	} catch (const std::out_of_range& e) { std::cout << "* Exception: "
		<< e.what() << std::endl; }
	std::cout << "vec1.at(0): "; try { std::cout << vec1.at(0) << std::endl;
	} catch (const std::out_of_range& e) { std::cout << "* Exception: "
		<< e.what() << std::endl; }
	std::cout << "vec1.at(1): "; try { std::cout << vec1.at(1) << std::endl;
	} catch (const std::out_of_range& e) { std::cout << "* Exception: "
		<< e.what() << std::endl; }
	std::cout << "vec2.at(-1): "; try { std::cout << vec2.at(-1) << std::endl;
	} catch (const std::out_of_range& e) { std::cout << "* Exception: "
		<< e.what() << std::endl; }
	std::cout << "vec2.at(2): "; try { std::cout << vec2.at(2) << std::endl;
	} catch (const std::out_of_range& e) { std::cout << "* Exception: "
		<< e.what() << std::endl; }
	std::cout << std::endl << std::boolalpha
		<< "vec1.get_allocator() == vec2.get_allocator(): "
		<< (vec1.get_allocator() == vec2.get_allocator())
		<< std::noboolalpha << std::endl << std::endl;
}

static void	modifiers__tests(void)
{
	NMSP::vector<int>			vec0;
	NMSP::vector<int>			vec1(1, 1);
	NMSP::vector<int>			vec2(2, 2);
	int							array[7] = {1, 2, 3, 4, 5, 6, 7};
	std::istringstream			iStr("3 3 3");
	std::istream_iterator<int>	firstInt(iStr);
	std::istream_iterator<int>	lastInt;
	size_t						offset = vec2.max_size() - (long)&vec2[0] + 1;

	std::cout << "vec0 ="; print(vec0);
	std::cout << "vec1 ="; print(vec1);
	std::cout << "vec2 ="; print(vec2);
	std::cout << "array[7] ="; print(NMSP::vector<int>(array, array + 7));
	std::cout << "iStr = 3 3 3" << std::endl << std::endl;

	std::cout << "vec0.assign(iStr.first, iStr.last);" << std::endl;
	vec0.assign(firstInt, lastInt);
	std::cout << "vec0 ="; print(vec0);
	try { vec0.assign((long)&vec2[0] + offset, (long)&vec2[2] + offset);
	} catch (const std::exception& e) { 
		std::cout << "* Exception: " << e.what() << std::endl; }
	std::cout << "vec0 ="; print(vec0); std::cout << std::endl;

	std::cout << "vec0.assign(0, 1);" << std::endl; vec0.assign(0, 1);
	std::cout << "vec0 ="; print(vec0);
	std::cout << "vec0.assign(1, 1);" << std::endl; vec0.assign(1, 1);
	std::cout << "vec0 ="; print(vec0);
	std::cout << "vec0.capacity() = " << vec0.capacity() << std::endl;
	std::cout << "vec0.assign(5, 5);" << std::endl; vec0.assign(5, 5);
	std::cout << "vec0 ="; print(vec0); std::cout << std::endl;

	std::cout << "vec0.assign(vec1.begin(), vec1.begin());" << std::endl;
	vec0.assign(vec1.begin(), vec1.begin());
	std::cout << "vec0 ="; print(vec0);
	std::cout << "vec0.assign(vec2.begin(), vec2.end());" << std::endl;
	vec0.assign(vec2.begin(), vec2.end());
	std::cout << "vec0 ="; print(vec0);
	std::cout << "vec0.capacity() = " << vec0.capacity() << std::endl;
	std::cout << "vec0.assign(array, array + 7);" << std::endl;
	vec0.assign(array, array + 7);
	std::cout << "vec0 ="; print(vec0); std::cout << std::endl;

	std::cout << "vec0.capacity() = " << vec0.capacity() << std::endl;
	std::cout << "vec0.push_back(8, 9, 10);" << std::endl;
	vec0.push_back(8); vec0.push_back(9); vec0.push_back(10);
	std::cout << "vec0.capacity() = " << vec0.capacity() << std::endl;
	std::cout << "vec0 ="; print(vec0); std::cout << std::endl;

	std::cout << "vec0.pop_back() x 3;";
	vec0.pop_back(); vec0.pop_back(); vec0.pop_back(); std::cout << std::endl;
	std::cout << "vec0.capacity() = " << vec0.capacity() << std::endl;
	std::cout << "vec0 ="; print(vec0); std::cout << std::endl;

	std::cout << "vec0.insert(vec0.end(), 8);" << std::endl;
	vec0.insert(vec0.end(), 8);
	std::cout << "vec0.insert(vec0.begin(), vec0[0]);" << std::endl;
	vec0.insert(vec0.begin(), vec0[0]);
	std::cout << "vec0.insert(vec0.begin() + 1, vec0[8]);" << std::endl;
	vec0.insert(vec0.begin() + 1, vec0[8]);
	std::cout << "vec0 ="; print(vec0); std::cout << std::endl;
/*
	// Undefined behaviour -- it will probably crash!
	std::cout << "vec0.insert(vec0.begin() - 100, vec0[0]);" << std::endl;
	vec0.insert(vec0.begin() - 100, vec0[0]);
	std::cout << "vec0 ="; print(vec0);
	std::cout << "vec0.insert(vec0.end() + 100, vec0[0]);" << std::endl;
	vec0.insert(vec0.begin() + 100, vec0[0]);
	std::cout << "vec0 ="; print(vec0); std::cout << std::endl;
*/
	std::cout << "vec1.capacity() = " << vec1.capacity() << std::endl;
	std::cout << "vec1 ="; print(vec1);
	std::cout << "*vec1.insert(vec1.begin(), 0) = "
		<< *vec1.insert(vec1.begin(), 0) << std::endl;
	std::cout << "*vec1.insert(vec1.end(), 2) = "
		<< *vec1.insert(vec1.end(), 2) << std::endl;
	std::cout << "vec1.capacity() = " << vec1.capacity() << std::endl;
	std::cout << "vec1 ="; print(vec1); std::cout << std::endl;

	std::cout << "vec1.insert(vec1.end(), 1, 3);" << std::endl;
	vec1.insert(vec1.end(), 1, 3);
	std::cout << "vec1.insert(vec1.begin(), 1, vec1[0]);" << std::endl;
	vec1.insert(vec1.begin(), 1, vec1[0]);
	std::cout << "vec1.insert(vec1.end() - 1, 3, vec1[4]);" << std::endl;
	vec1.insert(vec1.end() - 1, 3, vec1[4]);
	std::cout << "vec1.capacity() = " << vec1.capacity() << std::endl;
	std::cout << "vec1.insert(vec1.begin() + 1, 1, vec1[0]);" << std::endl;
	vec1.insert(vec1.begin() + 1, 1, vec1[0]);
	std::cout << "vec1.capacity() = " << vec1.capacity() << std::endl;
	std::cout << "vec1 ="; print(vec1); std::cout << std::endl;
/*
	// Undefined behaviour -- it will probably crash!
	std::cout << "vec1.insert(vec1.begin() - 100, 50, vec1[0]);" << std::endl;
	vec0.insert(vec1.begin() - 100, 50, vec1[0]);
	std::cout << "vec1 ="; print(vec1);
	std::cout << "vec1.insert(vec1.end() + 100, 50, vec1[0]);" << std::endl;
	vec0.insert(vec1.begin() + 100, 50, vec1[0]);
	std::cout << "vec1.capacity() = " << vec1.capacity() << std::endl;
	std::cout << "vec1 ="; print(vec1); std::cout << std::endl;
*/
	std::cout << "vec0.insert(vec0.begin(), vec0.max_size(), 2);" << std::endl;
	try { vec0.insert(vec0.begin(), vec0.max_size(), 2);
	} catch (const std::exception& e) { 
		std::cout << "* Exception: " << e.what() << "\n" << std::endl; }

	iStr.clear(); iStr.seekg(0); firstInt = iStr; std::cout
		<< "vec1.insert(vec1.begin() + 2, iStr.first, iStr.last);" << std::endl;
	vec1.insert(vec1.begin() + 2, firstInt, lastInt);
	std::cout << "vec1[7] = 4; vec1[10] = 5;" << std::endl;
	vec1[7] = 4; vec1[10] = 5;
	iStr.clear(); iStr.seekg(0); firstInt = iStr;
	std::cout << "vec1.insert(vec1.begin(), firstInt, lastInt);" << std::endl;
	vec1.insert(vec1.begin(), firstInt, lastInt);
	iStr.clear(); iStr.seekg(0); firstInt = iStr;
	std::cout << "vec1.insert(vec1.end(), firstInt, lastInt);" << std::endl;
	vec1.insert(vec1.end(), firstInt, lastInt);
	std::cout << "vec1.capacity() = " << vec1.capacity() << std::endl;
	std::cout << "vec1 ="; print(vec1); std::cout << std::endl;

	std::cout << "vec2.capacity() = " << vec2.capacity() << std::endl;
	std::cout << "vec2 ="; print(vec2); std::cout << std::endl;
	std::cout << "vec2.insert(vec2.end() - 1, array, array + 1);" << std::endl;
	vec2.insert(vec2.end() - 1, array, array + 1);
	std::cout << "vec2.insert(vec2.end(), array, array + 2);" << std::endl;
	vec2.insert(vec2.end(), array, array + 2);
	std::cout << "vec2.capacity() = " << vec2.capacity() << std::endl;
	std::cout << "vec2.insert(vec2.begin(), array, array + 3);" << std::endl;
	vec2.insert(vec2.begin(), array, array + 3);
	std::cout << "vec2.insert(vec2.end(), array + 2, array + 4);" << std::endl;
	vec2.insert(vec2.end(), array + 2, array + 4);
	std::cout << "vec2.capacity() = " << vec2.capacity() << std::endl;
	std::cout << "vec2.insert(vec2.end() - 1, array, array + 1);" << std::endl;
	vec2.insert(vec2.end() - 1, array, array + 1);
	std::cout << "vec2.capacity() = " << vec2.capacity() << std::endl;
	std::cout << "vec2 ="; print(vec2); std::cout << std::endl;
/*
	// Undefined behaviour
	std::cout << "vec2.insert(vec2.end(), array + 5, array);" << std::endl;
	vec2.insert(vec2.end(), array + 5, array);
	std::cout << "vec2 ="; print(vec2); std::cout << std::endl;
*/
	std::cout << "*vec2.erase(vec2.begin()) = "
		<< *vec2.erase(vec2.begin()) << std::endl;
	std::cout << "*vec2.erase(vec2.begin() + 1) = "
		<< *vec2.erase(vec2.begin() + 1) << std::endl;
	std::cout << "vec2.erase(vec2.end() - 1) = ";
	if (vec2.erase(vec2.end() - 1) == vec2.end())
		std::cout << "vec2.end()" << std::endl;
	std::cout << "*vec2.erase(vec2.begin(), vec2.begin()) = "
		<< *vec2.erase(vec2.begin(), vec2.begin()) << std::endl;
	std::cout << "*vec2.erase(vec2.begin() + 1, vec2.begin() + 2) = "
		<< *vec2.erase(vec2.begin() + 1, vec2.begin() + 2) << std::endl;
	std::cout << "vec2.erase(vec2.end(), vec2.end()) = ";
	if (vec2.erase(vec2.end(), vec2.end()) == vec2.end())
		std::cout << "vec2.end()" << std::endl;
	std::cout << "vec2.erase(vec2.end() - 1, vec2.end()) = ";
	if (vec2.erase(vec2.end() - 1, vec2.end()) == vec2.end())
		std::cout << "vec2.end()" << std::endl;
	std::cout << "vec2.erase(vec2.begin(), vec2.end()) = ";
	if (vec2.erase(vec2.begin(), vec2.end()) == vec2.end())
		std::cout << "vec2.end()" << std::endl;
	std::cout << "vec2.capacity() = " << vec2.capacity() << std::endl;
	std::cout << "vec2 ="; print(vec2); std::cout << std::endl;
}

static void	speed__tests(void)
{
	NMSP::vector<HeavyVec>				vec1(100);
	NMSP::vector<HeavyVec>				vec2;
	HeavyVec							heavy('+');
	std::string							str(SPEED_STRLEN, '-');
	NMSP::vector<std::string>			vec3;
	NMSP::vector<std::string>::iterator	it;

	vec2.assign(vec1.begin(), vec1.end());
	vec2.insert(vec2.begin() + (vec2.size() / 2), 20, heavy);
	vec2.insert(vec2.begin() + (vec2.size() / 2), 20, heavy);
	vec2.insert(vec2.begin() + (vec2.size() / 2), 20, heavy);
	vec2.insert(vec2.begin() + (vec2.size() / 2), 20, heavy);
	vec2.insert(vec2.begin() + (vec2.size() / 2), 20, heavy);
	for (int i = 0; i != 200; ++i)
		vec3.insert(vec3.begin() + vec3.size() / 2, 20, str);
	it = vec3.begin();
	while (it != vec3.end())
		it = vec3.erase(it);
}

void	vector__tests(bool testSpeed)
{
	typedef__tests();
	std::cout << success("typedefs") << std::endl;
	constructors_destructors__tests();
	std::cout << success("constructors & destructors") << std::endl;
	member_operators__tests();
	std::cout << success("member operators") << std::endl;
	relational_operators__tests();
	std::cout << success("relational operators") << std::endl;
	swap__tests();
	std::cout << success("swap functions") << std::endl;
	iterators__tests();
	std::cout << success("iterators") << std::endl;
	capacity__tests();
	std::cout << success("capacity functions") << std::endl;
	element_access__tests();
	std::cout << success("element access functions") << std::endl;
	modifiers__tests();
	std::cout << success("modifier functions") << std::endl;
	if (testSpeed) {
		speed__tests();
		std::cout << success("speed tests") << std::endl;
	}
}
