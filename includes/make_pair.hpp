/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   make_pair.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmazoyer <jmazoyer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/31 14:37:36 by jmazoyer          #+#    #+#             */
/*   Updated: 2022/03/31 14:37:36 by jmazoyer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MAKE_PAIR_HPP
# define MAKE_PAIR_HPP

# include "pair.hpp"

namespace	ft
{
	template <typename T1, typename T2>
	ft::pair<T1, T2>	make_pair(T1 x, T2 y)
	{
		return (ft::pair<T1, T2>(x, y));
	}
}

#endif
