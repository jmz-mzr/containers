#include <iostream>
#include <utility>
#include "make_pair.hpp"

#ifndef STD
# define NMSP	ft
#else
# define NMSP	std
#endif

/******************************************************************************/
/*                                   TESTS                                    */
/******************************************************************************/

void	make_pair__tests(void)
{
	const NMSP::pair<const std::string, const float>
									prod1(NMSP::make_pair("Pineapples", 7.8f));
	NMSP::pair<const char*, float>	prod2(NMSP::make_pair("Lemons", 4.2f));
	const NMSP::pair<const std::string, const float>	
									prod3(NMSP::make_pair("fsdsf", 5));
	const NMSP::pair<std::string, float>
									prod4(prod3);
	NMSP::pair<std::string, float>	array[4] = {prod1, prod2,
												prod3, prod4};

	array[2].first = "Bananas";
	array[3].first = "Mangos";
	array[3].second = 7.4f;
	for (int i = 0; i < 4; ++i)
		std::cout << array[i].first << " ($" << array[i].second << ")"
			<< std::endl;
}
