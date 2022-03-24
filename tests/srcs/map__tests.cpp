#include <iostream>
#include <map>
//#include "hackStdStack.hpp"
#include "map.hpp"
//#include "hackFtStack.hpp"
#include "colors.hpp"

#ifndef STD
# define NMSP	ft
#else
# define NMSP	std
#endif

#ifdef __APPLE__
# define SPEED_STRLEN		7500
# define SPEED_VEC_SIZE		3000
#elif defined(__linux__)
# define SPEED_STRLEN		5000
# define SPEED_VEC_SIZE		2000
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

/******************************************************************************/
/*                                   TESTS                                    */
/******************************************************************************/

static void	speed__tests(void)
{
}

void	map__tests(bool testSpeed)
{
	if (testSpeed) {
		speed__tests();
		std::cout << success("speed tests") << std::endl;
	}
}
