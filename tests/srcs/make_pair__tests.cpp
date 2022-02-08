#include <iostream>
#include "make_pair.hpp"

#define HEADER1		"*****************************************************\n*"
#define HEADER2		"*\n*****************************************************\n"
#define SPACE15		"               "

void	make_pair__tests(void)
{
	const ft::pair<const std::string, const float>
									product1(ft::make_pair("Pineapples", 7.8f));
	ft::pair<const char*, float>	product2(ft::make_pair("Lemons", 4.2f));
	const ft::pair<const std::string, const float>	
									product3(ft::make_pair("fsdsf", 5));
	const ft::pair<std::string, float>
									product4(product3);
	ft::pair<std::string, float>	array[4] = {product1, product2,
												product3, product4};

	std::cout << HEADER1 << SPACE15 << "      MAKE_PAIR      "
		<< SPACE15 << HEADER2 << std::endl;

	array[2].first = "Bananas";
	array[3].first = "Mangos";
	array[3].second = 7.4f;
	for (int i = 0; i < 4; ++i)
		std::cout << array[i].first << " ($" << array[i].second << ")"
			<< std::endl;
}
