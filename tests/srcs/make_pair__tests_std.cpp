#include <iostream>
#include <utility>

#define HEADER1		"*****************************************************\n*"
#define HEADER2		"*\n*****************************************************\n"
#define SPACE15		"               "

void	make_pair__tests_std(void)
{
	const std::pair<const std::string, const float>
									product1(std::make_pair("Pineapples", 7.8f));
	std::pair<const char*, float>	product2(std::make_pair("Lemons", 4.2f));
	const std::pair<const std::string, const float>	
									product3(std::make_pair("fsdsf", 5));
	const std::pair<std::string, float>
									product4(product3);
	std::pair<std::string, float>	array[4] = {product1, product2,
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
