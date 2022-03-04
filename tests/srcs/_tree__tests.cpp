#include <iostream>
#include <functional>
#include <memory>
#include "_tree.hpp"

int	main(void)
{
	ft::_tree<int, std::less<int>, std::allocator<int> >	tree;

	tree.insert_unique(1);
	tree.insert_unique(2);
	tree.insert_unique(3);
	tree.insert_unique(4);
	tree.print();
	return (0);
}
