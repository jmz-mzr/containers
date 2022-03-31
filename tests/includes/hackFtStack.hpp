/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hackFtStack.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmazoyer <jmazoyer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/31 14:37:37 by jmazoyer          #+#    #+#             */
/*   Updated: 2022/03/31 14:37:37 by jmazoyer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HACKFTSTACK_HPP
# define HACKFTSTACK_HPP

# include "stack.hpp"

namespace	ft
{
	struct	uniq: public std::string {
	};

	template<>
	bool	operator==(const stack<ft::uniq, ft::vector<ft::uniq> >&,
						const stack<ft::uniq, ft::vector<ft::uniq> >&)
	{
		ft::stack<int>		stack;

		stack.c.push_back(11);
		std::cout << "[stack.c.back() = " << stack.c.back() << "] ";
		return (false);
	}
}

#endif
