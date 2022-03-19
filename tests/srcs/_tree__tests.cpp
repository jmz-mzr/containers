#include <iostream>
#include <functional>
#include <memory>
#include <iostream>
#include <functional>
//#include <__tree>
#include "_tree.hpp"
#include "colors.hpp"
#include "pair.hpp"
#include "make_pair.hpp"

/******************************************************************************/
/*                            TEMPLATES / FUNCTIONS                           */
/******************************************************************************/

void	printHeader(const std::string& str)
{
	int				nbStars = 78 - (str.size() % 2);
	std::string		stars(nbStars, '*');
	int				nbSpaces = nbStars - 2 - str.size();
	std::string		spaces(nbSpaces / 2, ' ');
	static bool		addNewline = false;

	if (addNewline)
		std::cout << "\n";
	else
		addNewline = true;
	std::cout << B_CYAN << stars << "\n*"
		<< NO_COLOR << spaces << B_GREEN << str << NO_COLOR << spaces
		<< B_CYAN << "*\n" << stars << NO_COLOR << "\n" << std::endl;
}

std::string	success(const char* str)
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

void	printSuccess(void)
{
	std::cout << B_GREEN << "[OK]" << NO_COLOR
		<< " All tests ran successfully!" << std::endl;
}

/*
** Prints the expected result in gray
*/

std::string	expect(const char* str)
{
	std::string	output(GRAY);

	output += str;
	output += NO_COLOR;
	return (output);
}

/*
** Create a type that cannot be default constructed to test if the _tree
** can still be created
*/

class	priv {
	priv(void);
};

/*
** Allow ft::pair to be used with "operator<<"
*/

template <typename T1, typename T2>
struct	myPair: public ft::pair<T1, T2>
{
	myPair(void): ft::pair<T1, T2>() { }
	myPair(const T1& x, const T2& y): ft::pair<T1, T2>(x, y) { }
};

template <typename T1, typename T2>
std::ostream&	operator<<(std::ostream& os, const myPair<T1, T2>& pair)
{
	os << "(" << pair.first << ", " << pair.second << ")";
	return (os);
}

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

void	constructors_destructors__tests(void)
{
	ft::_tree<priv, std::less<priv>, myAlloc<priv> >		tree0;
	ft::_tree<int, std::less<int>, std::allocator<int> >	tree1
														((std::less<int>()));
	ft::_tree<int, std::less<int>, myAlloc<int> >			tree2
														((std::less<int>()),
														myAlloc<int>());
	tree2.insert_unique(1);
	tree2.insert_unique(2);
	tree2.insert_unique(4);
	tree2.insert_unique(3);
	ft::_tree<int, std::less<int>, myAlloc<int> >			tree3(tree2);

	std::cout << "tree2:" << expect(" [2 | 1, 4 | 3]") << std::endl;
	tree2.print(); std::cout << std::endl;
	std::cout << "tree3(tree2):" << expect(" [same]") << std::endl;
	tree3.print(); std::cout << std::endl;
}

void	member_operators__tests(void)
{
	ft::_tree<std::string, std::less<std::string>, std::allocator<std::string> >
															tree0;
	ft::_tree<std::string, std::less<std::string>, std::allocator<std::string> >
															tree1;
	tree1.insert_unique("02");
	tree1.insert_unique("01");
	tree1.insert_unique("00");
	ft::_tree<std::string, std::less<std::string>, std::allocator<std::string> >
															tree2(tree1);

	std::cout << "tree1:" << expect(" [1 | 0, 2]") << std::endl;
	tree1.print(); std::cout << std::endl;
	std::cout << "tree2(tree1):" << expect(" [same]") << std::endl;
	tree2.print(); std::cout << std::endl;
	std::cout << "tree0:" << expect(" [empty]") << "\n" << std::endl;
	std::cout << "tree2 = tree0:" << expect(" [same]") << "\n" << std::endl;
}

void	element_access__tests(void)
{
	const ft::_tree<int, std::less<int>, std::allocator<int> >	tree0;
	ft::_tree<int, std::less<int>, std::allocator<int> >		tree1;
	std::allocator<int>											alloc0
														(tree0.node_alloc());
	std::allocator<int>											alloc1
														(tree1.node_alloc());
	std::allocator<int>											alloc2
														(tree0.alloc());
	std::allocator<int>											alloc3
														(tree1.alloc());
	int*														ptr;

	std::cout << std::boolalpha;
	std::cout << "tree0.value_comp()(0, 1): "
		<< tree0.value_comp()(0, 1) << expect("  [true]") << std::endl;
	std::cout << "tree0.value_comp()(1, 1): "
		<< tree0.value_comp()(1, 1) << expect(" [false]") << std::endl;
	std::cout << "tree1.value_comp()(0, 1): "
		<< tree1.value_comp()(0, 1) << expect("  [true]") << std::endl;
	std::cout << "tree1.value_comp()(2, 1): "
		<< tree1.value_comp()(2, 1) << expect(" [false]") << std::endl;
	std::cout << std::endl;

	ptr = alloc0.allocate(1);
	alloc0.deallocate(ptr, 1);
	ptr = alloc1.allocate(1);
	alloc1.deallocate(ptr, 1);
	ptr = alloc2.allocate(1);
	alloc2.deallocate(ptr, 1);
	ptr = alloc3.allocate(1);
	alloc3.deallocate(ptr, 1);

	tree1.insert_unique(1);
	tree1.insert_unique(3);
	tree1.insert_unique(2);
	std::cout << "tree0.root() == tree0.end_node()->left: "
		<< (tree0.root() == tree0.end_node()->left) << expect(" [true]")
		<< std::endl;
	std::cout << "tree0.root() = "
		<< (!tree0.root() ? "NULL" : "Error!") << expect(" [NULL]")
		<< std::endl;
	std::cout << "tree1.root() == tree1.end_node()->left: "
		<< (tree1.root() == tree1.end_node()->left) << expect(" [true]")
		<< std::noboolalpha << std::endl;
	std::cout << "tree1.root() = " << tree1.root()->data
		<< expect(" [2]") << "\n" << std::endl;
}

void	capacity__tests(void)
{
	const ft::_tree<int, std::less<int>, std::allocator<int> >	tree1;
	ft::_tree<int, std::less<int>, std::allocator<int> >		tree2;

	std::cout << "tree1:" << expect(" [empty]") << std::endl;
	tree1.print(); std::cout << std::endl;
	std::cout << "tree1.size() = " << tree1.size() << expect(" [0]")
		<< ", tree1.max_size() = " << tree1.max_size() << "\n" << std::endl;

	tree2.insert_unique(1);
	tree2.insert_unique(-3);
	tree2.insert_unique(-2);
	std::cout << "tree2:" << expect(" [-2 | -3, 1]") << std::endl;
	tree2.print();
	std::cout << "tree2.size() = " << tree2.size() << expect(" [3]")
		<< ", tree2.max_size() = " << tree2.max_size() << "\n" << std::endl;

	std::cout << "tree2.erase_unique(-3);"; tree2.erase_unique(-3);
	std::cout << "tree2:" << expect(" [-2 | 1]") << std::endl;
	tree2.print();
	std::cout << "tree2.size() = " << tree2.size() << expect(" [2]")
		<< "\n" << std::endl;

	std::cout << "tree2 = tree1;" << std::endl; tree2 = tree1;
	std::cout << "tree2:" << expect(" [empty]") << std::endl;
	tree2.print();
	std::cout << "tree2.size() = " << tree2.size() << expect(" [0]")
		<< "\n" << std::endl;
}

void	iterators__tests(void)
{
	typedef myPair<int, int>		pair_t;
	typedef ft::_tree<pair_t, std::less<pair_t>, std::allocator<pair_t> >
									tree_t;

	tree_t							tree1;
	tree1.insert_unique(pair_t(1, 1));
	tree1.insert_unique(pair_t(2, 2));
	const tree_t					tree2(tree1);
	tree_t::iterator				it0;
	tree_t::const_iterator			const_it0;
	tree_t::iterator				it1(tree1.end_node());
	tree_t::const_iterator			const_it1(tree2.end_node());
	tree_t::iterator				it2(it0);
	tree_t::const_iterator			const_it2(it0);
	tree_t::const_iterator			const_it2_2(const_it0);

	it2 = it1;
	const_it2 = it1;
	const_it2_2 = const_it1;
	it2++; it2 = it1; ++it2; it2 = it1; it2--; it2 = it1; --it2;
	const_it2++; const_it2 = const_it1; ++const_it2; const_it2 = const_it1;
	const_it2--; const_it2 = const_it1; --const_it2;

	std::cout << "tree1: ";
	it1 = tree1.begin();
	std::cout << GRAY << "[" << (*it1++).first;
	for ( ; it1 != tree1.end(); ++it1)
		std::cout << " | " << it1->second;
	std::cout << "]" << NO_COLOR << std::endl;
	tree1.print(); std::cout << std::endl;

	std::cout << "tree2: ";
	const_it2 = tree2.begin();
	std::cout << GRAY << "[" << (*const_it2++).first;
	for ( ; const_it2 != tree2.end(); ++const_it2)
		std::cout << " | " << const_it2->second;
	std::cout << "]" << NO_COLOR << std::endl;
	tree2.print(); std::cout << std::endl;
}

void	modifiers__tests(void)
{
	typedef ft::_tree<int, std::less<int>, std::allocator<int> >
															tree_t;

	ft::pair<tree_t::iterator, bool>						inserted;
	tree_t::iterator										it;
	ft::_tree<int, std::less<int>, std::allocator<int> >	tree1;
	ft::_tree<int, std::less<int>, std::allocator<int> >	tree2;

	inserted = tree1.insert_unique(2);
	if (inserted.first != tree1.begin() || inserted.second == false)
		std::cout << "Error!" << std::endl;
	inserted = tree1.insert_unique(1);
	if (inserted.first != tree1.begin() || inserted.second == false)
		std::cout << "Error!" << std::endl;
	inserted = tree1.insert_unique(4);
	if (inserted.first != --tree1.end() || inserted.second == false)
		std::cout << "Error!" << std::endl;
	inserted = tree1.insert_unique(3);
	if (*inserted.first != 3 || inserted.second == false)
		std::cout << "Error!" << std::endl;
	inserted = tree1.insert_unique(3);
	if (*inserted.first != 3 || inserted.second == true)
		std::cout << "Error!" << std::endl;

	tree1.clear();
	it = tree1.insert_unique(tree1.begin(), 2);
	if (it != tree1.begin())
		std::cout << "Error!" << std::endl;
	it = tree1.insert_unique(tree1.end(), 1);
	if (it != tree1.begin())
		std::cout << "Error!" << std::endl;
	it = tree1.insert_unique(tree1.begin(), 4);
	if (it != --tree1.end())
		std::cout << "Error!" << std::endl;
	it = tree1.insert_unique(tree1.begin(), 3);
	if (*it != 3)
		std::cout << "Error!" << std::endl;
	it = tree1.insert_unique(++tree1.begin(), 3);
	if (*it != 3)
		std::cout << "Error!" << std::endl;

	std::cout << "tree1:" << expect(" [2 | 1, 4 | 3]") << std::endl;
	tree1.print(); std::cout << std::endl;

	std::cout << "tree2 = tree1;" << std::endl; tree2 = tree1;
	std::cout << "tree2.erase(tree2.begin(), tree2.end());" << std::endl;
	tree2.erase(tree2.begin(), tree2.end());
	std::cout << "tree2:" << expect(" [empty]") << std::endl;
	tree2.print(); std::cout << std::endl;

	std::cout << "tree2 = tree1;" << std::endl; tree2 = tree1;
	std::cout << "tree2.erase(tree2.begin()); (x2)" << std::endl;
	tree2.erase(tree2.begin()); tree2.erase(tree2.begin());
	std::cout << "tree2:" << expect(" [3 | 4]") << std::endl;
	tree2.print(); std::cout << std::endl;

	std::cout << "tree2.swap(tree1); (x2)" << std::endl;
	tree2.swap(tree1); tree2.swap(tree1);
	std::cout << "tree2 = tree1;" << std::endl; tree2 = tree1;
//	std::cout << "tree2.erase(tree2.end());" << std::endl;	// Should crash!
//	tree2.erase(tree2.end());	// Should crash!
	std::cout << "tree2.erase(--tree2.end());" << std::endl;
	tree2.erase(--tree2.end());
	std::cout << "tree2.erase_unique(4) = "
		<< tree2.erase_unique(4) << expect(" [0]") << std::endl;
	std::cout << "tree2.erase_unique(2) = "
		<< tree2.erase_unique(2) << expect(" [1]") << std::endl;
	std::cout << "tree2:" << expect(" [3 | 1]") << std::endl;
	tree2.print(); std::cout << std::endl;

	std::cout << "tree2.erase_unique(3) = "
		<< tree2.erase_unique(3) << expect(" [1]") << std::endl;
	std::cout << "tree2.erase_unique(1) = "
		<< tree2.erase_unique(1) << expect(" [1]") << std::endl;
	std::cout << "tree2.erase_unique(1) = "
		<< tree2.erase_unique(1) << expect(" [0]") << std::endl;
	std::cout << "tree2:" << expect(" [empty]") << std::endl
		<< (tree2.begin() == tree2.end() ? "" : "Error!");
	tree2.print(); std::cout << std::endl;

	std::cout << "tree2.swap(tree1);" << std::endl;
	tree2.swap(tree1);
	std::cout << "tree2:" << expect(" [2 | 1, 4 | 3]") << std::endl;
	tree2.print(); std::cout << std::endl;

	std::cout << "tree2.clear();" << std::endl;
	tree2.clear();
	std::cout << "tree2:" << expect(" [empty]") << std::endl;
	tree2.print(); std::cout << std::endl;
}

void	search__tests(void)
{
	typedef ft::_tree<int, std::less<int>, std::allocator<int> >
																tree_t;

	tree_t::iterator											it;
	tree_t::const_iterator										const_it;
	ft::pair<tree_t::iterator, tree_t::iterator>				eq_range;
	ft::pair<tree_t::const_iterator, tree_t::const_iterator>	const_eq_range;
	ft::_tree<int, std::less<int>, std::allocator<int> >		tree1;
	tree1.insert_unique(2);
	tree1.insert_unique(1);
	tree1.insert_unique(4);
	tree1.insert_unique(3);
	const ft::_tree<int, std::less<int>, std::allocator<int> >	tree2(tree1);

	std::cout << "tree1:" << expect(" [2 | 1, 4 | 3]") << std::endl;
	tree1.print(); std::cout << std::endl;
	std::cout << "const tree2(tree1):" << expect(" [same]") << std::endl;
	tree2.print(); std::cout << std::endl;

	std::cout << "tree1.find(5): ";
	if ((it = tree1.find(5)) != tree1.end()) std::cout << *it;
	std::cout << expect("  [ ]") << std::endl;
	std::cout << "tree2.find(0): ";
	if ((const_it = tree2.find(0)) != tree2.end()) std::cout << *const_it;
	std::cout << expect("  [ ]") << std::endl;
	std::cout << "tree1.find(4): ";
	if ((it = tree1.find(4)) != tree1.end()) std::cout << *it;
	std::cout << expect(" [4]") << std::endl;
	std::cout << "tree2.find(1): ";
	if ((const_it = tree2.find(1)) != tree2.end()) std::cout << *const_it;
	std::cout << expect(" [1]") << "\n" << std::endl;

	std::cout << "tree1.count_unique(1): "
		<< tree1.count_unique(1) << expect(" [1]") << std::endl;
	std::cout << "tree1.count_unique(0): "
		<< tree1.count_unique(0) << expect(" [0]") << std::endl;
	std::cout << "tree2.count_unique(4): "
		<< tree2.count_unique(4) << expect(" [1]") << std::endl;
	std::cout << "tree2.count_unique(5): "
		<< tree2.count_unique(5) << expect(" [0]") << "\n" << std::endl;

	std::cout << "tree1.lower_bound(5): ";
	if ((it = tree1.lower_bound(5)) != tree1.end())
		std::cout << *it;
	std::cout << expect("  [ ]") << std::endl;
	std::cout << "tree2.lower_bound(0): ";
	if ((const_it = tree2.lower_bound(0)) != tree2.end())
		std::cout << *const_it;
	std::cout << expect(" [1]") << std::endl;
	std::cout << "tree1.lower_bound(4): ";
	if ((it = tree1.lower_bound(4)) != tree1.end())
		std::cout << *it;
	std::cout << expect(" [4]") << std::endl;
	std::cout << "tree2.lower_bound(1): ";
	if ((const_it = tree2.lower_bound(1)) != tree2.end())
		std::cout << *const_it;
	std::cout << expect(" [1]") << "\n" << std::endl;

	std::cout << "tree1.upper_bound(5): ";
	if ((it = tree1.upper_bound(5)) != tree1.end())
		std::cout << *it;
	std::cout << expect("  [ ]") << std::endl;
	std::cout << "tree2.upper_bound(0): ";
	if ((const_it = tree2.upper_bound(0)) != tree2.end())
		std::cout << *const_it;
	std::cout << expect(" [1]") << std::endl;
	std::cout << "tree1.upper_bound(4): ";
	if ((it = tree1.upper_bound(4)) != tree1.end())
		std::cout << *it;
	std::cout << expect("  [ ]") << std::endl;
	std::cout << "tree2.upper_bound(1): ";
	if ((const_it = tree2.upper_bound(1)) != tree2.end())
		std::cout << *const_it;
	std::cout << expect(" [2]") << "\n" << std::endl;

	eq_range = tree1.equal_range_unique(5);
	std::cout << "tree1.equal_range_unique(5): ( ";
	if (eq_range.first != tree1.end())
		std::cout << *eq_range.first;
	std::cout << ", ";
	if (eq_range.second != tree1.end())
		std::cout << *eq_range.second;
	std::cout << ")"
		<< expect("  [    ]") << std::endl;
	const_eq_range = tree2.equal_range_unique(0);
	std::cout << "tree2.equal_range_unique(0): (" << *const_eq_range.first
		<< ", " << *const_eq_range.second << ")"
		<< expect(" [1, 1]") << std::endl;
	eq_range = tree1.equal_range_unique(4);
	std::cout << "tree1.equal_range_unique(4): (" << *eq_range.first << ", ";
	if (eq_range.second != tree1.end())
		std::cout << *eq_range.second;
	std::cout << " )"
		<< expect(" [4,  ]") << std::endl;
	const_eq_range = tree2.equal_range_unique(1);
	std::cout << "tree2.equal_range_unique(1): (" << *const_eq_range.first
		<< ", " << *const_eq_range.second << ")"
		<< expect(" [1, 2]") << "\n" << std::endl;
}

int	main(void)
{
	printHeader("_TREE");
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
	printSuccess();
	return (0);
}
