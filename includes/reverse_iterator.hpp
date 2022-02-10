#ifndef REVERSE_ITERATOR_HPP
# define REVERSE_ITERATOR_HPP

# include <iterator>
# include "iterator_traits.hpp"

namespace	ft
{
	template <typename Iter>
	class	reverse_iterator: public std::iterator<
						typename ft::iterator_traits<Iter>::iterator_category,
						typename ft::iterator_traits<Iter>::value_type,
						typename ft::iterator_traits<Iter>::difference_type,
						typename ft::iterator_traits<Iter>::pointer,
						typename ft::iterator_traits<Iter>::reference>
	{
	public:
		typedef Iter								iterator_type;
		typedef ft::iterator_traits<Iter>			traits;
		typedef typename traits::difference_type	difference_type;
		typedef difference_type						diff_type;
		typedef typename traits::reference			reference;
		typedef typename traits::pointer			pointer;

		reverse_iterator(void): current() { }
		explicit reverse_iterator(Iter it): current(it) { }
		template <typename RevIt>
		reverse_iterator(const reverse_iterator<RevIt>& src):
													current(src.base()) { }

		template <typename RevIt>
		reverse_iterator&	operator=(const reverse_iterator<RevIt>& rhs);

		reference	operator*(void) const { Iter it = current; return (*--it); }
		pointer		operator->(void) const { return (&(operator*())); }
		reference	operator[](diff_type n) const { return (*(*this + n)); }

		reverse_iterator&	operator++(void) { --current; return (*this); }
		reverse_iterator&	operator--(void) { ++current; return (*this); }
		reverse_iterator	operator++(int) { reverse_iterator revIt(*this);
											  --current; return (revIt); }
		reverse_iterator	operator--(int) { reverse_iterator revIt(*this);
											  ++current; return (revIt); }
		reverse_iterator	operator+(diff_type n) const {
									return (reverse_iterator(current - n)); }
		reverse_iterator	operator-(diff_type n) const {
									return (reverse_iterator(current + n)); }
		reverse_iterator&	operator+=(diff_type n) { current -= n;
													  return (*this); }
		reverse_iterator&	operator-=(diff_type n) { current += n;
													  return (*this); }

		Iter	base(void) const { return (current); }
	protected:
		Iter	current;
	};

	template <typename Iter>
	template <typename RevIt>
	reverse_iterator<Iter>&
		reverse_iterator<Iter>::operator=(const reverse_iterator<RevIt>& rhs)
	{
		current = rhs.base();
		return (*this);
	}

	template <typename Iter>
	reverse_iterator<Iter>
		operator+(typename reverse_iterator<Iter>::difference_type n,
					const reverse_iterator<Iter>& revIt)
	{
		return (reverse_iterator<Iter>(revIt.base() - n));
	}
	template <typename Iter>
	typename reverse_iterator<Iter>::difference_type
		operator-(const reverse_iterator<Iter>& lhs,
					const reverse_iterator<Iter>& rhs)
	{
		return (rhs.base() - lhs.base());
	}

	template <typename Iter>
	bool	operator==(const reverse_iterator<Iter>& lhs,
						const reverse_iterator<Iter>& rhs)
	{
		return (lhs.base() == rhs.base());
	}
	template <typename Iter>
	bool	operator!=(const reverse_iterator<Iter>& lhs,
						const reverse_iterator<Iter>& rhs)
	{
		return (lhs.base() != rhs.base());
	}
	template <typename Iter>
	bool	operator<(const reverse_iterator<Iter>& lhs,
						const reverse_iterator<Iter>& rhs)
	{
		return (lhs.base() > rhs.base());
	}
	template <typename Iter>
	bool	operator<=(const reverse_iterator<Iter>& lhs,
						const reverse_iterator<Iter>& rhs)
	{
		return (lhs.base() >= rhs.base());
	}
	template <typename Iter>
	bool	operator>(const reverse_iterator<Iter>& lhs,
						const reverse_iterator<Iter>& rhs)
	{
		return (lhs.base() < rhs.base());
	}
	template <typename Iter>
	bool	operator>=(const reverse_iterator<Iter>& lhs,
						const reverse_iterator<Iter>& rhs)
	{
		return (lhs.base() <= rhs.base());
	}
}

#endif
