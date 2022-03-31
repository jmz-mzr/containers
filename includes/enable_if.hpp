/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   enable_if.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmazoyer <jmazoyer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/31 14:37:36 by jmazoyer          #+#    #+#             */
/*   Updated: 2022/03/31 14:37:36 by jmazoyer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENABLE_IF_HPP
# define ENABLE_IF_HPP

namespace	ft
{
	template <bool B, typename T = void>
	struct	enable_if {
	};

	template <typename T>
	struct enable_if<true, T> {
		typedef T	type;
	};
}

#endif
