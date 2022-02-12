#include <iostream>
#include <vector>
#include <type_traits>
#include <climits>
#include <exception>
#include <string>
#include <sstream>
#include <iterator>
#include <memory>
#include <cstdio>
#include "vector.hpp"

#ifndef STD
# define NMSP	ft
#else
# define NMSP	std
#endif

/******************************************************************************/
/*                            TEMPLATES / FUNCTIONS                           */
/******************************************************************************/

/*
** Self-explanatory: it prints a vector's content
*/

template <typename T>
void	print(const NMSP::vector<T>& vec)
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
** Use a custom allocator to test with our vector class
*/

template <typename T>
class	myAlloc: public std::allocator<T> {
public:
	typedef size_t		size_type;
	typedef T*			pointer;
	typedef const T* 	const_pointer;

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

/******************************************************************************/
/*                                   TESTS                                    */
/******************************************************************************/

void	typedef__tests(void)
{
	typedef NMSP::vector<int>									vec;
	int															x;
	std::enable_if<true, vec::value_type>::type					a;
	std::enable_if<true, vec::allocator_type>::type				b;
	std::enable_if<true, vec::reference>::type					c = x;
	std::enable_if<true, vec::const_reference>::type			d = x;
	std::enable_if<true, vec::pointer>::type					e;
	std::enable_if<true, vec::const_pointer>::type				f;
	std::enable_if<true, vec::iterator>::type					g;
	std::enable_if<true, vec::const_iterator>::type				h;
	std::enable_if<true, vec::reverse_iterator>::type			i;
	std::enable_if<true, vec::const_reverse_iterator>::type		j;
	std::enable_if<true, vec::difference_type>::type			k;
	std::enable_if<true, vec::size_type>::type					l;

	(void)a; (void)b; (void)c; (void)d; (void)e; (void)f; (void)g; (void)h;
	(void)i; (void)j; (void)k; (void)l;
}

void	constructors_destructors__tests(void)
{
	try {
		NMSP::vector<std::string>		vec_2(LONG_MAX, "!");
	} catch (const std::length_error& e) {
		std::cout << "* Exception: " << e.what() << std::endl;
	} try {
		NMSP::vector<int>				vec_1(-1, 10);
	} catch (const std::length_error& e) {
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
	try {
		NMSP::vector<char>				vec6((long)&vec4[0], (long)&vec4[13]);
	} catch (const std::bad_alloc& e) {
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

void	member_operators__tests(void)
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

void	relational_operators__tests(void)
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

void	swap__tests(void)
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

void	iterators__tests(void)
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

void	capacity__tests(void)
{
	const NMSP::vector<int>	vec0;
	NMSP::vector<int>		vec1(2, 1);
	const NMSP::vector<int>	vec2(5, 2);

	std::cout << "vec0 = "; if (vec0.empty()) std::cout << "(empty)";
	std::cout << std::endl;
	std::cout << "vec1 ="; if (!vec1.empty()) print(vec1);
	std::cout << "vec2 ="; if (vec2.size() == 5) print(vec2);
	std::cout << std::endl;

	std::cout << "vec0.max_size() = " << vec0.max_size() << std::endl;
	std::cout << "vec0.capacity() = " << vec0.capacity() << std::endl;
	std::cout << "vec1.capacity() = " << vec1.capacity() << std::endl;
	std::cout << "vec1.reserve(LONG_MAX): "; try { vec1.reserve(LONG_MAX);
	} catch (const std::length_error& e) { std::cout << "* Exception: "
		<< e.what() << std::endl; }
	std::cout << "vec1 ="; print(vec1); std::cout << std::endl;

	std::cout << "vec1.reserve(2);" << std::endl; vec1.reserve(2);
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

void	vector__tests(void)
{
	typedef__tests();
	std::cout << "   All typedefs are correctly defined\n" << std::endl;
	constructors_destructors__tests();
	std::cout << "   All constructors & destructors work correctly\n" << std::endl;
	member_operators__tests();
	std::cout << "   All member operators work correctly\n" << std::endl;
	relational_operators__tests();
	std::cout << "   All relational operators work correctly\n" << std::endl;
	swap__tests();
	std::cout << "   All swap functions work correctly\n" << std::endl;
	iterators__tests();
	std::cout << "   All iterators work correctly\n" << std::endl;
	capacity__tests();
	std::cout << "   All capacity functions work correctly\n" << std::endl;
//	speed__tests();
}
