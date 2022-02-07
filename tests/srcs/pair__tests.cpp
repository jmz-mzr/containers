#include <iostream>
#include "pair.hpp"

#define HEADER1		"*****************************************************\n*"
#define HEADER2		"*\n*****************************************************\n"
#define SPACE15		"               "

void	pair__tests(void)
{
	const std::pair<const std::string,
		const float>				product0("Hello!", 0.f);
	ft::pair<std::string, float>	product1;
	ft::pair<std::string, float>	product1bis;
	ft::pair<ft::pair<std::string, float>::first_type, ft::pair<std::string,
		float>::second_type>		product2("Apples", 3.5);
	ft::pair<std::string, float>	product3(ft::pair<const char*,
															int>("Pears", 4));

	std::cout << HEADER1 << SPACE15 << "        PAIR         "
		<< SPACE15 << HEADER2 << std::endl;

	std::cout << std::boolalpha;
	product1 = ft::pair<std::string, float>("Oranges", 3.9);
	std::cout << product1.first << " ($" << product1.second << ") == "
		<< product1.first << " ($" << product1.second << "): "
		<< (product1 == product1) << std::endl;
	product1bis = product1; product1bis.second = 3.99;
	std::cout << product1.first << " ($" << product1.second << ") != "
		<< product1bis.first << " ($" << product1bis.second << "): "
		<< (product1bis != product1) << std::endl;
	std::cout << product1.first << " ($" << product1.second << ") < "
		<< product1.first << " ($" << product1.second << "): "
		<< (product1 < product1) << std::endl;
	std::cout << product1bis.first << " ($" << product1bis.second << ") < "
		<< product1.first << " ($" << product1.second << "): "
		<< (product1bis < product1) << std::endl;
	std::cout << product1.first << " ($" << product1.second << ") < "
		<< product1bis.first << " ($" << product1bis.second << "): "
		<< (product1 < product1bis) << std::endl;
	std::cout << product1.first << " ($" << product1.second << ") < "
		<< product3.first << " ($" << product3.second << "): "
		<< (product1 < product3) << std::endl;
	std::cout << product1.first << " ($" << product1.second << ") <= "
		<< product2.first << " ($" << product2.second << "): "
		<< (product1 <= product2) << std::endl;
	std::cout << product2.first << " ($" << product2.second << ") <= "
		<< product2.first << " ($" << product2.second << "): "
		<< (product2 <= product2) << std::endl;
	std::cout << product1.first << " ($" << product1.second << ") > "
		<< product1bis.first << " ($" << product1bis.second << "): "
		<< (product1 > product1bis) << std::endl;
	std::cout << product1bis.first << " ($" << product1bis.second << ") >= "
		<< product1.first << " ($" << product1.second << "): "
		<< (product1bis >= product1) << std::endl;
	std::cout << std::noboolalpha;
}
