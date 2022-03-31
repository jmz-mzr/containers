/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hackStdStack.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmazoyer <jmazoyer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/31 14:37:37 by jmazoyer          #+#    #+#             */
/*   Updated: 2022/03/31 14:37:37 by jmazoyer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HACKSTDSTACK_HPP
# define HACKSTDSTACK_HPP

# include <stack>
# include <vector>

namespace	std
{
	struct	uniq: public std::string {
	};

	template<>
	bool	operator==(const stack<std::uniq, std::vector<std::uniq> >&,
						const stack<std::uniq, std::vector<std::uniq> >&)
	{
		std::stack<int>		stack;

		stack.c.push_back(11);
		std::cout << "[stack.c.back() = " << stack.c.back() << "] ";
		return (false);
	}
}

#endif
