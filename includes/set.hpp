#ifndef SET_HPP
# define SET_HPP

# include <functional>
# include <memory>
# include "_tree.hpp"
# include "pair.hpp"
# include "equal.hpp"
# include "lexicographical_compare.hpp"

namespace	ft
{
	template <typename Key, class Compare = std::less<Key>,
				class Allocator = std::allocator<Key> >
	class	set {
	public:
		typedef Key										key_type;
		typedef Key										value_type;
		typedef Compare									key_compare;
		typedef Compare									value_compare;
		typedef Allocator								allocator_type;
		typedef typename Allocator::reference			reference;
		typedef typename Allocator::const_reference		const_reference;
		typedef typename Allocator::pointer				pointer;
		typedef typename Allocator::const_pointer		const_pointer;
		typedef typename Allocator::size_type			size_type;
		typedef typename Allocator::difference_type		difference_type;
	private:
		typedef _tree<value_type, value_compare, allocator_type>	_base;
	public:
		typedef typename _base::const_iterator			iterator;
		typedef typename _base::const_iterator			const_iterator;
		typedef ft::reverse_iterator<iterator>			reverse_iterator;
		typedef ft::reverse_iterator<const_iterator>	const_reverse_iterator;

		explicit set(const key_compare& comp = key_compare(),
						const allocator_type& alloc = allocator_type()):
										_tree(comp, alloc) { }
		set(const set& src);
		template <typename InputIt>
		set(InputIt first, InputIt last,
				const key_compare& comp = key_compare(),
				const allocator_type& alloc = allocator_type());
		~set(void) { }

		set&	operator=(const set& rhs);

		iterator				begin(void) { return (_tree.begin()); }
		const_iterator			begin(void) const { return (_tree.begin()); }
		iterator				end(void) { return (_tree.end()); }
		const_iterator			end(void) const { return (_tree.end()); }
		reverse_iterator		rbegin(void)
									{ return (reverse_iterator(end())); }
		const_reverse_iterator	rbegin(void) const
									{ return (const_reverse_iterator(end())); }
		reverse_iterator		rend(void)
									{ return (reverse_iterator(begin())); }
		const_reverse_iterator	rend(void) const { return
											(const_reverse_iterator(begin())); }

		bool			empty(void) const { return (_tree.size() == 0); }
		size_type		size(void) const { return (_tree.size()); }
		size_type		max_size(void) const { return (_tree.max_size()); }

		template <typename InputIt>
		void						insert(InputIt first, InputIt last);
		ft::pair<iterator, bool>	insert(const value_type& value);
		iterator					insert(iterator hint,
											const value_type& value);
		void						erase(iterator pos);
		size_type					erase(const key_type& key);
		void						erase(iterator first, iterator last);
		void						clear(void);
		void						swap(set& other);

		key_compare			key_comp(void) const
									{ return (_tree.value_comp()); }
		value_compare		value_comp(void) const
									{ return (_tree.value_comp()); }
		allocator_type		get_allocator(void) const
									{ return (_tree.alloc()); }

		iterator				find(const key_type& key) const
											{ return (_tree.find(key)); }
		size_type				count(const key_type& key) const
											{ return (_tree.count_unique(key)); }
		iterator				lower_bound(const key_type& key) const
											{ return (_tree.lower_bound(key)); }
		iterator				upper_bound(const key_type& key) const
											{ return (_tree.upper_bound(key)); }
		ft::pair<iterator, iterator>
								equal_range(const key_type& key) const
									{ return (_tree.equal_range_unique(key)); }
	private:
		_base	_tree;
	};

	/**************************************************************************/
	/*                       CONSTRUCTORS / DESTRUCTORS                       */
	/**************************************************************************/

	template <typename Key, class Compare, class Allocator>
	set<Key, Compare, Allocator>::set(const set& src): _tree(src._tree)
	{
		insert(src.begin(), src.end());
	}

	template <typename Key, class Compare, class Allocator>
	template <typename InputIt>
	set<Key, Compare, Allocator>::set(InputIt first, InputIt last,
											const key_compare& comp,
											const allocator_type& alloc):
															_tree(comp, alloc)
	{
		insert(first, last);
	}

	/**************************************************************************/
	/*                        MEMBER OPERATOR OVERLOADS                       */
	/**************************************************************************/

	template <typename Key, class Compare, class Allocator>
	set<Key, Compare, Allocator>&
		set<Key, Compare, Allocator>::operator=(const set& rhs)
	{
		if (this != &rhs)
			_tree = rhs._tree;
		return (*this);
	}

	/**************************************************************************/
	/*                      NON-MEMBER OPERATOR OVERLOADS                     */
	/**************************************************************************/

	template <typename Key, class Compare, class Allocator>
	bool	operator==(const set<Key, Compare, Allocator>& lhs,
						const set<Key, Compare, Allocator>& rhs)
	{
		return (lhs.size() == rhs.size()
					&& ft::equal(lhs.begin(), lhs.end(), rhs.begin()));
	}

	template <typename Key, class Compare, class Allocator>
	bool	operator!=(const set<Key, Compare, Allocator>& lhs,
						const set<Key, Compare, Allocator>& rhs)
	{
		return (!(lhs == rhs));
	}

	template <typename Key, class Compare, class Allocator>
	bool	operator<(const set<Key, Compare, Allocator>& lhs,
						const set<Key, Compare, Allocator>& rhs)
	{
		return (ft::lexicographical_compare(lhs.begin(), lhs.end(),
												rhs.begin(), rhs.end()));
	}

	template <typename Key, class Compare, class Allocator>
	bool	operator<=(const set<Key, Compare, Allocator>& lhs,
						const set<Key, Compare, Allocator>& rhs)
	{
		return (!(rhs < lhs));
	}

	template <typename Key, class Compare, class Allocator>
	bool	operator>(const set<Key, Compare, Allocator>& lhs,
						const set<Key, Compare, Allocator>& rhs)
	{
		return (rhs < lhs);
	}

	template <typename Key, class Compare, class Allocator>
	bool	operator>=(const set<Key, Compare, Allocator>& lhs,
						const set<Key, Compare, Allocator>& rhs)
	{
		return (!(lhs < rhs));
	}

	/**************************************************************************/
	/*                            MEMBER FUNCTIONS                            */
	/**************************************************************************/

	template <typename Key, class Compare, class Allocator>
	template <typename InputIt>
	void	set<Key, Compare, Allocator>::insert(InputIt first, InputIt last)
	{
		for (iterator end = _tree.end(); first != last; ++first)
			insert(end, *first);
	}

	template <typename Key, class Compare, class Allocator>
	ft::pair<typename set<Key, Compare, Allocator>::iterator, bool>
		set<Key, Compare, Allocator>::insert(const value_type& value)
	{
		return (_tree.insert_unique(value));
	}

	template <typename Key, class Compare, class Allocator>
	typename set<Key, Compare, Allocator>::iterator
		set<Key, Compare, Allocator>::insert(iterator hint,
												const value_type& value)
	{
		return (_tree.insert_unique(hint, value));
	}

	template <typename Key, class Compare, class Allocator>
	void	set<Key, Compare, Allocator>::erase(iterator pos)
	{
		_tree.erase(pos);
	}

	template <typename Key, class Compare, class Allocator>
	typename set<Key, Compare, Allocator>::size_type
		set<Key, Compare, Allocator>::erase(const key_type& key)
	{
		return (_tree.erase_unique(key));
	}

	template <typename Key, class Compare, class Allocator>
	void	set<Key, Compare, Allocator>::erase(iterator first,
													iterator last)
	{
		_tree.erase(first, last);
	}

	template <typename Key, class Compare, class Allocator>
	void	set<Key, Compare, Allocator>::clear(void)
	{
		_tree.clear();
	}

	template <typename Key, class Compare, class Allocator>
	void	set<Key, Compare, Allocator>::swap(set& other)
	{
		_tree.swap(other._tree);
	}

	/**************************************************************************/
	/*                          NON-MEMBER FUNCTIONS                          */
	/**************************************************************************/

	template <typename Key, class Compare, class Allocator>
	void	swap(const set<Key, Compare, Allocator>& lhs,
					const set<Key, Compare, Allocator>& rhs)
	{
		lhs.swap(rhs);
	}
}

#endif
