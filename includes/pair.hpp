/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pair.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmazoyer <jmazoyer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/31 14:37:36 by jmazoyer          #+#    #+#             */
/*   Updated: 2022/03/31 14:37:36 by jmazoyer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PAIR_HPP
# define PAIR_HPP

namespace	ft
{
	template <typename T1, typename T2>
	struct	pair {
		typedef T1	first_type;
		typedef T2	second_type;

		pair(void): first(), second() { }
		pair(const T1& x, const T2& y): first(x), second(y) { }
		template <typename U1, typename U2>
		pair(const pair<U1, U2>& p): first(p.first), second(p.second) { }

		T1		first;
		T2		second;
	};

	template <typename T1, typename T2>
	bool	operator==(const pair<T1,T2>& lhs, const pair<T1,T2>& rhs)
	{
		return (lhs.first == rhs.first && lhs.second == rhs.second);
	}
	template <typename T1, typename T2>
	bool	operator!=(const pair<T1,T2>& lhs, const pair<T1,T2>& rhs)
	{
		return (!(lhs == rhs));
	}
	template <typename T1, typename T2>
	bool	operator<(const pair<T1,T2>& lhs, const pair<T1,T2>& rhs)
	{
		return (lhs.first < rhs.first || (!(rhs.first < lhs.first)
					&& lhs.second < rhs.second));
	}
	template <typename T1, typename T2>
	bool	operator<=(const pair<T1,T2>& lhs, const pair<T1,T2>& rhs)
	{
		return (!(rhs < lhs));
	}
	template <typename T1, typename T2>
	bool	operator>(const pair<T1,T2>& lhs, const pair<T1,T2>& rhs)
	{
		return (rhs < lhs);
	}
	template <typename T1, typename T2>
	bool	operator>=(const pair<T1,T2>& lhs, const pair<T1,T2>& rhs)
	{
		return (!(lhs < rhs));
	}
}

#endif
