/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   equal.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmazoyer <jmazoyer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/31 14:37:36 by jmazoyer          #+#    #+#             */
/*   Updated: 2022/03/31 14:37:36 by jmazoyer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EQUAL_HPP
# define EQUAL_HPP

namespace	ft
{
	template <typename InputIt1, typename InputIt2>
	bool	equal(InputIt1 first1, InputIt1 last1, InputIt2 first2)
	{
		for ( ; first1 != last1; ++first1, ++first2)
			if (!(*first1 == *first2))
				return (false);
		return (true);
	}

	template <typename InputIt1, typename InputIt2, class BinaryPred>
	bool	equal(InputIt1 first1, InputIt1 last1,
					InputIt2 first2, BinaryPred pred)
	{
		for ( ; first1 != last1; ++first1, ++first2)
			if (!pred(*first1, *first2))
				return (false);
		return (true);
	}
}

#endif
