#ifndef MAP_HPP
# define MAP_HPP

# include <functional>
# include <memory>
# include "_tree.hpp"
# include "pair.hpp"
# include "equal.hpp"
# include "lexicographical_compare.hpp"

namespace	ft
{
	/**************************************************************************/
	/*                            MAP VALUE COMPARE                           */
	/**************************************************************************/

	/*
	** Build a function object allowing the map's "_tree" to directly compare
	** either two "Key"s, two "pair<const Key, T>"s, or a "Key" with a
	** "pair<const Key, T>"'s first element
	*/

	template <typename Key, typename T, class Compare>
	class	_map_value_compare {
	private:
		typedef ft::pair<const Key, T>	_pair;
	public:
		_map_value_compare(void): _comp() { }
		_map_value_compare(Compare c): _comp(c) { }

		const Compare&	key_comp(void) const { return (_comp); }

		bool	operator()(const _pair& lhs, const _pair& rhs) const
									{ return (_comp(lhs.first, rhs.first)); }
		bool	operator()(const _pair& lhs, const Key& rhs) const
									{ return (_comp(lhs.first, rhs)); }
		bool	operator()(const Key& lhs, const _pair& rhs) const
									{ return (_comp(lhs, rhs.first)); }
		bool	operator()(const Key& lhs, const Key& rhs) const
									{ return (_comp(lhs, rhs)); }
	private:
		Compare		_comp;
	};

	/**************************************************************************/
	/*                                MAP CLASS                               */
	/**************************************************************************/

	template <typename Key, typename T, class Compare = std::less<Key>,
				class Allocator = std::allocator<ft::pair<const Key, T> > >
	class	map {
	public:
		typedef Key										key_type;
		typedef T										mapped_type;
		typedef ft::pair<const key_type, mapped_type>	value_type;
		typedef Compare									key_compare;
		typedef Allocator								allocator_type;
		typedef typename Allocator::reference			reference;
		typedef typename Allocator::const_reference		const_reference;
		typedef typename Allocator::pointer				pointer;
		typedef typename Allocator::const_pointer		const_pointer;
		typedef typename Allocator::size_type			size_type;
		typedef typename Allocator::difference_type		difference_type;
	private:
		typedef _map_value_compare<Key, T, Compare>				_value_compare;
		typedef _tree<value_type, _value_compare, Allocator>	_base;
	public:
		typedef typename _base::iterator				iterator;
		typedef typename _base::const_iterator			const_iterator;
		typedef ft::reverse_iterator<iterator>			reverse_iterator;
		typedef ft::reverse_iterator<const_iterator>	const_reverse_iterator;

		class	value_compare: public std::binary_function
										<value_type, value_type, bool> {
		private:
			friend class	map;
		public:
			bool	operator()(const value_type& lhs,
								const value_type& rhs) const
									{ return (comp(lhs.first, rhs.first)); }
		protected:
			value_compare(key_compare c): comp(c) { }

			key_compare		comp;
		};

		explicit map(const key_compare& comp = key_compare(),
						const allocator_type& alloc = allocator_type()):
										_tree(_value_compare(comp), alloc) { }
		map(const map& src);
		template <typename InputIt>
		map(InputIt first, InputIt last,
				const key_compare& comp = key_compare(),
				const allocator_type& alloc = allocator_type());
		~map(void) { }

		map&	operator=(const map& rhs);

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

		mapped_type&	operator[](const key_type& key);

		template <typename InputIt>
		void						insert(InputIt first, InputIt last);
		ft::pair<iterator, bool>	insert(const value_type& value);
		iterator					insert(iterator hint,
											const value_type& value);
		void						erase(iterator pos);
		size_type					erase(const key_type& key);
		void						erase(iterator first, iterator last);
		void						clear(void);
		void						swap(map& other);

		key_compare			key_comp(void) const
									{ return (_tree.value_comp().key_comp()); }
		value_compare		value_comp(void) const
									{ return (value_compare(key_comp())); }
		allocator_type		get_allocator(void) const
									{ return (_tree.alloc()); }

		iterator				find(const key_type& key)
											{ return (_tree.find(key)); }
		const_iterator			find(const key_type& key) const
											{ return (_tree.find(key)); }
		size_type				count(const key_type& key) const
											{ return (_tree.count_unique(key)); }
		iterator				lower_bound(const key_type& key)
											{ return (_tree.lower_bound(key)); }
		const_iterator			lower_bound(const key_type& key) const
											{ return (_tree.lower_bound(key)); }
		iterator				upper_bound(const key_type& key)
											{ return (_tree.upper_bound(key)); }
		const_iterator			upper_bound(const key_type& key) const
											{ return (_tree.upper_bound(key)); }
		ft::pair<iterator, iterator>
								equal_range(const key_type& key)
									{ return (_tree.equal_range_unique(key)); }
		ft::pair<const_iterator, const_iterator>
								equal_range(const key_type& key) const
									{ return (_tree.equal_range_unique(key)); }
	private:
		_base	_tree;
	};

	/**************************************************************************/
	/*                       CONSTRUCTORS / DESTRUCTORS                       */
	/**************************************************************************/

	template <typename Key, typename T, class Compare, class Allocator>
	map<Key, T, Compare, Allocator>::map(const map& src): _tree(src._tree)
	{
		insert(src.begin(), src.end());
	}

	template <typename Key, typename T, class Compare, class Allocator>
	template <typename InputIt>
	map<Key, T, Compare, Allocator>::map(InputIt first, InputIt last,
											const key_compare& comp,
											const allocator_type& alloc):
													_tree(_value_compare(comp),
															alloc)
	{
		insert(first, last);
	}

	/**************************************************************************/
	/*                        MEMBER OPERATOR OVERLOADS                       */
	/**************************************************************************/

	template <typename Key, typename T, class Compare, class Allocator>
	map<Key, T, Compare, Allocator>&
		map<Key, T, Compare, Allocator>::operator=(const map& rhs)
	{
		if (this != &rhs)
			_tree = rhs._tree;
		return (*this);
	}

	template <typename Key, typename T, class Compare, class Allocator>
	typename map<Key, T, Compare, Allocator>::mapped_type&
		map<Key, T, Compare, Allocator>::operator[](const key_type& key)
	{
		iterator	it = lower_bound(key);

		if (it == end() || _tree.value_comp()(key, *it))
			it = insert(it, value_type(key, mapped_type()));
		return (it->second);
	}

	/**************************************************************************/
	/*                      NON-MEMBER OPERATOR OVERLOADS                     */
	/**************************************************************************/

	template <typename Key, typename T, class Compare, class Allocator>
	bool	operator==(const map<Key, T, Compare, Allocator>& lhs,
						const map<Key, T, Compare, Allocator>& rhs)
	{
		return (lhs.size() == rhs.size()
					&& ft::equal(lhs.begin(), lhs.end(), rhs.begin()));
	}

	template <typename Key, typename T, class Compare, class Allocator>
	bool	operator!=(const map<Key, T, Compare, Allocator>& lhs,
						const map<Key, T, Compare, Allocator>& rhs)
	{
		return (!(lhs == rhs));
	}

	template <typename Key, typename T, class Compare, class Allocator>
	bool	operator<(const map<Key, T, Compare, Allocator>& lhs,
						const map<Key, T, Compare, Allocator>& rhs)
	{
		return (ft::lexicographical_compare(lhs.begin(), lhs.end(),
												rhs.begin(), rhs.end()));
	}

	template <typename Key, typename T, class Compare, class Allocator>
	bool	operator<=(const map<Key, T, Compare, Allocator>& lhs,
						const map<Key, T, Compare, Allocator>& rhs)
	{
		return (!(rhs < lhs));
	}

	template <typename Key, typename T, class Compare, class Allocator>
	bool	operator>(const map<Key, T, Compare, Allocator>& lhs,
						const map<Key, T, Compare, Allocator>& rhs)
	{
		return (rhs < lhs);
	}

	template <typename Key, typename T, class Compare, class Allocator>
	bool	operator>=(const map<Key, T, Compare, Allocator>& lhs,
						const map<Key, T, Compare, Allocator>& rhs)
	{
		return (!(lhs < rhs));
	}

	/**************************************************************************/
	/*                            MEMBER FUNCTIONS                            */
	/**************************************************************************/

	template <typename Key, typename T, class Compare, class Allocator>
	template <typename InputIt>
	void	map<Key, T, Compare, Allocator>::insert(InputIt first, InputIt last)
	{
		for (iterator end = _tree.end(); first != last; ++first)
			insert(end, *first);
	}

	template <typename Key, typename T, class Compare, class Allocator>
	ft::pair<typename map<Key, T, Compare, Allocator>::iterator, bool>
		map<Key, T, Compare, Allocator>::insert(const value_type& value)
	{
		return (_tree.insert_unique(value));
	}

	template <typename Key, typename T, class Compare, class Allocator>
	typename map<Key, T, Compare, Allocator>::iterator
		map<Key, T, Compare, Allocator>::insert(iterator hint,
												const value_type& value)
	{
		return (_tree.insert_unique(hint, value));
	}

	template <typename Key, typename T, class Compare, class Allocator>
	void	map<Key, T, Compare, Allocator>::erase(iterator pos)
	{
		_tree.erase(pos);
	}

	template <typename Key, typename T, class Compare, class Allocator>
	typename map<Key, T, Compare, Allocator>::size_type
		map<Key, T, Compare, Allocator>::erase(const key_type& key)
	{
		return (_tree.erase_unique(key));
	}

	template <typename Key, typename T, class Compare, class Allocator>
	void	map<Key, T, Compare, Allocator>::erase(iterator first,
													iterator last)
	{
		_tree.erase(first, last);
	}

	template <typename Key, typename T, class Compare, class Allocator>
	void	map<Key, T, Compare, Allocator>::clear(void)
	{
		_tree.clear();
	}

	template <typename Key, typename T, class Compare, class Allocator>
	void	map<Key, T, Compare, Allocator>::swap(map& other)
	{
		_tree.swap(other._tree);
	}

	/**************************************************************************/
	/*                          NON-MEMBER FUNCTIONS                          */
	/**************************************************************************/

	template <typename Key, typename T, class Compare, class Allocator>
	void	swap(const map<Key, T, Compare, Allocator>& lhs,
					const map<Key, T, Compare, Allocator>& rhs)
	{
		lhs.swap(rhs);
	}
}

#endif
