#include <iostream>
#include <set>
#include <vector>
#include "set.hpp"
#include "colors.hpp"
#include "enable_if.hpp"
#include "pair.hpp"
#include "detect_system.hpp"

#ifndef STD
# define NMSP	ft
#else
# define NMSP	std
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
** Create a type that cannot be default constructed to test if the set
** can still be created
*/

class	priv {
	priv(void);
};

/*
** Use a custom allocator to test with the _tree class
*/

template <typename T>
class	myAlloc: public std::allocator<T> {
public:
	typedef size_t		size_type;
	typedef T*			pointer;
	typedef const T* 	const_pointer;

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

/******************************************************************************/
/*                                   TESTS                                    */
/******************************************************************************/

static void	typedef__tests(void)
{
	typedef NMSP::set<std::string>					set_t;

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

	(void)a; (void)b; (void)c; (void)d; (void)e; (void)f; (void)g; (void)h;
	(void)i; (void)j; (void)k; (void)l; (void)m; (void)n; (void)o;
}

static void	constructors_destructors__tests(void)
{
//	NMSP::set<int> 		/*Shouldn't compile*/		set00 = (std::less<int>());
	NMSP::set<priv>									set0;
	NMSP::set<int, std::less<int> >					set1((std::less<int>()));
	NMSP::set<int, std::less<int>, myAlloc<int> >	set2
													((std::less<int>()),
													myAlloc<int>());
	set2.insert(1);
	set2.insert(2);
	set2.insert(4);
	set2.insert(3);
	const NMSP::set<int, std::less<int>,
		  						myAlloc<int> >		set3(set2);
	NMSP::set<int>									set4(set3.begin(),
															set3.end());
	NMSP::set<int>									set5(set3.begin(),
															set3.end(),
														((std::less<int>())));
	NMSP::set<int>									set6(set3.begin(),
															set3.end(),
														((std::less<int>())),
														myAlloc<int>());

	std::cout << "set2: "; print(set2);
	std::cout << "set3(set2): "; print(set3);
	std::cout << "set4: "; print(set4);
	std::cout << "set5: "; print(set5);
	std::cout << "set6: "; print(set6);
	std::cout << std::endl;
}
/*
static void	member_operators__tests(void)
{
	NMSP::set<std::string, std::string> 	set0;
	NMSP::set<std::string, std::string>		set1;
	set1.insert(NMSP::make_pair("02", "02"));
	set1.insert(NMSP::make_pair("01", "01"));
	set1.insert(NMSP::make_pair("00", "00"));
	NMSP::set<std::string, std::string> 	set2(set1);

	std::cout << "set1: "; print(set1);
	std::cout << "set2(set1): "; print(set2);
	std::cout << "set0: "; print(set0);
	std::cout << "set2 = set0: "; set2 = set0; print(set2);
	std::cout << std::endl;

	std::cout << "set1[\"00\"]: " << set1["00"] << std::endl;
	std::cout << "set1[\"00\"] = \"0\": " << (set1["00"] = "0") << std::endl;
	std::cout << "set1[\"03\"]: " << set1["03"] << std::endl;
	std::cout << "set1: "; print(set1);
	std::cout << std::endl;
}

static void	element_access__tests(void)
{
	const NMSP::set<int, int>					set0;
	NMSP::set<int, int>							set1;
	std::allocator<NMSP::pair<const int, int> >	alloc0(set0.get_allocator());
	std::allocator<NMSP::pair<const int, int> >	alloc1(set1.get_allocator());
	NMSP::pair<int, int>						pair1(0, 1);
	NMSP::pair<int, int>						pair2(1, 1);
	NMSP::pair<const int, int>*					ptr;

	std::cout << std::boolalpha;
	std::cout << "set0.key_comp()(0, 1): " << set0.key_comp()(0, 1)
		<< std::endl;
	std::cout << "set0.key_comp()(1, 1): " << set0.key_comp()(1, 1)
		<< std::endl;
	std::cout << "set1.key_comp()(0, 1): " << set1.key_comp()(0, 1)
		<< std::endl;
	std::cout << "set1.key_comp()(2, 1): " << set1.key_comp()(2, 1)
		<< std::endl;
	std::cout << std::endl;

	std::cout << "set0.value_comp()((0, 1), (1, 1)): "
		<< set0.value_comp()(pair1, pair2) << std::endl;
	pair1 = NMSP::make_pair(1, 1);
	std::cout << "set1.value_comp()((1, 1), (1, 1)): "
		<< set1.value_comp()(pair1, pair2) << std::endl;
	pair1 = NMSP::make_pair(2, 1);
	std::cout << "set0.value_comp()((1, 1), (2, 1)): "
		<< set0.value_comp()(pair2, pair1) << std::endl;
	pair2 = NMSP::make_pair(1, 2);
	std::cout << "set1.value_comp()((2, 1), (1, 2)): "
		<< set1.value_comp()(pair1, pair2) << std::endl;
	std::cout << std::noboolalpha << std::endl;

	ptr = alloc0.allocate(1);
	alloc0.deallocate(ptr, 1);
	ptr = alloc1.allocate(1);
	alloc1.deallocate(ptr, 1);
}

static void	capacity__tests(void)
{
	const NMSP::set<int, int>	set1;
	NMSP::set<int, int>			set2;

	std::cout << "set1: "; print(set1);
	std::cout << "set1.size() = " << set1.size()
		<< ", set1.max_size() = " << set1.max_size() << "\n" << std::boolalpha
		<< "set1.empty(): " << set1.empty() << "\n" << std::endl;

	set2.insert(NMSP::make_pair(1, 1));
	set2.insert(NMSP::make_pair(-3, -3));
	set2.insert(NMSP::make_pair(-2, -2));
	std::cout << "set2: "; print(set2);
	std::cout << "set2.size() = " << set2.size()
		<< ", set2.max_size() = " << set2.max_size() << "\n" << std::endl;

	std::cout << "set2.erase(-3);" << std::endl; set2.erase(-3);
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
	NMSP::set<int, int>							set1;
	set1.insert(NMSP::make_pair(1, 1));
	set1.insert(NMSP::make_pair(2, 2));
	const NMSP::set<int, int>					set2(set1);
	NMSP::set<int, int>::iterator				it0;
	NMSP::set<int, int>::const_iterator			const_it0;
	NMSP::set<int, int>::iterator				it1(set1.end());
	NMSP::set<int, int>::const_iterator			const_it1(set2.end());
	NMSP::set<int, int>::iterator				it2(it0);
	NMSP::set<int, int>::const_iterator			const_it2(it0);
	NMSP::set<int, int>::const_iterator			const_it2_2(const_it0);
	NMSP::set<int, int>::reverse_iterator		r_it0;
	NMSP::set<int, int>::const_reverse_iterator	const_r_it0;
	NMSP::set<int, int>::reverse_iterator		r_it1(set1.end());
	NMSP::set<int, int>::const_reverse_iterator	const_r_it1(set2.end());
	NMSP::set<int, int>::reverse_iterator		r_it2(it0);
	NMSP::set<int, int>::const_reverse_iterator	const_r_it2(it0);
	NMSP::set<int, int>::const_reverse_iterator	const_r_it2_2(const_it0);
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
	typedef NMSP::set<int, int>			set_t;

	NMSP::pair<set_t::iterator, bool>	inserted;
	set_t::iterator						it;
	NMSP::set<int, int>					set0;
	NMSP::set<int, int>					set1;
	NMSP::set<int, int>					set2;

	it = set0.insert(set0.begin(), NMSP::pair<const int, int>(1, 1));	// 1
	if (it != set0.begin())
		std::cout << "Error!" << std::endl;
	it = set0.insert(set0.end(), NMSP::pair<const int, int>(1, 1));		// 2
	if (*it != NMSP::pair<const int, int>(1, 1))
		std::cout << "Error!" << std::endl;
	it = set0.insert(set0.end(), NMSP::pair<const int, int>(0, 0));		// 3
	if (it != set0.begin())
		std::cout << "Error!" << std::endl;
	it = set0.insert(set0.begin(), NMSP::pair<const int, int>(0, 0));	// 4
	if (*it != NMSP::pair<const int, int>(0, 0))
		std::cout << "Error!" << std::endl;
	it = set0.insert(set0.begin(), NMSP::pair<const int, int>(4, 4));	// 5
	if (it != --set0.end())
		std::cout << "Error!" << std::endl;
	it = --set0.end();
	it = set0.insert(--it, NMSP::pair<const int, int>(2, 2));			// 6
	if (*it != NMSP::pair<const int, int>(2, 2))
		std::cout << "Error!" << std::endl;
	it = set0.insert(++set0.begin(), NMSP::pair<const int, int>(2, 2));	// 7
	if (*it != NMSP::pair<const int, int>(2, 2))
		std::cout << "Error!" << std::endl;
	it = set0.insert(set0.end(), NMSP::pair<const int, int>(5, 5));		// 8
	if (it != --set0.end())
		std::cout << "Error!" << std::endl;
	it = set0.insert(--set0.end(), NMSP::pair<const int, int>(6, 6));	// 9
	if (it != --set0.end())
		std::cout << "Error!" << std::endl;
	it = set0.insert(set0.begin(), NMSP::pair<const int, int>(-1, -1));	// 10
	if (it != set0.begin())
		std::cout << "Error!" << std::endl;
	it = ++set0.begin();
	it = set0.insert(++it, NMSP::pair<const int, int>(3, 3));			// 11
	if (*it != NMSP::pair<const int, int>(3, 3))
		std::cout << "Error!" << std::endl;
	it = set0.insert(--set0.end(), NMSP::pair<const int, int>(7, 7));	// 12
	if (it != --set0.end())
		std::cout << "Error!" << std::endl;

	std::cout << "set0: "; print(set0);
	std::cout << std::endl;

	inserted = set1.insert(NMSP::pair<const int, int>(2, 2));
	if (inserted.first != set1.begin() || inserted.second == false)
		std::cout << "Error!" << std::endl;
	inserted = set1.insert(NMSP::pair<const int, int>(1, 1));
	if (inserted.first != set1.begin() || inserted.second == false)
		std::cout << "Error!" << std::endl;
	inserted = set1.insert(NMSP::pair<const int, int>(4, 4));
	if (inserted.first != --set1.end() || inserted.second == false)
		std::cout << "Error!" << std::endl;
	inserted = set1.insert(NMSP::pair<const int, int>(3, 3));
	if (*inserted.first != NMSP::pair<const int, int>(3, 3)
			|| inserted.second == false)
		std::cout << "Error!" << std::endl;
	inserted = set1.insert(NMSP::pair<const int, int>(3, 3));
	if (*inserted.first != NMSP::pair<const int, int>(3, 3)
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
	std::cout << "set2.erase(4) = " << set2.erase(4) << std::endl;
	std::cout << "set2.erase(2) = " << set2.erase(2) << std::endl;
	std::cout << "set2: "; print(set2);
	std::cout << std::endl;

	std::cout << "set2.erase(3) = " << set2.erase(3) << std::endl;
	std::cout << "set2.erase(1) = " << set2.erase(1) << std::endl;
	std::cout << "set2.erase(1) = " << set2.erase(1) << std::endl;
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
	typedef NMSP::set<int, int>		set_t;

	set_t::iterator												it;
	set_t::const_iterator										const_it;
	NMSP::pair<set_t::iterator, set_t::iterator>				eq_range;
	NMSP::pair<set_t::const_iterator, set_t::const_iterator>	const_eq_range;
	NMSP::set<int, int>											set1;
	set1.insert(NMSP::pair<int, int>(2, 2));
	set1.insert(NMSP::pair<int, int>(1, 1));
	set1.insert(NMSP::pair<int, int>(4, 4));
	set1.insert(NMSP::pair<int, int>(3, 3));
	const NMSP::set<int, int>									set2(set1);

	std::cout << "set1: "; print(set1);
	std::cout << std::endl;
	std::cout << "const set2(set1): "; print(set2);
	std::cout << std::endl;

	std::cout << "set1.find(5): ";
	if ((it = set1.find(5)) != set1.end()) std::cout << it->first;
	std::cout << std::endl;
	std::cout << "set2.find(0): ";
	if ((const_it = set2.find(0)) != set2.end()) std::cout << const_it->first;
	std::cout << std::endl;
	std::cout << "set1.find(4): ";
	if ((it = set1.find(4)) != set1.end()) std::cout << it->first;
	std::cout << std::endl;
	std::cout << "set2.find(1): ";
	if ((const_it = set2.find(1)) != set2.end()) std::cout << const_it->first;
	std::cout << "\n" << std::endl;

	std::cout << "set1.count(1): " << set1.count(1) << std::endl;
	std::cout << "set1.count(0): " << set1.count(0) << std::endl;
	std::cout << "set2.count(4): " << set2.count(4) << std::endl;
	std::cout << "set2.count(5): " << set2.count(5) << std::endl;
	std::cout << std::endl;

	std::cout << "set1.lower_bound(5): ";
	if ((it = set1.lower_bound(5)) != set1.end())
		std::cout << it->first;
	std::cout << std::endl;
	std::cout << "set2.lower_bound(0): ";
	if ((const_it = set2.lower_bound(0)) != set2.end())
		std::cout << const_it->first;
	std::cout << std::endl;
	std::cout << "set1.lower_bound(4): ";
	if ((it = set1.lower_bound(4)) != set1.end())
		std::cout << it->first;
	std::cout << std::endl;
	std::cout << "set2.lower_bound(1): ";
	if ((const_it = set2.lower_bound(1)) != set2.end())
		std::cout << const_it->first;
	std::cout << "\n" << std::endl;

	std::cout << "set1.upper_bound(5): ";
	if ((it = set1.upper_bound(5)) != set1.end())
		std::cout << it->first;
	std::cout << std::endl;
	std::cout << "set2.upper_bound(0): ";
	if ((const_it = set2.upper_bound(0)) != set2.end())
		std::cout << const_it->first;
	std::cout << std::endl;
	std::cout << "set1.upper_bound(4): ";
	if ((it = set1.upper_bound(4)) != set1.end())
		std::cout << it->first;
	std::cout << std::endl;
	std::cout << "set2.upper_bound(1): ";
	if ((const_it = set2.upper_bound(1)) != set2.end())
		std::cout << const_it->first;
	std::cout << "\n" << std::endl;

	eq_range = set1.equal_range(5);
	std::cout << "set1.equal_range(5): ( ";
	if (eq_range.first != set1.end())
		std::cout << eq_range.first->first;
	std::cout << ", ";
	if (eq_range.second != set1.end())
		std::cout << eq_range.second->first;
	std::cout << ")" << std::endl;
	const_eq_range = set2.equal_range(0);
	std::cout << "set2.equal_range(0): (" << const_eq_range.first->first
		<< ", " << const_eq_range.second->first << ")" << std::endl;
	eq_range = set1.equal_range(4);
	std::cout << "set1.equal_range(4): (" << eq_range.first->first << ", ";
	if (eq_range.second != set1.end())
		std::cout << eq_range.second->first;
	std::cout << " )" << std::endl;
	const_eq_range = set2.equal_range(1);
	std::cout << "set2.equal_range(1): (" << const_eq_range.first->first
		<< ", " << const_eq_range.second->first << ")\n" << std::endl;
}

static void	relational_operators__tests(void)
{
	NMSP::set<int, int>							set1;
	set1.insert(NMSP::pair<int, int>(2, 2));
	set1.insert(NMSP::pair<int, int>(1, 1));
	set1.insert(NMSP::pair<int, int>(4, 4));
	set1.insert(NMSP::pair<int, int>(3, 3));
	const NMSP::set<int, int>					set2(set1);

	std::cout << "set1: "; print(set1);
	std::cout << "set2: "; print(set2);
	std::cout << std::endl;

	std::cout << std::boolalpha;
	std::cout << "set1 == set2: " << (set1 == set2) << std::endl;
	std::cout << "set1 != set2: " << (set1 != set2) << std::endl;
	std::cout << "set1 <= set2: " << (set1 <= set2) << std::endl;
	std::cout << "set1 >= set2: " << (set1 >= set2) << "\n" << std::endl;

	std::cout << "set1[1] = 2;" << std::endl; set1[1] = 2;
	std::cout << "set1 < set2: " << (set1 < set2) << std::endl;
	std::cout << "set1 > set2: " << (set1 > set2) << "\n" << std::endl;

	std::cout << "set1[0] = 0;" << std::endl; set1[0] = 0;
	std::cout << "set1 <= set2: " << (set1 <= set2) << std::endl;
	std::cout << "set1 >= set2: " << (set1 >= set2) << std::endl;
	std::cout << std::noboolalpha << std::endl;
}
*/
static void	speed__tests(void)
{
//	NMSP::set<int, Heavyset>			set1;
//	NMSP::set<int, Heavyset>			set2;
//	Heavyset							heavy;
//	NMSP::set<int, Heavyset>::iterator	it;
//	int									i = 0;
//
//	for ( ; i < 5000; i += 2)
//		set1.insert(set1.begin(), NMSP::pair<const int, Heavyset>(i, heavy));
//	for ( ; i < 10000; i += 2)
//		it = set1.insert(set1.end(), NMSP::pair<const int, Heavyset>(i, heavy));
//	for ( ; i < 15000; i += 2)
//		it = set1.insert(it, NMSP::pair<const int, Heavyset>(i, heavy));
//	i = 1;
//	for ( ; i < 15000; i += 2)
//		set2[i];
//	set1.insert(set2.begin(), set2.end());
//	for ( ; !set1.empty(); --i)
//		set1.erase(i);
//	set2.erase(set2.begin(), set2.end());
}

void	set__tests(bool testSpeed)
{
	typedef__tests();
	std::cout << success("typedefs") << std::endl;
	constructors_destructors__tests();
	std::cout << success("constructors & destructors") << std::endl;
/*	member_operators__tests();
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
*/	if (testSpeed) {
		speed__tests();
		std::cout << success("speed tests") << std::endl;
	}
}
