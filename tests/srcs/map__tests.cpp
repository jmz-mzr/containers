#include <iostream>
#include <map>
#include <vector>
#include "map.hpp"
#include "colors.hpp"
#include "enable_if.hpp"
#include "pair.hpp"
#include "make_pair.hpp"
#include "detect_system.hpp"

#ifndef STD
# define NMSP	ft
#else
# define NMSP	std
#endif

#ifdef __APPLE__
# define SPEED_STRLEN		400
# define SPEED_VEC_SIZE		300
#elif defined(__linux__)
# define SPEED_STRLEN		300
# define SPEED_VEC_SIZE		200
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

template <typename Key, typename T, typename Comp, typename Alloc>
static void	print(const NMSP::map<Key, T, Comp, Alloc>& map)
{
	typename NMSP::map<Key, T, Comp, Alloc>::const_iterator	it = map.begin();

	while (it != map.end()) {
		std::cout << "(" << it->first << ", " << it->second << ")";
		if (++it != map.end())
			std::cout << ", ";
	}
	std::cout << std::endl;
}

/*
** Create a type that cannot be default constructed to test if the map
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

/*
** Build a heavy class to test the performance
** (the defined values depend on your machine: here my Linux VM cannot
** handle the same tests as my native MacOS)
*/

class	HeavyMap {
public:
	HeavyMap(void): str(SPEED_STRLEN, '-'), strVec(SPEED_VEC_SIZE, str) { }
	HeavyMap(char c): str(SPEED_STRLEN, c), strVec(SPEED_VEC_SIZE, str) { }
private:
	std::string					str;
	std::vector<std::string>	strVec;
};

/******************************************************************************/
/*                                   TESTS                                    */
/******************************************************************************/

static void	typedef__tests(void)
{
	typedef NMSP::map<int, int>					map_t;
	typedef NMSP::map<int, int>::value_compare	comp_t;

	int															x = 0;
	NMSP::pair<const int, int>									myPair;
	std::less<int>												comp;
	std::allocator<NMSP::pair<const int, int> >					alloc;
	ft::enable_if<true, map_t::key_type>::type					a = x;
	ft::enable_if<true, map_t::mapped_type>::type				b = x;
	ft::enable_if<true, map_t::value_type>::type				c = myPair;
	ft::enable_if<true, map_t::key_compare>::type				d = comp;
	ft::enable_if<true, map_t::allocator_type>::type			e = alloc;
	ft::enable_if<true, map_t::reference>::type					f = myPair;
	ft::enable_if<true, map_t::const_reference>::type			g = myPair;
	ft::enable_if<true, map_t::pointer>::type					h;
	ft::enable_if<true, map_t::const_pointer>::type				i;
	ft::enable_if<true, map_t::size_type>::type					j;
	ft::enable_if<true, map_t::difference_type>::type			k;
	ft::enable_if<true, map_t::iterator>::type					l;
	ft::enable_if<true, map_t::const_iterator>::type			m;
	ft::enable_if<true, map_t::reverse_iterator>::type			n;
	ft::enable_if<true, map_t::const_reverse_iterator>::type	o;
	ft::enable_if<true, comp_t::result_type>::type				p = true;
	ft::enable_if<true, comp_t::first_argument_type>::type		q = myPair;
	ft::enable_if<true, comp_t::second_argument_type>::type		r = myPair;

	(void)a; (void)b; (void)c; (void)d; (void)e; (void)f; (void)g; (void)h;
	(void)i; (void)j; (void)k; (void)l; (void)m; (void)n; (void)o; (void)p;
	(void)q; (void)r;
}

void	constructors_destructors__tests(void)
{
//	NMSP::map<int, priv>	/* Shouldn't compile */	map0 = (std::less<int>());
	NMSP::map<int, priv>							map0;
	NMSP::map<int, char, std::less<int> >			map1
													((std::less<int>()));
	NMSP::map<int, char, std::less<int>,
		myAlloc<NMSP::pair<const int, char> > >		map2
													((std::less<int>()),
													myAlloc<int>());
	map2.insert(NMSP::make_pair(1, 'a'));
	map2.insert(NMSP::make_pair(2, 'b'));
	map2.insert(NMSP::make_pair(4, 'd'));
	map2.insert(NMSP::make_pair(3, 'c'));
	NMSP::map<int, char, std::less<int>,
		myAlloc<NMSP::pair<const int, char> > >		map3(map2);
	NMSP::map<int, char>							map4(map3.begin(),
															map3.end());
	NMSP::map<int, char>							map5(map3.begin(),
															map3.end(),
														((std::less<int>())));
	NMSP::map<int, char>							map6(map3.begin(),
															map3.end(),
														((std::less<int>())),
														myAlloc<int>());

	std::cout << "map2: "; print(map2);
	std::cout << "map3(map2): "; print(map3);
	std::cout << "map4: "; print(map4);
	std::cout << "map5: "; print(map5);
	std::cout << "map6: "; print(map6);
	std::cout << std::endl;
}

void	member_operators__tests(void)
{
	NMSP::map<std::string, std::string> 	map0;
	NMSP::map<std::string, std::string>		map1;
	map1.insert(NMSP::make_pair("02", "02"));
	map1.insert(NMSP::make_pair("01", "01"));
	map1.insert(NMSP::make_pair("00", "00"));
	NMSP::map<std::string, std::string> 	map2(map1);

	std::cout << "map1: "; print(map1);
	std::cout << "map2(map1): "; print(map2);
	std::cout << "map0: "; print(map0);
	std::cout << "map2 = map0: "; map2 = map0; print(map2);
	std::cout << std::endl;

	std::cout << "map1[\"00\"]: " << map1["00"] << std::endl;
	std::cout << "map1[\"00\"] = \"0\": " << (map1["00"] = "0") << std::endl;
	std::cout << "map1[\"03\"]: " << map1["03"] << std::endl;
	std::cout << "map1: "; print(map1);
	std::cout << std::endl;
}

void	element_access__tests(void)
{
	const NMSP::map<int, int>					map0;
	NMSP::map<int, int>							map1;
	std::allocator<NMSP::pair<const int, int> >	alloc0(map0.get_allocator());
	std::allocator<NMSP::pair<const int, int> >	alloc1(map1.get_allocator());
	NMSP::pair<int, int>						pair1(0, 1);
	NMSP::pair<int, int>						pair2(1, 1);
	NMSP::pair<const int, int>*					ptr;

	std::cout << std::boolalpha;
	std::cout << "map0.key_comp()(0, 1): " << map0.key_comp()(0, 1)
		<< std::endl;
	std::cout << "map0.key_comp()(1, 1): " << map0.key_comp()(1, 1)
		<< std::endl;
	std::cout << "map1.key_comp()(0, 1): " << map1.key_comp()(0, 1)
		<< std::endl;
	std::cout << "map1.key_comp()(2, 1): " << map1.key_comp()(2, 1)
		<< std::endl;
	std::cout << std::endl;

	std::cout << "map0.value_comp()((0, 1), (1, 1)): "
		<< map0.value_comp()(pair1, pair2) << std::endl;
	pair1 = NMSP::make_pair(1, 1);
	std::cout << "map1.value_comp()((1, 1), (1, 1)): "
		<< map1.value_comp()(pair1, pair2) << std::endl;
	pair1 = NMSP::make_pair(2, 1);
	std::cout << "map0.value_comp()((1, 1), (2, 1)): "
		<< map0.value_comp()(pair2, pair1) << std::endl;
	pair2 = NMSP::make_pair(1, 2);
	std::cout << "map1.value_comp()((2, 1), (1, 2)): "
		<< map1.value_comp()(pair1, pair2) << std::endl;
	std::cout << std::noboolalpha << std::endl;

	ptr = alloc0.allocate(1);
	alloc0.deallocate(ptr, 1);
	ptr = alloc1.allocate(1);
	alloc1.deallocate(ptr, 1);
}

void	capacity__tests(void)
{
	const NMSP::map<int, int>	map1;
	NMSP::map<int, int>			map2;

	std::cout << "map1: "; print(map1);
	std::cout << "map1.size() = " << map1.size()
		<< ", map1.max_size() = " << map1.max_size() << "\n" << std::boolalpha
		<< "map1.empty(): " << map1.empty() << "\n" << std::endl;

	map2.insert(NMSP::make_pair(1, 1));
	map2.insert(NMSP::make_pair(-3, -3));
	map2.insert(NMSP::make_pair(-2, -2));
	std::cout << "map2: "; print(map2);
	std::cout << "map2.size() = " << map2.size()
		<< ", map2.max_size() = " << map2.max_size() << "\n" << std::endl;

	std::cout << "map2.erase(-3);" << std::endl; map2.erase(-3);
	std::cout << "map2: "; print(map2);
	std::cout << "map2.size() = " << map2.size() << "\n" << std::endl;

	std::cout << "map2.empty(): " << map2.empty() << std::endl;
	std::cout << "map2 = map1;" << std::endl; map2 = map1;
	std::cout << "map2: "; print(map2);
	std::cout << "map2.empty(): " << map2.empty() << std::noboolalpha
		<< std::endl;
	std::cout << "map2.size() = " << map2.size() << "\n" << std::endl;
}

void	iterators__tests(void)
{
	NMSP::map<int, int>							map1;
	map1.insert(NMSP::make_pair(1, 1));
	map1.insert(NMSP::make_pair(2, 2));
	const NMSP::map<int, int>					map2(map1);
	NMSP::map<int, int>::iterator				it0;
	NMSP::map<int, int>::const_iterator			const_it0;
	NMSP::map<int, int>::iterator				it1(map1.end());
	NMSP::map<int, int>::const_iterator			const_it1(map2.end());
	NMSP::map<int, int>::iterator				it2(it0);
	NMSP::map<int, int>::const_iterator			const_it2(it0);
	NMSP::map<int, int>::const_iterator			const_it2_2(const_it0);
	NMSP::map<int, int>::reverse_iterator		r_it0;
	NMSP::map<int, int>::const_reverse_iterator	const_r_it0;
	NMSP::map<int, int>::reverse_iterator		r_it1(map1.end());
	NMSP::map<int, int>::const_reverse_iterator	const_r_it1(map2.end());
	NMSP::map<int, int>::reverse_iterator		r_it2(it0);
	NMSP::map<int, int>::const_reverse_iterator	const_r_it2(it0);
	NMSP::map<int, int>::const_reverse_iterator	const_r_it2_2(const_it0);
	r_it2 = r_it0;
	const_r_it2 = r_it0;
	const_r_it2_2 = const_r_it0;

	it0 = map1.begin(); const_it0 = map1.end();
	std::cout << std::boolalpha;
	std::cout << "map1.begin() == map1.end() (const): "
		<< (it0 == const_it0) << std::endl;
	std::cout << "map1.end() (const) == map1.begin(): "
		<< (const_it0 == it0) << std::endl;
	const_it0 = map1.begin(); it0 = map1.end();
	std::cout << "map1.begin() (const) != map1.end(): "
		<< (it0 != const_it0) << std::endl;
	std::cout << "map1.end() != map1.begin() (const): "
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

	std::cout << "map1 (reversed): ";
	r_it1 = map1.rbegin();
	const_r_it1 = map1.rend();
	std::cout << "(" << r_it1->first << ", " << (*r_it1++).second << ")";
	for ( ; r_it1 != const_r_it1; ++r_it1)
		std::cout << ", (" << r_it1->first << ", " << r_it1->second << ")";
	std::cout << std::endl;

	if (const_r_it1 != r_it1)
		std::cout << "Error!\n" << std::endl;

	std::cout << "map2 (reversed): ";
	const_r_it1 = map2.rbegin();
	std::cout << "(" << const_r_it1->first << ", "
		<< (*const_r_it1++).second << ")";
	for ( ; const_r_it1 != map2.rend(); ++const_r_it1)
		std::cout << ", (" << const_r_it1->first << ", "
			<< const_r_it1->second << ")";
	std::cout << "\n" << std::endl;

	std::cout << "map1: "; print(map1);
	std::cout << "map2: "; print(map2);
	std::cout << std::endl;
}

void	modifiers__tests(void)
{
	typedef NMSP::map<int, int>			map_t;

	NMSP::pair<map_t::iterator, bool>	inserted;
	map_t::iterator						it;
	NMSP::map<int, int>					map0;
	NMSP::map<int, int>					map1;
	NMSP::map<int, int>					map2;

	it = map0.insert(map0.begin(), NMSP::pair<const int, int>(1, 1));	// 1
	if (it != map0.begin())
		std::cout << "Error!" << std::endl;
	it = map0.insert(map0.end(), NMSP::pair<const int, int>(1, 1));		// 2
	if (*it != NMSP::pair<const int, int>(1, 1))
		std::cout << "Error!" << std::endl;
	it = map0.insert(map0.end(), NMSP::pair<const int, int>(0, 0));		// 3
	if (it != map0.begin())
		std::cout << "Error!" << std::endl;
	it = map0.insert(map0.begin(), NMSP::pair<const int, int>(0, 0));	// 4
	if (*it != NMSP::pair<const int, int>(0, 0))
		std::cout << "Error!" << std::endl;
	it = map0.insert(map0.begin(), NMSP::pair<const int, int>(4, 4));	// 5
	if (it != --map0.end())
		std::cout << "Error!" << std::endl;
	it = --map0.end();
	it = map0.insert(--it, NMSP::pair<const int, int>(2, 2));			// 6
	if (*it != NMSP::pair<const int, int>(2, 2))
		std::cout << "Error!" << std::endl;
	it = map0.insert(++map0.begin(), NMSP::pair<const int, int>(2, 2));	// 7
	if (*it != NMSP::pair<const int, int>(2, 2))
		std::cout << "Error!" << std::endl;
	it = map0.insert(map0.end(), NMSP::pair<const int, int>(5, 5));		// 8
	if (it != --map0.end())
		std::cout << "Error!" << std::endl;
	it = map0.insert(--map0.end(), NMSP::pair<const int, int>(6, 6));	// 9
	if (it != --map0.end())
		std::cout << "Error!" << std::endl;
	it = map0.insert(map0.begin(), NMSP::pair<const int, int>(-1, -1));	// 10
	if (it != map0.begin())
		std::cout << "Error!" << std::endl;
	it = ++map0.begin();
	it = map0.insert(++it, NMSP::pair<const int, int>(3, 3));			// 11
	if (*it != NMSP::pair<const int, int>(3, 3))
		std::cout << "Error!" << std::endl;
	it = map0.insert(--map0.end(), NMSP::pair<const int, int>(7, 7));	// 12
	if (it != --map0.end())
		std::cout << "Error!" << std::endl;

	std::cout << "map0: "; print(map0);
	std::cout << std::endl;

	inserted = map1.insert(NMSP::pair<const int, int>(2, 2));
	if (inserted.first != map1.begin() || inserted.second == false)
		std::cout << "Error!" << std::endl;
	inserted = map1.insert(NMSP::pair<const int, int>(1, 1));
	if (inserted.first != map1.begin() || inserted.second == false)
		std::cout << "Error!" << std::endl;
	inserted = map1.insert(NMSP::pair<const int, int>(4, 4));
	if (inserted.first != --map1.end() || inserted.second == false)
		std::cout << "Error!" << std::endl;
	inserted = map1.insert(NMSP::pair<const int, int>(3, 3));
	if (*inserted.first != NMSP::pair<const int, int>(3, 3)
			|| inserted.second == false)
		std::cout << "Error!" << std::endl;
	inserted = map1.insert(NMSP::pair<const int, int>(3, 3));
	if (*inserted.first != NMSP::pair<const int, int>(3, 3)
			|| inserted.second == true)
		std::cout << "Error!" << std::endl;

	std::cout << "map1: "; print(map1);
	std::cout << std::endl;

	std::cout << "map2.insert(map0.end(), map0.end())\n";
	map2.insert(map0.end(), map0.end());
	std::cout << "map2: "; print(map2);
	std::cout << "map2.insert(map0.begin(), ++map0.begin())\n";
	map2.insert(map0.begin(), ++map0.begin());
	std::cout << "map2: "; print(map2);
	std::cout << "map2.insert(map0.begin(), map0.end())\n";
	map2.insert(map0.begin(), map0.end());
	std::cout << "map2: "; print(map2);
	std::cout << std::endl;

	std::cout << "map2 = map1;" << std::endl; map2 = map1;
	std::cout << "map2.erase(map2.begin(), map2.end());" << std::endl;
	map2.erase(map2.begin(), map2.end());
	std::cout << "map2: "; print(map2);
	std::cout << std::endl;

	std::cout << "map2 = map1;" << std::endl; map2 = map1;
	std::cout << "map2.erase(map2.begin()); (x2)" << std::endl;
	map2.erase(map2.begin()); map2.erase(map2.begin());
	std::cout << "map2: "; print(map2);
	std::cout << std::endl;

	std::cout << "map2.swap(map1); swap(map1, map2);" << std::endl;
	map2.swap(map1); swap(map1, map2);
	std::cout << "map2 = map1;" << std::endl; map2 = map1;
//	std::cout << "map2.erase(map2.end());" << std::endl;	// Should crash!
//	map2.erase(map2.end());	// Should crash!
	std::cout << "map2.erase(--map2.end());" << std::endl;
	map2.erase(--map2.end());
	std::cout << "map2.erase(4) = " << map2.erase(4) << std::endl;
	std::cout << "map2.erase(2) = " << map2.erase(2) << std::endl;
	std::cout << "map2: "; print(map2);
	std::cout << std::endl;

	std::cout << "map2.erase(3) = " << map2.erase(3) << std::endl;
	std::cout << "map2.erase(1) = " << map2.erase(1) << std::endl;
	std::cout << "map2.erase(1) = " << map2.erase(1) << std::endl;
	std::cout << "map2: "; print(map2);
	std::cout << std::endl;

	std::cout << "map2.swap(map1); (x2); swap(map1, map2);" << std::endl;
	map2.swap(map1); map2.swap(map1); swap(map1, map2);
	std::cout << "map2: "; print(map2);
	std::cout << std::endl;

	std::cout << "map2.clear();" << std::endl;
	map2.clear();
	std::cout << "map2: "; print(map2);
	std::cout << std::endl;
}

void	search__tests(void)
{
	typedef NMSP::map<int, int>		map_t;

	map_t::iterator												it;
	map_t::const_iterator										const_it;
	NMSP::pair<map_t::iterator, map_t::iterator>				eq_range;
	NMSP::pair<map_t::const_iterator, map_t::const_iterator>	const_eq_range;
	NMSP::map<int, int>											map1;
	map1.insert(NMSP::pair<int, int>(2, 2));
	map1.insert(NMSP::pair<int, int>(1, 1));
	map1.insert(NMSP::pair<int, int>(4, 4));
	map1.insert(NMSP::pair<int, int>(3, 3));
	const NMSP::map<int, int>									map2(map1);

	std::cout << "map1: "; print(map1);
	std::cout << std::endl;
	std::cout << "const map2(map1): "; print(map2);
	std::cout << std::endl;

	std::cout << "map1.find(5): ";
	if ((it = map1.find(5)) != map1.end()) std::cout << it->first;
	std::cout << std::endl;
	std::cout << "map2.find(0): ";
	if ((const_it = map2.find(0)) != map2.end()) std::cout << const_it->first;
	std::cout << std::endl;
	std::cout << "map1.find(4): ";
	if ((it = map1.find(4)) != map1.end()) std::cout << it->first;
	std::cout << std::endl;
	std::cout << "map2.find(1): ";
	if ((const_it = map2.find(1)) != map2.end()) std::cout << const_it->first;
	std::cout << "\n" << std::endl;

	std::cout << "map1.count(1): " << map1.count(1) << std::endl;
	std::cout << "map1.count(0): " << map1.count(0) << std::endl;
	std::cout << "map2.count(4): " << map2.count(4) << std::endl;
	std::cout << "map2.count(5): " << map2.count(5) << std::endl;
	std::cout << std::endl;

	std::cout << "map1.lower_bound(5): ";
	if ((it = map1.lower_bound(5)) != map1.end())
		std::cout << it->first;
	std::cout << std::endl;
	std::cout << "map2.lower_bound(0): ";
	if ((const_it = map2.lower_bound(0)) != map2.end())
		std::cout << const_it->first;
	std::cout << std::endl;
	std::cout << "map1.lower_bound(4): ";
	if ((it = map1.lower_bound(4)) != map1.end())
		std::cout << it->first;
	std::cout << std::endl;
	std::cout << "map2.lower_bound(1): ";
	if ((const_it = map2.lower_bound(1)) != map2.end())
		std::cout << const_it->first;
	std::cout << "\n" << std::endl;

	std::cout << "map1.upper_bound(5): ";
	if ((it = map1.upper_bound(5)) != map1.end())
		std::cout << it->first;
	std::cout << std::endl;
	std::cout << "map2.upper_bound(0): ";
	if ((const_it = map2.upper_bound(0)) != map2.end())
		std::cout << const_it->first;
	std::cout << std::endl;
	std::cout << "map1.upper_bound(4): ";
	if ((it = map1.upper_bound(4)) != map1.end())
		std::cout << it->first;
	std::cout << std::endl;
	std::cout << "map2.upper_bound(1): ";
	if ((const_it = map2.upper_bound(1)) != map2.end())
		std::cout << const_it->first;
	std::cout << "\n" << std::endl;

	eq_range = map1.equal_range(5);
	std::cout << "map1.equal_range(5): ( ";
	if (eq_range.first != map1.end())
		std::cout << eq_range.first->first;
	std::cout << ", ";
	if (eq_range.second != map1.end())
		std::cout << eq_range.second->first;
	std::cout << ")" << std::endl;
	const_eq_range = map2.equal_range(0);
	std::cout << "map2.equal_range(0): (" << const_eq_range.first->first
		<< ", " << const_eq_range.second->first << ")" << std::endl;
	eq_range = map1.equal_range(4);
	std::cout << "map1.equal_range(4): (" << eq_range.first->first << ", ";
	if (eq_range.second != map1.end())
		std::cout << eq_range.second->first;
	std::cout << " )" << std::endl;
	const_eq_range = map2.equal_range(1);
	std::cout << "map2.equal_range(1): (" << const_eq_range.first->first
		<< ", " << const_eq_range.second->first << ")\n" << std::endl;
}

static void	relational_operators__tests(void)
{
	NMSP::map<int, int>							map1;
	map1.insert(NMSP::pair<int, int>(2, 2));
	map1.insert(NMSP::pair<int, int>(1, 1));
	map1.insert(NMSP::pair<int, int>(4, 4));
	map1.insert(NMSP::pair<int, int>(3, 3));
	const NMSP::map<int, int>					map2(map1);

	std::cout << "map1: "; print(map1);
	std::cout << "map2: "; print(map2);
	std::cout << std::endl;

	std::cout << std::boolalpha;
	std::cout << "map1 == map2: " << (map1 == map2) << std::endl;
	std::cout << "map1 != map2: " << (map1 != map2) << std::endl;
	std::cout << "map1 <= map2: " << (map1 <= map2) << std::endl;
	std::cout << "map1 >= map2: " << (map1 >= map2) << "\n" << std::endl;

	std::cout << "map1[1] = 2;" << std::endl; map1[1] = 2;
	std::cout << "map1 < map2: " << (map1 < map2) << std::endl;
	std::cout << "map1 > map2: " << (map1 > map2) << "\n" << std::endl;

	std::cout << "map1[0] = 0;" << std::endl; map1[0] = 0;
	std::cout << "map1 <= map2: " << (map1 <= map2) << std::endl;
	std::cout << "map1 >= map2: " << (map1 >= map2) << std::endl;
	std::cout << std::noboolalpha << std::endl;
}

static void	speed__tests(void)
{
	NMSP::map<int, HeavyMap>			map1;
	NMSP::map<int, HeavyMap>			map2;
	HeavyMap							heavy;
	NMSP::map<int, HeavyMap>::iterator	it;
	int									i = 0;

	for ( ; i < 5000; i += 2)
		map1.insert(map1.begin(), NMSP::pair<const int, HeavyMap>(i, heavy));
	for ( ; i < 10000; i += 2)
		it = map1.insert(map1.end(), NMSP::pair<const int, HeavyMap>(i, heavy));
	for ( ; i < 15000; i += 2)
		it = map1.insert(it, NMSP::pair<const int, HeavyMap>(i, heavy));
	i = 1;
	for ( ; i < 15000; i += 2)
		map2[i];
	map1.insert(map2.begin(), map2.end());
	for ( ; !map1.empty(); --i)
		map1.erase(i);
	map2.erase(map2.begin(), map2.end());
}

void	map__tests(bool testSpeed)
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
