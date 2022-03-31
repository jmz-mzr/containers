/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pair__tests.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmazoyer <jmazoyer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/31 14:37:37 by jmazoyer          #+#    #+#             */
/*   Updated: 2022/03/31 14:37:37 by jmazoyer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <utility>
#include "pair.hpp"

#ifndef STD
# define NMSP	ft
#else
# define NMSP	std
#endif

/******************************************************************************/
/*                                   TESTS                                    */
/******************************************************************************/

void	pair__tests(void)
{
	const std::pair<const std::string,
		const float>							product0("Hello!", 0.f);
	NMSP::pair<std::string, float>				product1;
	NMSP::pair<std::string, float>				product1bis;
	NMSP::pair<NMSP::pair<std::string, float>::first_type, NMSP::pair
		<std::string,float>::second_type>		product2("Apples", 3.5);
	NMSP::pair<std::string, float>				product3(NMSP::pair<const char*,
															int>("Pears", 4));

	std::cout << std::boolalpha;
	product1 = NMSP::pair<std::string, float>("Oranges", 3.9);
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
