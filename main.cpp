#include <iostream>
#include "enable_if__is_integral__tests.hpp"
#include "equal__tests.hpp"
#include "lexicographical_compare__tests.hpp"
#include "pair__tests.hpp"
#include "make_pair__tests.hpp"

int	main(void)
{
	enable_if__is_integral__tests();
	std::cout << std::endl;
	equal__tests();
	std::cout << std::endl;
	lexicographical_compare__tests();
	std::cout << std::endl;
	pair__tests();
	std::cout << std::endl;
	make_pair__tests();
	return (0);
}
