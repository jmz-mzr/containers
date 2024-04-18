/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set__tests.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmazoyer <jmazoyer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/31 14:37:37 by jmazoyer          #+#    #+#             */
/*   Updated: 2022/03/31 17:56:25 by jmazoyer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <set>
#include <vector>
#include "set.hpp"
#include "colors.hpp"
#include "enable_if.hpp"
#include "is_integral.hpp"
#include "iterator_traits.hpp"
#include "pair.hpp"
#include "make_pair.hpp"
#include "detect_system.hpp"

#ifndef STD
# define NMSP	ft
#else
# define NMSP	std
#endif

#ifdef __APPLE__
# define SPEED_SET			4000000
#elif defined(__linux__)
# define SPEED_SET			4500000
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
** Self-explanatory: it prints a set's content
*/

template <typename Key, class Comp, class Alloc>
static void	print(const NMSP::set<Key, Comp, Alloc>& set)
{
	typename NMSP::set<Key, Comp, Alloc>::const_iterator	it = set.begin();

	while (it != set.end()) {
		std::cout << *it;
		if (++it != set.end())
			std::cout << ", ";
	}
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
** Create a type that cannot be default constructed to test if the set
** can still be created
*/

class	priv {
	priv(void);
};

/*
** Create a custom allocator to test with the set class
*/

template <typename T>
class	myAlloc: public std::allocator<T> {
public:
	typedef size_t		size_type;
	typedef T*			pointer;
	typedef const T*	const_pointer;

	myAlloc(): std::allocator<T>() { }
	myAlloc(const myAlloc &a): std::allocator<T>(a) { }
	template <class U>
	myAlloc(const myAlloc<U>& alloc): std::allocator<T>(alloc) { }
	~myAlloc() { }

	pointer allocate(size_type n, const void* hint = NULL)
	{
		return (std::allocator<T>::allocate(n, hint));
	}

	void deallocate(pointer p, size_type n)
	{
		return (std::allocator<T>::deallocate(p, n));
	}
};

/*
** Allow std::vector to be used with "operator<<"
*/

template <typename T>
struct	myVec: public std::vector<T>
{
	myVec(void): std::vector<T>() { }
	myVec(const myVec& src): std::vector<T>(src) { }
	myVec(size_t n, const T& val): std::vector<T>(n, val) { }
	template <typename InputIt>
	myVec(InputIt first, InputIt last): std::vector<T>(first, last) { }
};

template <typename T>
std::ostream&	operator<<(std::ostream& os, const myVec<T>& vec)
{
	typename std::vector<T>::const_iterator it = vec.begin();

	os << "[";
	while (it != vec.end()) {
		os << *it;
		if (++it != vec.end())
			os << ", ";
	}
	os << "]";
	return (os);
}

/*
** Allow NMSP::pair to be used with "operator<<"
*/

template <typename T1, typename T2>
struct	myPair: public NMSP::pair<T1, T2>
{
	myPair(void): NMSP::pair<T1, T2>() { }
	myPair(const T1& x, const T2& y): NMSP::pair<T1, T2>(x, y) { }
};

template <typename T1, typename T2>
std::ostream&	operator<<(std::ostream& os, const myPair<T1, T2>& pair)
{
	os << "(" << pair.first << ", " << pair.second << ")";
	return (os);
}

/******************************************************************************/
/*                                   TESTS                                    */
/******************************************************************************/

static void	typedef__tests(void)
{
	typedef NMSP::set<std::string>							set_t;
	typedef ft::iterator_traits<set_t::iterator>			it_traits;
	typedef ft::iterator_traits<set_t::reverse_iterator>	rev_it_traits;

	std::string													x = "0";
	std::less<std::string>										comp;
	std::allocator<std::string>									alloc;
	ft::enable_if<true, set_t::key_type>::type					a = x;
	ft::enable_if<true, set_t::value_type>::type				b = x;
	ft::enable_if<true, set_t::key_compare>::type				c = comp;
	ft::enable_if<true, set_t::value_compare>::type				d = comp;
	ft::enable_if<true, set_t::allocator_type>::type			e = alloc;
	ft::enable_if<true, set_t::reference>::type					f = x;
	ft::enable_if<true, set_t::const_reference>::type			g = x;
	ft::enable_if<true, set_t::pointer>::type					h;
	ft::enable_if<true, set_t::const_pointer>::type				i;
	ft::enable_if<true, set_t::size_type>::type					j;
	ft::enable_if<true, set_t::difference_type>::type			k;
	ft::enable_if<true, set_t::iterator>::type					l;
	ft::enable_if<true, set_t::const_iterator>::type			m;
	ft::enable_if<true, set_t::reverse_iterator>::type			n;
	ft::enable_if<true, set_t::const_reverse_iterator>::type	o;

	if (!is_const_pointer<it_traits::pointer>::value
			|| !is_const_pointer<rev_it_traits::pointer>::value)
		std::cout << "Error: set iterators aren't const!\n" << std::endl;

	(void)a; (void)b; (void)c; (void)d; (void)e; (void)f; (void)g; (void)h;
	(void)i; (void)j; (void)k; (void)l; (void)m; (void)n; (void)o;
}

static void	constructors_destructors__tests(void)
{
	typedef myVec<char>		vec_t;

//	NMSP::set<int>		/*Shouldn't compile*/		set00 = (std::less<int>());
	NMSP::set<priv>									set0;
	NMSP::set<vec_t, std::less<vec_t> >				set1((std::less<vec_t>()));
	NMSP::set<vec_t, std::less<vec_t>,
				myAlloc<vec_t> >	set2
													((std::less<vec_t>()),
													myAlloc<vec_t>());
	set2.insert(vec_t(1, 'a'));
	set2.insert(vec_t(2, 'b'));
	set2.insert(vec_t(4, 'd'));
	set2.insert(vec_t(3, 'c'));
	const NMSP::set<vec_t, std::less<vec_t>,
								myAlloc<vec_t> >	set3(set2);
	NMSP::set<vec_t>								set4(set3.begin(),
															set3.end());
	NMSP::set<vec_t>								set5(set3.begin(),
															set3.end(),
														((std::less<vec_t>())));
	NMSP::set<vec_t>								set6(set3.begin(),
															set3.end(),
														((std::less<vec_t>())),
														myAlloc<vec_t>());

	std::cout << "set2: "; print(set2);
	std::cout << "set3(set2): "; print(set3);
	std::cout << "set4: "; print(set4);
	std::cout << "set5: "; print(set5);
	std::cout << "set6: "; print(set6);
	set2.erase(set2.begin());
	std::cout << std::endl;
}

static void	member_operators__tests(void)
{
	const NMSP::set<std::string>	set0;
	NMSP::set<std::string>			set1;
	set1.insert("02");
	set1.insert("01");
	set1.insert("00");
	NMSP::set<std::string>			set2(set1);

	std::cout << "set1: "; print(set1);
	std::cout << "set2(set1): "; print(set2);
	std::cout << "set0: "; print(set0);
	std::cout << "set2 = set0: "; set2 = set0; print(set2);
	std::cout << std::endl;
}

static void	element_access__tests(void)
{
	const NMSP::set<std::string>	set0;
	NMSP::set<std::string>			set1;
	std::allocator<std::string>		alloc0(set0.get_allocator());
	std::allocator<std::string>		alloc1(set1.get_allocator());
	std::string*					ptr;

	std::cout << std::boolalpha;
	std::cout << "set0.key_comp()(\"0\", \"1\"): " << set0.key_comp()("0", "1")
		<< std::endl;
	std::cout << "set0.key_comp()(\"1\", \"1\"): " << set0.key_comp()("1", "1")
		<< std::endl;
	std::cout << "set1.key_comp()(\"0\", \"1\"): " << set1.key_comp()("0", "1")
		<< std::endl;
	std::cout << "set1.key_comp()(\"2\", \"1\"): " << set1.key_comp()("2", "1")
		<< std::endl;
	std::cout << std::endl;

	std::cout << "set0.value_comp()(\"0\", \"1\"): "
		<< set0.value_comp()("0", "1") << std::endl;
	std::cout << "set0.value_comp()(\"1\", \"1\"): "
		<< set0.value_comp()("1", "1") << std::endl;
	std::cout << "set1.value_comp()(\"0\", \"1\"): "
		<< set1.value_comp()("0", "1") << std::endl;
	std::cout << "set1.value_comp()(\"2\", \"1\"): "
		<< set1.value_comp()("2", "1") << std::endl;
	std::cout << std::endl;

	ptr = alloc0.allocate(1);
	alloc0.deallocate(ptr, 1);
	ptr = alloc1.allocate(1);
	alloc1.deallocate(ptr, 1);
}

static void	capacity__tests(void)
{
	const NMSP::set<myPair<int, int> >	set1;
	NMSP::set<myPair<int, int> >		set2;

	std::cout << "set1: "; print(set1);
	std::cout << "set1.size() = " << set1.size()
		<< ", set1.max_size() = " << set1.max_size() << "\n" << std::boolalpha
		<< "set1.empty(): " << set1.empty() << "\n" << std::endl;

	set2.insert(myPair<int, int>(1, 1));
	set2.insert(myPair<int, int>(-3, -3));
	set2.insert(myPair<int, int>(-2, -2));
	std::cout << "set2: "; print(set2);
	std::cout << "set2.size() = " << set2.size()
		<< ", set2.max_size() = " << set2.max_size() << "\n" << std::endl;

	std::cout << "set2.erase(-3);" << std::endl;
	set2.erase(myPair<int, int>(-3, -3));
	std::cout << "set2: "; print(set2);
	std::cout << "set2.size() = " << set2.size() << "\n" << std::endl;

	std::cout << "set2.empty(): " << set2.empty() << std::endl;
	std::cout << "set2 = set1;" << std::endl; set2 = set1;
	std::cout << "set2: "; print(set2);
	std::cout << "set2.empty(): " << set2.empty() << std::noboolalpha
		<< std::endl;
	std::cout << "set2.size() = " << set2.size() << "\n" << std::endl;
}

static void	iterators__tests(void)
{
	typedef myPair<int, int>	pair_t;

	NMSP::set<pair_t>							set1;
	set1.insert(pair_t(1, 1));
	set1.insert(pair_t(2, 2));
	const NMSP::set<pair_t>						set2(set1);
	NMSP::set<pair_t>::iterator					it0;
	NMSP::set<pair_t>::const_iterator			const_it0;
	NMSP::set<pair_t>::iterator					it1(set1.end());
	NMSP::set<pair_t>::const_iterator			const_it1(set2.end());
	NMSP::set<pair_t>::iterator					it2(it0);
	NMSP::set<pair_t>::const_iterator			const_it2(it0);
	NMSP::set<pair_t>::const_iterator			const_it2_2(const_it0);
	NMSP::set<pair_t>::reverse_iterator			r_it0;
	NMSP::set<pair_t>::const_reverse_iterator	const_r_it0;
	NMSP::set<pair_t>::reverse_iterator			r_it1(set1.end());
	NMSP::set<pair_t>::const_reverse_iterator	const_r_it1(set2.end());
	NMSP::set<pair_t>::reverse_iterator			r_it2(it0);
	NMSP::set<pair_t>::const_reverse_iterator	const_r_it2(it0);
	NMSP::set<pair_t>::const_reverse_iterator	const_r_it2_2(const_it0);
	r_it2 = r_it0;
	const_r_it2 = r_it0;
	const_r_it2_2 = const_r_it0;

	it0 = set1.begin(); const_it0 = set1.end();
	std::cout << std::boolalpha;
	std::cout << "set1.begin() == set1.end() (const): "
		<< (it0 == const_it0) << std::endl;
	std::cout << "set1.end() (const) == set1.begin(): "
		<< (const_it0 == it0) << std::endl;
	const_it0 = set1.begin(); it0 = set1.end();
	std::cout << "set1.begin() (const) != set1.end(): "
		<< (it0 != const_it0) << std::endl;
	std::cout << "set1.end() != set1.begin() (const): "
		<< (const_it0 != it0) << std::endl;
	std::cout << std::noboolalpha << std::endl;

	it2 = it1;
	const_it2 = it1;
	const_it2_2 = const_it1;
	if (!LIBCPP) {
		it2++; it2 = it1;
		++it2; it2 = it1;
	}
	it2--; it2 = it1;
	--it2;
	if (!LIBCPP) {
		const_it2++; const_it2 = const_it1;
		++const_it2; const_it2 = const_it1;
	}
	const_it2--; const_it2 = const_it1;
	--const_it2;

	r_it2 = r_it1;
	const_r_it2 = r_it1;
	const_r_it2_2 = const_r_it1;
	r_it2++; r_it2 = r_it1;
	++r_it2; r_it2 = r_it1;
	if (!LIBCPP) {
		r_it2--; r_it2 = r_it1;
		--r_it2;
	}
		const_r_it2++; const_r_it2 = const_r_it1;
		++const_r_it2; const_r_it2 = const_r_it1;
	if (!LIBCPP) {
		const_r_it2--; const_r_it2 = const_r_it1;
		--const_r_it2;
	}

	std::cout << "set1 (reversed): ";
	r_it1 = set1.rbegin();
	const_r_it1 = set1.rend();
	std::cout << "(" << r_it1->first << ", " << (*r_it1++).second << ")";
	for ( ; r_it1 != const_r_it1; ++r_it1)
		std::cout << ", (" << r_it1->first << ", " << r_it1->second << ")";
	std::cout << std::endl;

	if (const_r_it1 != r_it1)
		std::cout << "Error!\n" << std::endl;

	std::cout << "set2 (reversed): ";
	const_r_it1 = set2.rbegin();
	std::cout << "(" << const_r_it1->first << ", "
		<< (*const_r_it1++).second << ")";
	for ( ; const_r_it1 != set2.rend(); ++const_r_it1)
		std::cout << ", (" << const_r_it1->first << ", "
			<< const_r_it1->second << ")";
	std::cout << "\n" << std::endl;

	std::cout << "set1: "; print(set1);
	std::cout << "set2: "; print(set2);
	std::cout << std::endl;
}

static void	modifiers__tests(void)
{
	typedef NMSP::set<myPair<int, int> >	set_t;

	NMSP::pair<set_t::iterator, bool>	inserted;
	set_t::iterator						it;
	NMSP::set<myPair<int, int> >		set0;
	NMSP::set<myPair<int, int> >		set1;
	NMSP::set<myPair<int, int> >		set2;

	it = set0.insert(set0.begin(), myPair<int, int>(1, 1));		// 1
	if (it != set0.begin())
		std::cout << "Error!" << std::endl;
	it = set0.insert(set0.end(), myPair<int, int>(1, 1));		// 2
	if (*it != myPair<int, int>(1, 1))
		std::cout << "Error!" << std::endl;
	it = set0.insert(set0.end(), myPair<int, int>(0, 0));		// 3
	if (it != set0.begin())
		std::cout << "Error!" << std::endl;
	it = set0.insert(set0.begin(), myPair<int, int>(0, 0));		// 4
	if (*it != myPair<int, int>(0, 0))
		std::cout << "Error!" << std::endl;
	it = set0.insert(set0.begin(), myPair<int, int>(4, 4));		// 5
	if (it != --set0.end())
		std::cout << "Error!" << std::endl;
	it = --set0.end();
	it = set0.insert(--it, myPair<int, int>(2, 2));				// 6
	if (*it != myPair<int, int>(2, 2))
		std::cout << "Error!" << std::endl;
	it = set0.insert(++set0.begin(), myPair<int, int>(2, 2));	// 7
	if (*it != myPair<int, int>(2, 2))
		std::cout << "Error!" << std::endl;
	it = set0.insert(set0.end(), myPair<int, int>(5, 5));		// 8
	if (it != --set0.end())
		std::cout << "Error!" << std::endl;
	it = set0.insert(--set0.end(), myPair<int, int>(6, 6));		// 9
	if (it != --set0.end())
		std::cout << "Error!" << std::endl;
	it = set0.insert(set0.begin(), myPair<int, int>(-1, -1));	// 10
	if (it != set0.begin())
		std::cout << "Error!" << std::endl;
	it = ++set0.begin();
	it = set0.insert(++it, myPair<int, int>(3, 3));				// 11
	if (*it != myPair<int, int>(3, 3))
		std::cout << "Error!" << std::endl;
	it = set0.insert(--set0.end(), myPair<int, int>(7, 7));		// 12
	if (it != --set0.end())
		std::cout << "Error!" << std::endl;

	std::cout << "set0: "; print(set0);
	std::cout << std::endl;

	inserted = set1.insert(myPair<int, int>(2, 2));
	if (inserted.first != set1.begin() || inserted.second == false)
		std::cout << "Error!" << std::endl;
	inserted = set1.insert(myPair<int, int>(1, 1));
	if (inserted.first != set1.begin() || inserted.second == false)
		std::cout << "Error!" << std::endl;
	inserted = set1.insert(myPair<int, int>(4, 4));
	if (inserted.first != --set1.end() || inserted.second == false)
		std::cout << "Error!" << std::endl;
	inserted = set1.insert(myPair<int, int>(3, 3));
	if (*inserted.first != myPair<int, int>(3, 3)
			|| inserted.second == false)
		std::cout << "Error!" << std::endl;
	inserted = set1.insert(myPair<int, int>(3, 3));
	if (*inserted.first != myPair<int, int>(3, 3)
			|| inserted.second == true)
		std::cout << "Error!" << std::endl;

	std::cout << "set1: "; print(set1);
	std::cout << std::endl;

	std::cout << "set2.insert(set0.end(), set0.end())\n";
	set2.insert(set0.end(), set0.end());
	std::cout << "set2: "; print(set2);
	std::cout << "set2.insert(set0.begin(), ++set0.begin())\n";
	set2.insert(set0.begin(), ++set0.begin());
	std::cout << "set2: "; print(set2);
	std::cout << "set2.insert(set0.begin(), set0.end())\n";
	set2.insert(set0.begin(), set0.end());
	std::cout << "set2: "; print(set2);
	std::cout << std::endl;

	std::cout << "set2 = set1;" << std::endl; set2 = set1;
	std::cout << "set2.erase(set2.begin(), set2.end());" << std::endl;
	set2.erase(set2.begin(), set2.end());
	std::cout << "set2: "; print(set2);
	std::cout << std::endl;

	std::cout << "set2 = set1;" << std::endl; set2 = set1;
	std::cout << "set2.erase(set2.begin()); (x2)" << std::endl;
	set2.erase(set2.begin()); set2.erase(set2.begin());
	std::cout << "set2: "; print(set2);
	std::cout << std::endl;

	std::cout << "set2.swap(set1); swap(set1, set2);" << std::endl;
	set2.swap(set1); swap(set1, set2);
	std::cout << "set2 = set1;" << std::endl; set2 = set1;
//	std::cout << "set2.erase(set2.end());" << std::endl;	// Should crash!
//	set2.erase(set2.end());	// Should crash!
	std::cout << "set2.erase(--set2.end());" << std::endl;
	set2.erase(--set2.end());
	std::cout << "set2.erase(4) = " << set2.erase(myPair<int, int>(4, 4))
		<< std::endl;
	std::cout << "set2.erase(2) = " << set2.erase(myPair<int, int>(2, 2))
		<< std::endl;
	std::cout << "set2: "; print(set2);
	std::cout << std::endl;

	std::cout << "set2.erase(3) = " << set2.erase(myPair<int, int>(3, 3))
		<< std::endl;
	std::cout << "set2.erase(1) = " << set2.erase(myPair<int, int>(1, 1))
		<< std::endl;
	std::cout << "set2.erase(1) = " << set2.erase(myPair<int, int>(1, 1))
		<< std::endl;
	std::cout << "set2: "; print(set2);
	std::cout << std::endl;

	std::cout << "set2.swap(set1); (x2); swap(set1, set2);" << std::endl;
	set2.swap(set1); set2.swap(set1); swap(set1, set2);
	std::cout << "set2: "; print(set2);
	std::cout << std::endl;

	std::cout << "set2.clear();" << std::endl;
	set2.clear();
	std::cout << "set2: "; print(set2);
	std::cout << std::endl;
}

static void	search__tests(void)
{
	typedef NMSP::set<int>		set_t;

	set_t::iterator									it;
	NMSP::pair<set_t::iterator, set_t::iterator>	eq_range;
	NMSP::set<int>									set1;
	set1.insert(2);
	set1.insert(1);
	set1.insert(4);
	set1.insert(3);
	const NMSP::set<int>							set2(set1);

	std::cout << "set1: "; print(set1);
	std::cout << std::endl;
	std::cout << "const set2(set1): "; print(set2);
	std::cout << std::endl;

	std::cout << "set1.find(5): ";
	if ((it = set1.find(5)) != set1.end()) std::cout << *it;
	std::cout << std::endl;
	std::cout << "set2.find(0): ";
	if ((it = set2.find(0)) != set2.end()) std::cout << *it;
	std::cout << std::endl;
	std::cout << "set1.find(4): ";
	if ((it = set1.find(4)) != set1.end()) std::cout << *it;
	std::cout << std::endl;
	std::cout << "set2.find(1): ";
	if ((it = set2.find(1)) != set2.end()) std::cout << *it;
	std::cout << "\n" << std::endl;

	std::cout << "set1.count(1): " << set1.count(1) << std::endl;
	std::cout << "set1.count(0): " << set1.count(0) << std::endl;
	std::cout << "set2.count(4): " << set2.count(4) << std::endl;
	std::cout << "set2.count(5): " << set2.count(5) << std::endl;
	std::cout << std::endl;

	std::cout << "set1.lower_bound(5): ";
	if ((it = set1.lower_bound(5)) != set1.end())
		std::cout << *it;
	std::cout << std::endl;
	std::cout << "set2.lower_bound(0): ";
	if ((it = set2.lower_bound(0)) != set2.end())
		std::cout << *it;
	std::cout << std::endl;
	std::cout << "set1.lower_bound(4): ";
	if ((it = set1.lower_bound(4)) != set1.end())
		std::cout << *it;
	std::cout << std::endl;
	std::cout << "set2.lower_bound(1): ";
	if ((it = set2.lower_bound(1)) != set2.end())
		std::cout << *it;
	std::cout << "\n" << std::endl;

	std::cout << "set1.upper_bound(5): ";
	if ((it = set1.upper_bound(5)) != set1.end())
		std::cout << *it;
	std::cout << std::endl;
	std::cout << "set2.upper_bound(0): ";
	if ((it = set2.upper_bound(0)) != set2.end())
		std::cout << *it;
	std::cout << std::endl;
	std::cout << "set1.upper_bound(4): ";
	if ((it = set1.upper_bound(4)) != set1.end())
		std::cout << *it;
	std::cout << std::endl;
	std::cout << "set2.upper_bound(1): ";
	if ((it = set2.upper_bound(1)) != set2.end())
		std::cout << *it;
	std::cout << "\n" << std::endl;

	eq_range = set1.equal_range(5);
	std::cout << "set1.equal_range(5): ( ";
	if (eq_range.first != set1.end())
		std::cout << *eq_range.first;
	std::cout << ", ";
	if (eq_range.second != set1.end())
		std::cout << *eq_range.second;
	std::cout << " )" << std::endl;
	eq_range = set2.equal_range(0);
	std::cout << "set2.equal_range(0): (" << *eq_range.first
		<< ", " << *eq_range.second << ")" << std::endl;
	eq_range = set1.equal_range(4);
	std::cout << "set1.equal_range(4): (" << *eq_range.first << ", ";
	if (eq_range.second != set1.end())
		std::cout << *eq_range.second;
	std::cout << " )" << std::endl;
	eq_range = set2.equal_range(1);
	std::cout << "set2.equal_range(1): (" << *eq_range.first
		<< ", " << *eq_range.second << ")\n" << std::endl;

//	it = set1.begin();
//	*it = 2; // Shouldn't compile!
}

static void	relational_operators__tests(void)
{
	NMSP::set<int>			set1;
	set1.insert(2);
	set1.insert(1);
	set1.insert(4);
	set1.insert(3);
	const NMSP::set<int>	set2(set1);

	std::cout << "set1: "; print(set1);
	std::cout << "set2: "; print(set2);
	std::cout << std::endl;

	std::cout << std::boolalpha;
	std::cout << "set1 == set2: " << (set1 == set2) << std::endl;
	std::cout << "set1 != set2: " << (set1 != set2) << std::endl;
	std::cout << "set1 <= set2: " << (set1 <= set2) << std::endl;
	std::cout << "set1 >= set2: " << (set1 >= set2) << "\n" << std::endl;

	std::cout << "set1.erase(2);" << std::endl; set1.erase(2);
	std::cout << "set1 < set2: " << (set1 < set2) << std::endl;
	std::cout << "set1 > set2: " << (set1 > set2) << "\n" << std::endl;

	std::cout << "set1.insert(0);" << std::endl; set1.insert(0);
	std::cout << "set1 <= set2: " << (set1 <= set2) << std::endl;
	std::cout << "set1 >= set2: " << (set1 >= set2) << std::endl;
	std::cout << std::noboolalpha << std::endl;
}

static void	speed__tests(void)
{
	NMSP::set<int>				set1;
	NMSP::set<int>				set2;
	NMSP::set<int>::iterator	it;
	int							i = 0;

	for ( ; i < SPEED_SET; i += 2)
		set1.insert(set1.begin(), i);
	for ( ; i < SPEED_SET + 500000; i += 2)
		it = set1.insert(set1.end(), i);
	for ( ; i < SPEED_SET + 1000000; i += 2)
		it = set1.insert(it, i);
	i = 1;
	for ( ; i < SPEED_SET + 1000000; i += 2)
		set1.insert(i);
	set1.insert(set2.begin(), set2.end());
	while(!set1.empty())
		set1.erase(i--);
	set1.insert(set2.begin(), set2.end());
	set1.erase(set1.begin(), set1.end());
	while(!set2.empty())
		set2.erase(set2.begin());
}

void	set__tests(bool testSpeed)
{
	typedef__tests();
	std::cout << success("typedefs") << std::endl;
	constructors_destructors__tests();
	std::cout << success("constructors & destructors") << std::endl;
	member_operators__tests();
	std::cout << success("member operators") << std::endl;
	element_access__tests();
	std::cout << success("element access functions") << std::endl;
	capacity__tests();
	std::cout << success("capacity functions") << std::endl;
	iterators__tests();
	std::cout << success("iterators") << std::endl;
	modifiers__tests();
	std::cout << success("modifier functions") << std::endl;
	search__tests();
	std::cout << success("search functions") << std::endl;
	relational_operators__tests();
	std::cout << success("relational operators") << std::endl;
	if (testSpeed) {
		speed__tests();
		std::cout << success("speed tests") << std::endl;
	}
}
