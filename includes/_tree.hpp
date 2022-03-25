#ifndef _TREE_HPP
# define _TREE_HPP

# include <memory>
# include <iterator>
# include <cstddef>
# include <algorithm>
# include <iomanip>
# include "reverse_iterator.hpp"
# include "pair.hpp"
# include "colors.hpp"

namespace	ft
{
	/**************************************************************************/
	/*                            _TREE DEFINITION                            */
	/**************************************************************************/

	/*
	** This "_tree" is a red-black tree
	** Its "root" is the left child of its "end_node", so when "root" != NULL,
	** "end_node->left" points to "root" and "root->parent" points to "end_node"
	** No other member than "left" is read or written at "end_node"
	*/

	/**************************************************************************/
	/*                        _TREE GENERIC ALGORITHMS                        */
	/**************************************************************************/

	/*
	** The following algorithms taking a NodePtr expect the "_tree" structure
	*/

	template <typename NodePtr>
	bool	_tree_is_left_child(NodePtr node)
	{
		return (node == node->parent->left);
	}

	template <typename NodePtr>
	NodePtr	_tree_min(NodePtr node)
	{
		while (node->left)
			node = node->left;
		return (node);
	}

	template <typename NodePtr>
	NodePtr	_tree_max(NodePtr node)
	{
		while (node->right)
			node = node->right;
		return (node);
	}

	template <typename NodePtr>
	NodePtr	_tree_next(NodePtr node)
	{
		if (node->right)
			return (_tree_min(node->right));
		while (node->parent && !_tree_is_left_child(node))
			node = node->parent;
		return (node->parent);
	}

	template <typename NodePtr>
	NodePtr	_tree_prev(NodePtr node)
	{
		if (node->left)
			return (_tree_max(node->left));
		while (node->parent && _tree_is_left_child(node))
			node = node->parent;
		return (node->parent);
	}

	template <typename NodePtr>
	void	_tree_rotate_left(NodePtr node)
	{
		NodePtr	new_top = node->right;

		node->right = new_top->left;
		if (node->right)
			node->right->parent = node;
		new_top->parent = node->parent;
		if (_tree_is_left_child(node))
			node->parent->left = new_top;
		else
			node->parent->right = new_top;
		new_top->left = node;
		node->parent = new_top;
	}

	template <typename NodePtr>
	void	_tree_rotate_right(NodePtr node)
	{
		NodePtr	new_top = node->left;

		node->left = new_top->right;
		if (node->left)
			node->left->parent = node;
		new_top->parent = node->parent;
		if (_tree_is_left_child(node))
			node->parent->left = new_top;
		else
			node->parent->right = new_top;
		new_top->right = node;
		node->parent = new_top;
	}

	template <typename NodePtr>
	void	_tree_insert_recolor(NodePtr root, NodePtr& node, NodePtr uncle)
	{
		node = node->parent;
		node->is_black = true;
		node = node->parent;
		node->is_black = (node == root);
		uncle->is_black = true;
	}
	template <typename NodePtr>
	void	_tree_insert_rebalance_left(NodePtr& node)
	{
		if (!_tree_is_left_child(node)) {
			node = node->parent;
			_tree_rotate_left(node);
		}
		node = node->parent;
		node->is_black = true;
		node = node->parent;
		node->is_black = false;
		_tree_rotate_right(node);
	}
	template <typename NodePtr>
	void	_tree_insert_rebalance_right(NodePtr& node)
	{
		if (_tree_is_left_child(node)) {
			node = node->parent;
			_tree_rotate_right(node);
		}
		node = node->parent;
		node->is_black = true;
		node = node->parent;
		node->is_black = false;
		_tree_rotate_left(node);
	}
	template <typename NodePtr>
	void	_tree_insert_rebalance(NodePtr root, NodePtr node)
	{
		NodePtr		uncle;

		node->is_black = (node == root);
		while (node != root && !node->parent->is_black) {
			if (_tree_is_left_child(node->parent)) {
				uncle = node->parent->parent->right;
				if (uncle && !uncle->is_black)
					_tree_insert_recolor(root, node, uncle);
				else {
					_tree_insert_rebalance_left(node);
					break ;
				}
			} else {
				uncle = node->parent->parent->left;
				if (uncle && !uncle->is_black)
					_tree_insert_recolor(root, node, uncle);
				else {
					_tree_insert_rebalance_right(node);
					break ;
				}
			}
		}
	}

	template <typename NodePtr>
	void	_tree_remove_rebalance_red_sibling(NodePtr& root, NodePtr& sibling,
												bool is_left_child)
	{
		sibling->is_black = true;
		sibling->parent->is_black = false;
		if (!is_left_child) {
			_tree_rotate_left(sibling->parent);
			if (root == sibling->left)
				root = sibling;
			sibling = sibling->left->right;
		} else {
			_tree_rotate_right(sibling->parent);
			if (root == sibling->right)
				root = sibling;
			sibling = sibling->right->left;
		}
	}
	template <typename NodePtr>
	bool	_tree_remove_rebalance_recurse(NodePtr root, NodePtr& child,
													NodePtr& sibling)
	{
		sibling->is_black = false;
		child = sibling->parent;
		if (child == root || !child->is_black) {
			child->is_black = true;
			return (false);
		}
		sibling = child->parent->left;
		if (_tree_is_left_child(child))
			sibling = child->parent->right;
		return (true);
	}
	template <typename NodePtr>
	void	_tree_remove_rebalance_sibling_red_child(NodePtr sibling,
														bool is_left_child)
	{
		if (!is_left_child) {
			if (!sibling->right || sibling->right->is_black) {
				sibling->left->is_black = true;
				sibling->is_black = false;
				_tree_rotate_right(sibling);
				sibling = sibling->parent;
			}
			sibling->is_black = sibling->parent->is_black;
			sibling->parent->is_black = true;
			sibling->right->is_black = true;
			_tree_rotate_left(sibling->parent);
		} else {
			if (!sibling->left || sibling->left->is_black) {
				sibling->right->is_black = true;
				sibling->is_black = false;
				_tree_rotate_left(sibling);
				sibling = sibling->parent;
			}
			sibling->is_black = sibling->parent->is_black;
			sibling->parent->is_black = true;
			sibling->left->is_black = true;
			_tree_rotate_right(sibling->parent);
		}
	}
	template <typename NodePtr>
	void	_tree_remove_rebalance(NodePtr root, NodePtr child, NodePtr sibling)
	{
		bool	is_left_child;

		while (true) {
			is_left_child = _tree_is_left_child(sibling);
			if (!sibling->is_black)
				_tree_remove_rebalance_red_sibling(root, sibling,
															is_left_child);
			if ((!sibling->left || sibling->left->is_black)
					&& (!sibling->right || sibling->right->is_black)) {
				if (!_tree_remove_rebalance_recurse(root, child, sibling))
					break ;
			} else {
				_tree_remove_rebalance_sibling_red_child(sibling,
															is_left_child);
				break ;
			}
		}
	}

	template <typename NodePtr>
	void	_tree_remove_node(NodePtr& root, NodePtr& to_del,
								NodePtr& to_del_child, NodePtr& to_del_sibling)
	{
		if (to_del_child)
			to_del_child->parent = to_del->parent;
		if (_tree_is_left_child(to_del)) {
			to_del->parent->left = to_del_child;
			if (to_del != root)
				to_del_sibling = to_del->parent->right;
			else
				root = to_del_child;
		} else {
			to_del->parent->right = to_del_child;
			to_del_sibling = to_del->parent->left;
		}
	}
	template <typename NodePtr>
	void	_tree_remove_successor(NodePtr& root, NodePtr& node,
									NodePtr& to_del)
	{
		to_del->parent = node->parent;
		if (_tree_is_left_child(node))
			to_del->parent->left = to_del;
		else
			to_del->parent->right = to_del;
		to_del->left = node->left;
		to_del->left->parent = to_del;
		to_del->right = node->right;
		if (to_del->right)
			to_del->right->parent = to_del;
		to_del->is_black = node->is_black;
		if (root == node)
			root = to_del;
	}
	template <typename NodePtr>
	void	_tree_remove(NodePtr root, NodePtr node)
	{
		NodePtr		to_del;
		NodePtr		to_del_child;
		NodePtr		to_del_sibling = NULL;
		bool		removed_black;

		to_del = node;
		if (node->left && node->right)
			to_del = _tree_next(node);
		to_del_child = to_del->right;
		if (to_del->left)
			to_del_child = to_del->left;
		_tree_remove_node(root, to_del, to_del_child, to_del_sibling);
		removed_black = to_del->is_black;
		if (to_del != node)
			_tree_remove_successor(root, node, to_del);
		if (removed_black && root) {
			if (to_del_child)
				to_del_child->is_black = true;
			else
				_tree_remove_rebalance(root, to_del_child, to_del_sibling);
		}
	}

	/*
	** Debugging: "_tree_sub_invariant" returns the black height of a subtree,
	** or 0 if it is an improper red-black tree
	** "_tree_invariant" returns true if the tree rooted at "root" is a proper
	** red-black tree, and false otherwise
	*/

	template <typename NodePtr>
	size_t	_tree_sub_invariant(NodePtr node)
	{
		size_t	left_black_height;

		if (!node)
			return (1);
		if ((node->left && node->left->parent != node)
				|| (node->right && node->right->parent != node))
			return (0);
		if (node->left && node->left == node->right)
			return (0);
		if (!node->is_black)
			if ((node->left && !node->left->is_black)
					|| (node->right && !node->right->is_black))
				return (0);
		left_black_height = _tree_sub_invariant(node->left);
		if (!left_black_height)
			return (0);
		if (left_black_height != _tree_sub_invariant(node->right))
			return (0);
		return (left_black_height + node->is_black);
	}
	template <typename NodePtr>
	bool	_tree_invariant(NodePtr root)
	{
		if (!root)
			return (true);
		if (!root->parent)
			return (false);
		if (!_tree_is_left_child(root))
			return (false);
		if (!root->is_black)
			return (false);
		return (_tree_sub_invariant(root) != 0);
	}

	/**************************************************************************/
	/*                               _TREE NODES                              */
	/**************************************************************************/

	/*
	** The "_tree" nodes are built through a "node_base" class with "parent",
	** "left" and "right" pointers to nodes, and a "is_black" color
	** The value is only stored in the "node" class, so that the "_tree"'s
	** "end_node" can be created without a value to initialize
	*/

	class	_tree_node_base {
	public:
		typedef _tree_node_base*		pointer;
		typedef const _tree_node_base*	const_pointer;

		_tree_node_base(void): parent(NULL), left(NULL),
								right(NULL), is_black(false) { }
		_tree_node_base(const _tree_node_base&): parent(NULL), left(NULL),
												right(NULL), is_black(false) { }
		virtual ~_tree_node_base(void) { }

		pointer		parent;
		pointer		left;
		pointer		right;
		bool		is_black;
	private:
		_tree_node_base&	operator=(const _tree_node_base&);
	};

	template <typename T>
	class	_tree_node: public _tree_node_base {
	public:
		typedef _tree_node_base		base;
		typedef T					value_type;
		typedef _tree_node*			pointer;
		typedef const _tree_node*	const_pointer;

		explicit _tree_node(const T& data): data(data) { }
		virtual ~_tree_node(void) { }

		value_type		data;
	private:
		_tree_node(void);
		_tree_node(const _tree_node&);
		_tree_node&	operator=(const _tree_node&);
	};

	/**************************************************************************/
	/*                             _TREE ITERATORS                            */
	/**************************************************************************/

	/*
	** In order to avoid the "get_ptr()" accessors, we would have to declare
	** "_tree" as a friend class, which might be forbidden in evaluation
	*/

	template <typename T>
	class	_tree_iterator {
	public:
		typedef std::bidirectional_iterator_tag		iterator_category;
		typedef T									value_type;
		typedef	std::ptrdiff_t						difference_type;
		typedef value_type&							reference;
		typedef value_type*							pointer;
	private:
		typedef _tree_node<value_type>				_node;
		typedef typename _node::pointer				_node_pointer;
		typedef typename _node::base				_node_base;
		typedef typename _node_base::pointer		_node_base_pointer;
	public:
		_tree_iterator(void) { }
		_tree_iterator(const _tree_iterator& it): _ptr(it._ptr) { }
		explicit _tree_iterator(_node_base_pointer ptr): _ptr(ptr) { }
		~_tree_iterator(void) { }

		_tree_iterator&	operator=(const _tree_iterator& rhs)
							{ _ptr = rhs._ptr; return (*this); }

		reference		operator*(void) const
							{ return (static_cast<_node_pointer>(_ptr)->data); }
		pointer			operator->(void) const
							{ return (&(operator*())); }

		_tree_iterator&	operator++(void) { _ptr = _tree_next(_ptr);
											return (*this); }
		_tree_iterator&	operator--(void) { _ptr = _tree_prev(_ptr);
											return (*this); }
		_tree_iterator	operator++(int) { _tree_iterator it(*this);
											++(*this); return (it); }
		_tree_iterator	operator--(int) { _tree_iterator it(*this);
											--(*this); return (it); }

		const _node_base_pointer&	get_ptr(void) const { return (_ptr); }

		friend bool	operator==(const _tree_iterator& lhs,
								const _tree_iterator& rhs)
							{ return (lhs._ptr == rhs._ptr); }
		friend bool	operator!=(const _tree_iterator& lhs,
								const _tree_iterator& rhs)
							{ return (!(lhs == rhs)); }
	private:
		_node_base_pointer		_ptr;
	};

	template <typename T>
	class	_tree_const_iterator {
	public:
		typedef std::bidirectional_iterator_tag		iterator_category;
		typedef T									value_type;
		typedef	std::ptrdiff_t						difference_type;
		typedef const value_type&					reference;
		typedef const value_type*					pointer;
	private:
		typedef const _tree_node<value_type>		_node;
		typedef typename _node::const_pointer		_node_pointer;
		typedef const typename _node::base			_node_base;
		typedef typename _node_base::const_pointer	_node_base_pointer;
	public:
		_tree_const_iterator(void) { }
		_tree_const_iterator(const _tree_const_iterator& it):
														_ptr(it._ptr) { }
		_tree_const_iterator(const _tree_iterator<T>& it):
														_ptr(it.get_ptr()) { }
		explicit _tree_const_iterator(_node_base_pointer ptr): _ptr(ptr) { }

		_tree_const_iterator&	operator=(const _tree_const_iterator& rhs)
											{ _ptr = rhs._ptr; return (*this); }

		reference		operator*(void) const
							{ return (static_cast<_node_pointer>(_ptr)->data); }
		pointer			operator->(void) const
							{ return (&(operator*())); }

		_tree_const_iterator&	operator++(void) { _ptr = _tree_next(_ptr);
													return (*this); }
		_tree_const_iterator&	operator--(void) { _ptr = _tree_prev(_ptr);
													return (*this); }
		_tree_const_iterator	operator++(int)
											{ _tree_const_iterator it(*this);
												++(*this); return (it); }
		_tree_const_iterator	operator--(int)
											{ _tree_const_iterator it(*this);
												--(*this); return (it); }

		const _node_base_pointer&	get_ptr(void) const { return (_ptr); }

		friend bool	operator==(const _tree_const_iterator& lhs,
								const _tree_const_iterator& rhs)
							{ return (lhs._ptr == rhs._ptr); }
		friend bool	operator!=(const _tree_const_iterator& lhs,
								const _tree_const_iterator& rhs)
							{ return (!(lhs == rhs)); }
	private:
		_node_base_pointer		_ptr;
	};

	/**************************************************************************/
	/*                            _TREE_BASE CLASS                            */
	/**************************************************************************/

	/*
	** The "_tree_base" is managing the "_tree"'s "end_node"
	*/

	template <class Allocator>
	class	_tree_base {
	public:
		typedef _tree_node_base							node_base;
		typedef typename Allocator::template
							rebind<node_base>::other	node_base_allocator;
		typedef typename node_base_allocator::pointer	node_base_pointer;
		typedef typename node_base_allocator::
										const_pointer	node_base_const_pointer;
	protected:
		_tree_base(void);
		~_tree_base(void);

		node_base_allocator		_node_base_alloc;
		node_base_pointer		_end_node_ptr;
	private:
		_tree_base(const _tree_base&);
		_tree_base&	operator=(const _tree_base&);
	};

	template <class Allocator>
	_tree_base<Allocator>::_tree_base(void):
										_node_base_alloc(node_base_allocator()),
										_end_node_ptr(NULL)
	{
		_end_node_ptr = _node_base_alloc.allocate(1);
		_node_base_alloc.construct(_end_node_ptr, node_base());
	}

	template <class Allocator>
	_tree_base<Allocator>::~_tree_base(void)
	{
		_node_base_alloc.destroy(_end_node_ptr);
		_node_base_alloc.deallocate(_end_node_ptr, 1);
		_end_node_ptr = NULL;
	}

	/**************************************************************************/
	/*                               _TREE CLASS                              */
	/**************************************************************************/

	template <typename T, class Compare, class Allocator>
	class	_tree: protected _tree_base<Allocator> {
	public:
		typedef T										value_type;
		typedef Compare									value_compare;
		typedef Allocator								allocator_type;
		typedef typename Allocator::reference			reference;
		typedef typename Allocator::const_reference		const_reference;
		typedef typename Allocator::pointer				pointer;
		typedef typename Allocator::const_pointer		const_pointer;
		typedef typename Allocator::size_type			size_type;
		typedef typename Allocator::difference_type		difference_type;
		using typename _tree_base<Allocator>::			node_base;
		using typename _tree_base<Allocator>::			node_base_pointer;
		using typename _tree_base<Allocator>::			node_base_const_pointer;
		typedef _tree_node<value_type>					node;
		typedef typename Allocator::template
								rebind<node>::other		node_allocator;
		typedef typename node_allocator::pointer		node_pointer;
		typedef typename node_allocator::const_pointer	node_const_pointer;
		typedef _tree_iterator<value_type>				iterator;
		typedef _tree_const_iterator<value_type>		const_iterator;
		typedef ft::reverse_iterator<iterator>			reverse_iterator;
		typedef ft::reverse_iterator<const_iterator>	const_reverse_iterator;

		_tree(void);
		explicit _tree(const value_compare& comp);
		_tree(const value_compare& comp, const allocator_type& alloc);
		_tree(const _tree& src);
		~_tree(void) { _destroy(root()); }

		_tree&	operator=(const _tree& rhs);

		const value_compare&	value_comp(void) const { return (_comp); }
		const node_allocator&	node_alloc(void) const { return (_node_alloc); }
		allocator_type			alloc(void) const
									{ return (allocator_type(_node_alloc)); }

		const size_type&	size(void) const { return (_size); }
		size_type			max_size(void) const
											{ return (_node_alloc.max_size()); }

		node_base_pointer		end_node(void) { return (this->_end_node_ptr); }
		node_base_const_pointer	end_node(void) const
											{ return (this->_end_node_ptr); }

		node_pointer		root(void) { return (static_cast
									<node_pointer>(end_node()->left)); }
		node_const_pointer	root(void) const { return (static_cast
									<node_const_pointer>(end_node()->left)); }

		iterator			begin(void)
									{ return (iterator(_begin_node())); }
		const_iterator		begin(void) const
									{ return (const_iterator(_begin_node())); }
		iterator			end(void)
									{ return (iterator(end_node())); }
		const_iterator		end(void) const
									{ return (const_iterator(end_node())); }

		ft::pair<iterator, bool>	insert_unique(const value_type& value);
		iterator					insert_unique(iterator hint,
													const value_type& value);
		void						erase(iterator pos);
		void						erase(iterator first, iterator last);
		template <typename Key>
		size_type					erase_unique(const Key& key);
		void						clear(void);
		void						swap(_tree& other);

		void						print(size_t indent = 4) const;

		template <typename Key>
		iterator					find(const Key& key);
		template <typename Key>
		const_iterator				find(const Key& key) const;
		template <typename Key>
		size_type					count_unique(const Key& key) const;
		template <typename Key>
		iterator					lower_bound(const Key& key);
		template <typename Key>
		const_iterator				lower_bound(const Key& key) const;
		template <typename Key>
		iterator					upper_bound(const Key& key);
		template <typename Key>
		const_iterator				upper_bound(const Key& key) const;
		template <typename Key>
		ft::pair<iterator, iterator>
									equal_range_unique(const Key& key);
		template <typename Key>
		ft::pair<const_iterator, const_iterator>
									equal_range_unique(const Key& key) const;
	private:
		typedef _tree_base<Allocator>	_base;

		node_base_pointer&			_begin_node(void)
												{ return (_begin_node_ptr); }
		const node_base_pointer&	_begin_node(void) const
												{ return (_begin_node_ptr); }

		template <typename InputIt>
		void				_assign_unique(InputIt first, InputIt last);
		node_pointer		_clone_node(node_const_pointer src_node);
		void				_structural_copy(node_base_pointer parent,
											node_base_pointer& child,
											node_base_const_pointer src_node);
		void				_assign_tree(const _tree& src);
		node_pointer		_construct_node(const value_type& value);
		iterator			_insert_node_at(node_base_pointer parent,
												node_base_pointer& child,
												node_base_pointer new_node);
		iterator			_insert_unique_after(iterator hint,
													const value_type& value);
		iterator			_insert_unique_before(iterator hint,
													const value_type& value);
		void				_destroy(node_pointer node);
		void				_print(node_base_const_pointer root,
									unsigned indent, unsigned padding) const;

		value_compare		_comp;
		node_allocator		_node_alloc;
		size_type			_size;
		node_base_pointer	_begin_node_ptr;
	};

	/**************************************************************************/
	/*                       CONSTRUCTORS / DESTRUCTORS                       */
	/**************************************************************************/

	template <typename T, class Compare, class Allocator>
	_tree<T, Compare, Allocator>::_tree(void): _base(),
												_comp(),
												_node_alloc(),
												_size(0),
												_begin_node_ptr(NULL)
	{
		_begin_node() = end_node();
	}

	template <typename T, class Compare, class Allocator>
	_tree<T, Compare, Allocator>::_tree(const value_compare& comp):
												_base(),
												_comp(comp),
												_node_alloc(),
												_size(0),
												_begin_node_ptr(NULL)
	{
		_begin_node() = end_node();
	}

	template <typename T, class Compare, class Allocator>
	_tree<T, Compare, Allocator>::_tree(const value_compare& comp,
											const allocator_type& alloc):
												_base(),
												_comp(comp),
												_node_alloc(alloc),
												_size(0),
												_begin_node_ptr(NULL)
	{
		_begin_node() = end_node();
	}

	template <typename T, class Compare, class Allocator>
	_tree<T, Compare, Allocator>::_tree(const _tree& src):
												_base(),
												_comp(src._comp),
												_node_alloc(src._node_alloc),
												_size(0),
												_begin_node_ptr(NULL)
	{
		_begin_node() = end_node();
		_assign_tree(src);
	}

	/**************************************************************************/
	/*                       MEMBER OPERATOR OVERLOADS                        */
	/**************************************************************************/

	template <typename T, class Compare, class Allocator>
	_tree<T, Compare, Allocator>&
		_tree<T, Compare, Allocator>::operator=(const _tree& rhs)
	{
		if (this != &rhs) {
			clear();
			_comp = rhs._comp;
			_node_alloc = rhs._node_alloc;
			_assign_tree(rhs);
		}
		return (*this);
	}

	/**************************************************************************/
	/*                            MEMBER FUNCTIONS                            */
	/**************************************************************************/

	template <typename T, class Compare, class Allocator>
	template <typename InputIt>
	void	_tree<T, Compare, Allocator>::_assign_unique(InputIt first,
															InputIt last)
	{
		if (_size != 0)
			clear();
		for ( ; first != last; ++first)
			insert_unique(*first);
	}

	template <typename T, class Compare, class Allocator>
	typename _tree<T, Compare, Allocator>::node_pointer
		_tree<T, Compare, Allocator>::_clone_node(node_const_pointer src_node)
	{
		node_pointer	new_node = _node_alloc.allocate(1);

		try {
			_node_alloc.construct(new_node, src_node->data);
		} catch (...) {
			_node_alloc.deallocate(new_node, 1);
			throw ;
		}
		new_node->is_black = src_node->is_black;
		return (new_node);
	}

	template <typename T, class Compare, class Allocator>
	void	_tree<T, Compare, Allocator>::_structural_copy(
											node_base_pointer parent,
											node_base_pointer& child,
											node_base_const_pointer src_node)
	{
		node_pointer	new_node;

		while (src_node) {
			new_node = _clone_node(static_cast<node_const_pointer>(src_node));
			new_node->parent = parent;
			if (!child) {
				child = new_node;
				if (_begin_node()->left)
					_begin_node() = _begin_node()->left;
				parent = child;
			} else {
				parent->right = new_node;
				parent = parent->right;
			}
			++_size;
			_structural_copy(parent, parent->left, src_node->left);
			src_node = src_node->right;
		}
	}

	template <typename T, class Compare, class Allocator>
	void	_tree<T, Compare, Allocator>::_assign_tree(const _tree& src)
	{
		if (_size != 0)
			clear();
		try {
			_structural_copy(end_node(), end_node()->left, src.root());
		} catch (...) {
			clear();
			throw ;
		}
	}

	template <typename T, class Compare, class Allocator>
	typename _tree<T, Compare, Allocator>::node_pointer
		_tree<T, Compare, Allocator>::_construct_node(const value_type& value)
	{
		node_pointer	node = _node_alloc.allocate(1);

		try {
			_node_alloc.construct(node, value);
		} catch (...) {
			_node_alloc.deallocate(node, 1);
			throw ;
		}
		return (node);
	}

	template <typename T, class Compare, class Allocator>
	void	_tree<T, Compare, Allocator>::_destroy(node_pointer node)
	{
		if (node) {
			_destroy(static_cast<node_pointer>(node->left));
			_destroy(static_cast<node_pointer>(node->right));
			_node_alloc.destroy(node);
			_node_alloc.deallocate(node, 1);
		}
	}

	template <typename T, class Compare, class Allocator>
	void	_tree<T, Compare, Allocator>::erase(iterator pos)
	{
		node_base_pointer	node = pos.get_ptr();

		if (node == _begin_node())
			_begin_node() = _tree_next(node);
		_tree_remove(end_node()->left, node);
		--_size;
		_node_alloc.destroy(static_cast<node_pointer>(node));
		_node_alloc.deallocate(static_cast<node_pointer>(node), 1);
	}

	template <typename T, class Compare, class Allocator>
	void	_tree<T, Compare, Allocator>::erase(iterator first, iterator last)
	{
		if (first == begin() && last == end())
			clear();
		else
			while (first != last)
				erase(first++);
	}

	template <typename T, class Compare, class Allocator>
	template <typename Key>
	typename _tree<T, Compare, Allocator>::size_type
		_tree<T, Compare, Allocator>::erase_unique(const Key& key)
	{
		iterator	it = find(key);

		if (it == end())
			return (0);
		erase(it);
		return (1);
	}

	template <typename T, class Compare, class Allocator>
	void	_tree<T, Compare, Allocator>::clear(void)
	{
		_destroy(root());
		_size = 0;
		_begin_node() = end_node();
		end_node()->left = NULL;
	}

	template <typename T, class Compare, class Allocator>
	void	_tree<T, Compare, Allocator>::swap(_tree& other)
	{
		std::swap(this->_end_node_ptr, other._end_node_ptr);
		std::swap(_begin_node_ptr, other._begin_node_ptr);
		std::swap(_comp, other._comp);
		std::swap(_node_alloc, other._node_alloc);
		std::swap(_size, other._size);
		if (_size != 0)
			end_node()->left->parent = end_node();
		if (other._size != 0)
			other.end_node()->left->parent = other.end_node();
	}

	template <typename T, class Compare, class Allocator>
	void	_tree<T, Compare, Allocator>::_print(node_base_const_pointer root,
										unsigned indent, unsigned padding) const
	{
		std::string	data_spaces(padding, ' ');
		std::string	branch_spaces(indent / 2, ' ');

		if (!root)
			return ;
		if (root->right)
			_print(root->right, indent, padding + indent);
		std::cout << data_spaces;
		if (root->right)
			std::cout << branch_spaces << "/\n" << data_spaces;
		if (!root->is_black)
			std::cout << RED;
		std::cout << static_cast<node_const_pointer>(root)->data
			<< NO_COLOR << std::endl;
		if (root->left) {
			std::cout << data_spaces << branch_spaces << "\\\n";
			_print(root->left, indent, padding + indent);
		}
	}
	template <typename T, class Compare, class Allocator>
	void	_tree<T, Compare, Allocator>::print(const size_t indent) const
	{
		_print(root(), indent, 0);
	}

	template <typename T, class Compare, class Allocator>
	typename _tree<T, Compare, Allocator>::iterator
		_tree<T, Compare, Allocator>::_insert_node_at(node_base_pointer parent,
													node_base_pointer& child,
													node_base_pointer new_node)
	{
		new_node->left = NULL;
		new_node->right = NULL;
		new_node->parent = parent;
		child = new_node;
		if (_begin_node()->left)
			_begin_node() = _begin_node()->left;
		_tree_insert_rebalance(end_node()->left, child);
		++_size;
		return (iterator(child));
	}

	template <typename T, class Compare, class Allocator>
	ft::pair<typename _tree<T, Compare, Allocator>::iterator, bool>
		_tree<T, Compare, Allocator>::insert_unique(const value_type& value)
	{
		node_base_pointer	parent = end_node();
		node_base_pointer	child = parent->left;
		bool				is_less = true;
		iterator			parent_it;

		while (child) {
			parent = child;
			child = parent->left;
			is_less = _comp(value, static_cast<node_pointer>(parent)->data);
			if (!is_less)
				child = parent->right;
		}
		parent_it = iterator(parent);
		if (is_less && (parent_it == begin() || _comp(*--parent_it, value)))
			return (ft::pair<iterator, bool>(_insert_node_at(parent,
							parent->left, _construct_node(value)), true));
		else if (_comp(*parent_it, value))
			return (ft::pair<iterator, bool>(_insert_node_at(parent,
							parent->right, _construct_node(value)), true));
		return (ft::pair<iterator, bool>(parent_it, false));
	}

	template <typename T, class Compare, class Allocator>
	typename _tree<T, Compare, Allocator>::iterator
		_tree<T, Compare, Allocator>::_insert_unique_after(iterator hint,
														const value_type& value)
	{
		iterator			next = hint;
		node_base_pointer	parent;

		++next;
		if (next == end() || _comp(value, *next)) {
			if (!hint.get_ptr()->right) {
				parent = hint.get_ptr();
				return (_insert_node_at(parent, parent->right,
							_construct_node(value)));
			} else {
				parent = next.get_ptr();
				return (_insert_node_at(parent, parent->left,
							_construct_node(value)));
			}
		} else if (!_comp(*next, value))
			return (next);
		return (insert_unique(value).first);
	}
	template <typename T, class Compare, class Allocator>
	typename _tree<T, Compare, Allocator>::iterator
		_tree<T, Compare, Allocator>::_insert_unique_before(iterator hint,
														const value_type& value)
	{
		iterator			prev = hint;
		node_base_pointer	parent;

		if (hint == begin() || _comp(*--prev, value)) {
			if (hint != end() && !hint.get_ptr()->left) {
				parent = hint.get_ptr();
				return (_insert_node_at(parent, parent->left,
							_construct_node(value)));
			} else {
				parent = prev.get_ptr();
				return (_insert_node_at(parent, parent->right,
							_construct_node(value)));
			}
		} else if (!_comp(value, *prev))
			return (prev);
		return (insert_unique(value).first);
	}
	template <typename T, class Compare, class Allocator>
	typename _tree<T, Compare, Allocator>::iterator
		_tree<T, Compare, Allocator>::insert_unique(iterator hint,
													const value_type& value)
	{
		iterator			prev = hint;
		node_base_pointer	parent;

		if (hint != end() && _comp(*hint, value))
			return (_insert_unique_after(hint, value));
		else if (hint == end() && hint == begin()) {
			parent = hint.get_ptr();
			return (_insert_node_at(parent, parent->left,
						_construct_node(value)));
		} else if (hint == end() || _comp(value, *hint))
			return (_insert_unique_before(hint, value));
		return (hint);
	}

	template <typename T, class Compare, class Allocator>
	template <typename Key>
	typename _tree<T, Compare, Allocator>::iterator
		_tree<T, Compare, Allocator>::find(const Key& key)
	{
		iterator	it = lower_bound(key);

		if (it != end() && !_comp(key, *it))
			return (it);
		return (end());
	}

	template <typename T, class Compare, class Allocator>
	template <typename Key>
	typename _tree<T, Compare, Allocator>::const_iterator
		_tree<T, Compare, Allocator>::find(const Key& key) const
	{
		const_iterator	it = lower_bound(key);

		if (it != end() && !_comp(key, *it))
			return (it);
		return (end());
	}

	template <typename T, class Compare, class Allocator>
	template <typename Key>
	typename _tree<T, Compare, Allocator>::size_type
		_tree<T, Compare, Allocator>::count_unique(const Key& key) const
	{
		if (find(key) != end())
			return (1);
		return (0);
	}

	template <typename T, class Compare, class Allocator>
	template <typename Key>
	typename _tree<T, Compare, Allocator>::iterator
		_tree<T, Compare, Allocator>::lower_bound(const Key& key)
	{
		node_pointer		node = root();
		node_base_pointer	result = end_node();

		while (node) {
			if (!_comp(node->data, key)) {
				result = node;
				node = static_cast<node_pointer>(node->left);
			}
			else
				node = static_cast<node_pointer>(node->right);
		}
		return (iterator(result));
	}

	template <typename T, class Compare, class Allocator>
	template <typename Key>
	typename _tree<T, Compare, Allocator>::const_iterator
		_tree<T, Compare, Allocator>::lower_bound(const Key& key) const
	{
		node_const_pointer		node = root();
		node_base_const_pointer	result = end_node();

		while (node) {
			if (!_comp(node->data, key)) {
				result = node;
				node = static_cast<node_const_pointer>(node->left);
			}
			else
				node = static_cast<node_const_pointer>(node->right);
		}
		return (const_iterator(result));
	}

	template <typename T, class Compare, class Allocator>
	template <typename Key>
	typename _tree<T, Compare, Allocator>::iterator
		_tree<T, Compare, Allocator>::upper_bound(const Key& key)
	{
		node_pointer		node = root();
		node_base_pointer	result = end_node();

		while (node) {
			if (_comp(key, node->data)) {
				result = node;
				node = static_cast<node_pointer>(node->left);
			}
			else
				node = static_cast<node_pointer>(node->right);
		}
		return (iterator(result));
	}

	template <typename T, class Compare, class Allocator>
	template <typename Key>
	typename _tree<T, Compare, Allocator>::const_iterator
		_tree<T, Compare, Allocator>::upper_bound(const Key& key) const
	{
		node_const_pointer		node = root();
		node_base_const_pointer	result = end_node();

		while (node) {
			if (_comp(key, node->data)) {
				result = node;
				node = static_cast<node_pointer>(node->left);
			}
			else
				node = static_cast<node_pointer>(node->right);
		}
		return (const_iterator(result));
	}

	template <typename T, class Compare, class Allocator>
	template <typename Key>
	ft::pair<typename _tree<T, Compare, Allocator>::iterator,
				typename _tree<T, Compare, Allocator>::iterator>
		_tree<T, Compare, Allocator>::equal_range_unique(const Key& key)
	{
		typedef ft::pair<iterator, iterator>	pair_t;

		node_pointer		node = root();
		node_base_pointer	result = end_node();

		while (node) {
			if (_comp(key, node->data)) {
				result = node;
				node = static_cast<node_pointer>(node->left);
			}
			else if (_comp(node->data, key))
				node = static_cast<node_pointer>(node->right);
			else if (node->right) {
				result = _tree_min(node->right);
				return (pair_t(iterator(node), iterator(result)));
			} else
				return (pair_t(iterator(node), iterator(result)));
		}
		return (pair_t(iterator(result), iterator(result)));
	}

	template <typename T, class Compare, class Allocator>
	template <typename Key>
	ft::pair<typename _tree<T, Compare, Allocator>::const_iterator,
				typename _tree<T, Compare, Allocator>::const_iterator>
		_tree<T, Compare, Allocator>::equal_range_unique(const Key& key) const
	{
		typedef ft::pair<const_iterator, const_iterator>	pair_t;

		node_const_pointer		node = root();
		node_base_const_pointer	result = end_node();

		while (node) {
			if (_comp(key, node->data)) {
				result = node;
				node = static_cast<node_const_pointer>(node->left);
			}
			else if (_comp(node->data, key))
				node = static_cast<node_const_pointer>(node->right);
			else if (node->right) {
				result = _tree_min(node->right);
				return (pair_t(const_iterator(node), const_iterator(result)));
			} else
				return (pair_t(const_iterator(node), const_iterator(result)));
		}
		return (pair_t(const_iterator(result), const_iterator(result)));
	}
}

#endif
