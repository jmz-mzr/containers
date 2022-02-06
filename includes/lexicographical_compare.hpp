#ifndef LEXICOGRAPHICAL_COMPARE_HPP
# define LEXICOGRAPHICAL_COMPARE_HPP

namespace	ft
{
	template <typename InputIt1, typename InputIt2>
	bool	lexicographical_compare(InputIt1 first1, InputIt1 last1,
									InputIt2 first2, InputIt2 last2)
	{
		for ( ; first1 != last1; ++first1, ++first2) {
			if (first2 == last2 || *first2 < *first1)
				return (false);
			if (*first1 < *first2)
				return (true);
		}
		return (first2 != last2);
	}

	template <typename InputIt1, typename InputIt2, class Compare>
	bool	lexicographical_compare(InputIt1 first1, InputIt1 last1,
									InputIt2 first2, InputIt2 last2, Compare cmp)
	{
		for ( ; first1 != last1; ++first1, ++first2) {
			if (first2 == last2 || cmp(*first2, *first1))
				return (false);
			if (cmp(*first1, *first2))
				return (true);
		}
		return (first2 != last2);
	}
}

#endif
