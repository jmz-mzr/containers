#ifndef _TREE_HPP
# define _TREE_HPP

# include <memory>
# include <iterator>
# include <cstddef>
# include <algorithm>
# include <iomanip>
# include "reverse_iterator.hpp"
# include "pair.hpp"

namespace	ft
{
/*
** Note on what this tree is, and how it is constructed
*/
	template <typename NodePtr>
	bool	_tree_is_left_child(NodePtr node)
	{
//		if (LIBSTDCPP)
//			return (node->parent && node == node->parent->left);
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
		while (!_tree_is_left_child(node))
			node = node->parent;
		return (node->parent);
	}

	template <typename NodePtr>
	NodePtr	_tree_prev(NodePtr node)
	{
		if (node->left)
			return (_tree_max(node->left));
		while (_tree_is_left_child(node))
			node = node->parent;
		return (node->parent);
	}

/*	template <typename NodePtr>
	NodePtr	_tree_leaf(NodePtr node)
	{
		while (true) {
			if (node->left) {
				node = node->left;
				continue ;
			}
			if (node->right) {
				node = node->right;
				continue ;
			}
			break ;
		}
		return (node);
	}*/

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

/*	template <typename NodePtr>
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
	}*/
	template <typename NodePtr>
	void	_tree_insert_rebalance(NodePtr root, NodePtr node)
	{
		NodePtr		uncle;

		node->is_black = (node == root);
		while (node != root && !node->parent->is_black) {
			if (_tree_is_left_child(node->parent)) {
				uncle = node->parent->parent->right;
				if (uncle && !uncle->is_black) {
					node = node->parent;
					node->is_black = true;
					node = node->parent;
					node->is_black = (node == root);
					uncle->is_black = true;
				} else {
					if (!_tree_is_left_child(node)) {
						node = node->parent;
						_tree_rotate_left(node);
					}
					node = node->parent;
					node->is_black = true;
					node = node->parent;
					node->is_black = false;
					_tree_rotate_right(node);
					break ;
				}
			} else {
				uncle = node->parent->parent->left;
				if (uncle && !uncle->is_black) {
					node = node->parent;
					node->is_black = true;
					node = node->parent;
					node->is_black = (node == root);
					uncle->is_black = true;
				} else {
					if (_tree_is_left_child(node)) {
						node = node->parent;
						_tree_rotate_right(node);
					}
					node = node->parent;
					node->is_black = true;
					node = node->parent;
					node->is_black = false;
					_tree_rotate_left(node);
					break ;
				}
			}
		}
	}

/*	template <typename NodePtr>
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
		while (true) {
			if (!_tree_is_left_child(sibling)) {
				if (!sibling->is_black)
					_tree_remove_rebalance_red_sibling(root, sibling, false);
				if ((!sibling->left || sibling->left->is_black)
						&& (!sibling->right || sibling->right->is_black)) {
					if (!_tree_remove_rebalance_recurse(root, child, sibling))
						break ;
				} else {
					_tree_remove_rebalance_sibling_red_child(sibling, false);
					break ;
				}
			} else {
				if (!sibling->is_black)
					_tree_remove_rebalance_red_sibling(root, sibling, true);
				if ((!sibling->left || sibling->left->is_black)
						&& (!sibling->right || sibling->right->is_black)) {
					if (!_tree_remove_rebalance_recurse(root, child, sibling))
						break ;
				} else {
					_tree_remove_rebalance_sibling_red_child(sibling, true);
					break ;
				}
			}
		}
	}*/

/*	template <typename NodePtr>
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
	}*/
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
		removed_black = to_del->is_black;
		if (to_del != node) {
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
		if (removed_black && root) {
			if (to_del_child)
				to_del_child->is_black = true;
			else {
				while (true) {
					if (!_tree_is_left_child(to_del_sibling)) {
						if (!to_del_sibling->is_black) {
							to_del_sibling->is_black = true;
							to_del_sibling->parent->is_black = false;
							_tree_rotate_left(to_del_sibling->parent);
							if (root == to_del_sibling->left)
								root = to_del_sibling;
							to_del_sibling = to_del_sibling->left->right;
						}
						if ((!to_del_sibling->left
								|| to_del_sibling->left->is_black)
								&& (!to_del_sibling->right
								|| to_del_sibling->right->is_black)) {
							to_del_sibling->is_black = false;
							to_del_child = to_del_sibling->parent;
							if (to_del_child == root
									|| !to_del_child->is_black) {
								to_del_child->is_black = true;
								break ;
							}
							to_del_sibling = to_del_child->parent->left;
							if (_tree_is_left_child(to_del_child))
								to_del_sibling = to_del_child->parent->right;
						} else {
							if (!to_del_sibling->right
									|| to_del_sibling->right->is_black) {
								to_del_sibling->left->is_black = true;
								to_del_sibling->is_black = false;
								_tree_rotate_right(to_del_sibling);
								to_del_sibling = to_del_sibling->parent;
							}
							to_del_sibling->is_black =
								to_del_sibling->parent->is_black;
							to_del_sibling->parent->is_black = true;
							to_del_sibling->right->is_black = true;
							_tree_rotate_left(to_del_sibling->parent);
							break ;
						}
					} else {
						if (!to_del_sibling->is_black) {
							to_del_sibling->is_black = true;
							to_del_sibling->parent->is_black = false;
							_tree_rotate_right(to_del_sibling->parent);
							if (root == to_del_sibling->right)
								root = to_del_sibling;
							to_del_sibling = to_del_sibling->right->left;
						}
						if ((!to_del_sibling->left
								|| to_del_sibling->left->is_black)
								&& (!to_del_sibling->right
								|| to_del_sibling->right->is_black)) {
							to_del_sibling->is_black = false;
							to_del_child = to_del_sibling->parent;
							if (to_del_child == root
									|| !to_del_child->is_black) {
								to_del_child->is_black = true;
								break ;
							}
							to_del_sibling = to_del_child->parent->left;
							if (_tree_is_left_child(to_del_child))
								to_del_sibling = to_del_child->parent->right;
						} else {
							if (!to_del_sibling->left
									|| to_del_sibling->left->is_black) {
								to_del_sibling->right->is_black = true;
								to_del_sibling->is_black = false;
								_tree_rotate_left(to_del_sibling);
								to_del_sibling = to_del_sibling->parent;
							}
							to_del_sibling->is_black =
								to_del_sibling->parent->is_black;
							to_del_sibling->parent->is_black = true;
							to_del_sibling->left->is_black = true;
							_tree_rotate_right(to_del_sibling->parent);
							break ;
						}
					}
				}
			}
		}
	}

/*
** Debugging: "_tree_sub_invariant" returns the black height of a subtree,
** or 0 if it is an improper red-black tree
** "_tree_invariant" returns true if the tree rooted at root is a proper
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

/*	template <typename T>
	class	_tree_node {
	public:
		typedef T	value_type;

		explicit _tree_node(const T& data): data(data), parent(), left(),
											right(), is_black(false) { }

		T				data;
		_tree_node*		parent;
		_tree_node*		left;
		_tree_node*		right;
		bool			is_black;
//		bool			is_constructed;
	private:
		_tree_node(const _tree_node&);
		_tree_node&	operator=(const _tree_node&);
	};*/

	class	_tree_node_base {
	public:
		typedef _tree_node_base*		pointer;
		typedef const _tree_node_base*	const_pointer;

		_tree_node_base(void): parent(NULL), left(NULL), right(NULL),
								is_black(false) { }
		_tree_node_base(const _tree_node_base&): parent(NULL), left(NULL),
												right(NULL), is_black(false) { }
		virtual ~_tree_node_base(void) { }

		_tree_node_base*	parent;
		_tree_node_base*	left;
		_tree_node_base*	right;
		bool				is_black;
	private:
		_tree_node_base&	operator=(const _tree_node_base&);
	};

	template <typename T>
	class	_tree_node: public _tree_node_base {
	public:
		typedef _tree_node_base		base;
		typedef T					value_type;

		explicit _tree_node(const T& data): data(data) { }
		virtual ~_tree_node(void) { }

		T			data;
	private:
		_tree_node(void);
		_tree_node(const _tree_node&);
		_tree_node&	operator=(const _tree_node&);
	};

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
		typedef _node*								_node_pointer;
		typedef typename _node::base				_node_base;
		typedef typename _node_base::pointer		_node_base_pointer;
	public:
		_tree_iterator(void) { }
		_tree_iterator(const _tree_iterator& it): _ptr(it._ptr) { }
		explicit _tree_iterator(_node_pointer ptr): _ptr(ptr) { }
		~_tree_iterator(void) { }

		_tree_iterator&	operator=(const _tree_iterator& rhs) {
											_ptr = rhs._ptr; return (*this); }

		reference		operator*(void) const {
							return (static_cast<_node_pointer>(_ptr)->data); }
		pointer			operator->(void) const { return (&(operator*())); }

		_tree_iterator&	operator++(void) { _ptr = _tree_next(_ptr);
											return (*this); }
		_tree_iterator&	operator--(void) { _ptr = _tree_prev(_ptr);
											return (*this); }
		_tree_iterator	operator++(int) { _tree_iterator it(*this);
											++(*this); return (it); }
		_tree_iterator	operator--(int) { _tree_iterator it(*this);
											--(*this); return (it); }

		const _node_base_pointer&	get_ptr(void) const { return (_ptr); }
	private:
		_node_base_pointer		_ptr;
	};

	template <typename T>
	bool	operator==(const _tree_iterator<T>& lhs,
						const _tree_iterator<T>& rhs)
	{
		return (lhs.get_ptr() == rhs.get_ptr());
	}
	template <typename T>
	bool	operator!=(const _tree_iterator<T>& lhs,
						const _tree_iterator<T>& rhs)
	{
		return (!(lhs == rhs));
	}

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
		typedef const _node*						_node_pointer;
		typedef const typename _node::base			_node_base;
		typedef typename _node_base::const_pointer	_node_base_pointer;
	public:
		_tree_const_iterator(void) { }
		_tree_const_iterator(const _tree_const_iterator& it):
														_ptr(it._ptr) { }
		_tree_const_iterator(const _tree_iterator<T>& it):
														_ptr(it.get_ptr()) { }
		explicit _tree_const_iterator(_node_pointer ptr): _ptr(ptr) { }

		_tree_const_iterator&	operator=(const _tree_const_iterator& rhs) {
											_ptr = rhs._ptr; return (*this); }

		reference		operator*(void) const {
							return (static_cast<_node_pointer>(_ptr)->data); }
		pointer			operator->(void) const { return (&(operator*())); }

		_tree_const_iterator&	operator++(void) { _ptr = _tree_next(_ptr);
													return (*this); }
		_tree_const_iterator&	operator--(void) { _ptr = _tree_prev(_ptr);
													return (*this); }
		_tree_const_iterator	operator++(int) {
												_tree_const_iterator it(*this);
												++(*this); return (it); }
		_tree_const_iterator	operator--(int) {
												_tree_const_iterator it(*this);
												--(*this); return (it); }

		const _node_base_pointer&	get_ptr(void) const { return (_ptr); }
	private:
		_node_base_pointer		_ptr;
	};

	template <typename T>
	bool	operator==(const _tree_const_iterator<T>& lhs,
						const _tree_const_iterator<T>& rhs)
	{
		return (lhs.get_ptr() == rhs.get_ptr());
	}
	template <typename T>
	bool	operator!=(const _tree_const_iterator<T>& lhs,
						const _tree_const_iterator<T>& rhs)
	{
		return (!(lhs == rhs));
	}

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
		_tree_base(const Allocator&);
		~_tree_base(void);

		node_base_allocator		_base_alloc;
		node_base_pointer		_end_node_ptr;
//		node_base_const_pointer	_end_node_const_ptr;
	private:
		_tree_base(void);
		_tree_base(const _tree_base&);
		_tree_base&	operator=(const _tree_base&);
	};

	template <class Allocator>
	_tree_base<Allocator>::_tree_base(const Allocator&):
											_base_alloc(node_base_allocator()),
											_end_node_ptr(NULL)//,
//											_end_node_const_ptr(NULL)
	{
		_end_node_ptr = _base_alloc.allocate(1);
		_base_alloc.construct(_end_node_ptr, node_base());
//		_end_node_const_ptr = _end_node_ptr;
	}

	template <class Allocator>
	_tree_base<Allocator>::~_tree_base(void)
	{
		_base_alloc.destroy(_end_node_ptr);
		_base_alloc.deallocate(_end_node_ptr, 1);
		_end_node_ptr = NULL;
//		_end_node_const_ptr = NULL;
	}

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
//		using typename _tree_base<Allocator>::			node_base;
//		using typename _tree_base<Allocator>::			node_base_pointer;
//		using typename _tree_base<Allocator>::			node_base_const_pointer;
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

		value_compare&			value_comp(void) { return (_comp); }
		const value_compare&	value_comp(void) const { return (_comp); }
		node_allocator&			node_alloc(void) { return
														(_node_alloc); }
		const node_allocator&	node_alloc(void) const { return
															(_node_alloc); }
		allocator_type			alloc(void) const { return (allocator_type
															(_node_alloc)); }

		const size_type&	size(void) const { return (_size); }
		size_type			max_size(void) const { return
												(_node_alloc.max_size()); }

		node_pointer			end_node(void) { return (static_cast<
									node_pointer>(this->_end_node_ptr)); }
		node_const_pointer		end_node(void) const { return (static_cast<
									node_const_pointer>(this->_end_node_ptr)); }
		node_pointer			root(void) { return (static_cast<
									node_pointer>(end_node()->left)); }
		node_const_pointer		root(void) const { return (static_cast<
									node_const_pointer>(end_node()->left)); }

		iterator			begin(void) { return
											(iterator(_begin_node())); }
		const_iterator		begin(void) const { return
											(const_iterator(_begin_node())); }
		iterator			end(void) { return
											(iterator(end_node())); }
		const_iterator		end(void) const { return
											(const_iterator(end_node())); }

		ft::pair<iterator, bool>	insert_unique(const value_type& value);
		iterator					insert_unique(iterator hint,
													const value_type& value);
		void						erase(iterator pos);
		void						erase(iterator first, iterator last);
		template <typename Key>
		size_type					erase_unique(const Key& key);
//		node_pointer				remove(iterator pos);
		void						clear(void);
		void						swap(_tree& other);
		void						print(node_pointer root = NULL,
											size_t indent = 0);

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

		template <typename InputIt>	// protect via enable_if ?
		void					_assign_unique(InputIt first, InputIt last);
		node_pointer			_construct_node(const value_type& value);
		node_pointer&			_begin_node(void) { return (_begin_node_ptr); }
		const node_pointer&		_begin_node(void) const { return (_begin_node_ptr); }
		iterator				_insert_node_at(node_pointer parent,
													node_pointer& child,
													node_pointer new_node);
		iterator				_insert_unique_after(iterator hint,
														const value_type& value);
/*		node_pointer&			_find_leaf_low(node_pointer& parent,
												const value_type& value);
		node_pointer&			_find_leaf_high(node_pointer& parent,
												const value_type& value);
		node_pointer&			_find_leaf(iterator hint, node_pointer& parent,
												const value_type& value);	// hint before!
		template <typename Key>
		node_pointer&			_find_equal(node_pointer& parent,
												const Key& key);
		template <typename Key>
		node_pointer&			_find_equal_after(iterator hint, node_pointer& parent,
													const Key& key);
		template <typename Key>
		node_pointer&			_find_equal(iterator hint, node_pointer& parent,
												const Key& key);*/	// hint before!
		void					_destroy(node_pointer node);
//		node_pointer			_detach(void);
//		static node_pointer		_detach(node_pointer cache);	// plutot destroy & insert_u ?

		value_compare		_comp;
		node_allocator		_node_alloc;
		size_type			_size;
		node_pointer		_begin_node_ptr;
//		node_ptr	_min;
//		node_ptr	_max;
	};

	/**************************************************************************/
	/*                       CONSTRUCTORS / DESTRUCTORS                       */
	/**************************************************************************/

	template <typename T, class Compare, class Allocator>
	_tree<T, Compare, Allocator>::_tree(void): _base(allocator_type()),
												_comp(),
												_node_alloc(),
												_size(0),
												_begin_node_ptr(node_pointer())
	{
		_begin_node() = end_node();
	}

	template <typename T, class Compare, class Allocator>
	_tree<T, Compare, Allocator>::_tree(const value_compare& comp):
												_base(allocator_type()),
												_comp(comp),
												_node_alloc(),
												_size(0),
												_begin_node_ptr(node_pointer())
	{
		_begin_node() = end_node();
	}

	template <typename T, class Compare, class Allocator>
	_tree<T, Compare, Allocator>::_tree(const value_compare& comp,
											const allocator_type& alloc):
												_base(alloc),
												_comp(comp),
												_node_alloc(alloc),
												_size(0),
												_begin_node_ptr(node_pointer())
	{
		_begin_node() = end_node();
	}

	template <typename T, class Compare, class Allocator>
	_tree<T, Compare, Allocator>::_tree(const _tree& src):
												_base(src._node_alloc),
												_comp(src._comp),
												_node_alloc(src._node_alloc),
												_size(0),
												_begin_node_ptr(node_pointer())
	{
		_begin_node() = end_node();
		_assign_unique(src.begin(), src.end());
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
			_assign_unique(rhs.begin(), rhs.end());
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
		node_pointer	node = pos._ptr;

		if (node == _begin_node())
			_begin_node() = static_cast<node_pointer>(_tree_next(node));
		_tree_remove(end_node()->left, pos._ptr);
		--_size;
		_node_alloc.destroy(node);
		_node_alloc.deallocate(node, 1);
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
	void	_tree<T, Compare, Allocator>::swap(_tree& other)	// test
	{
		std::swap(this->_end_node_ptr, other._end_node_ptr);
		std::swap(_begin_node_ptr, other._begin_node_ptr);
		std::swap(_comp, other._comp);
		std::swap(_node_alloc, other._node_alloc);
		std::swap(_size, other._size);
		if (_size == 0)
			_begin_node() = end_node();	// pas besoin ?
		else
			end_node()->left->parent = end_node();
		if (other._size == 0)
			other._begin_node() = other.end_node();	// pas besoin ?
		else
			other.end_node()->left->parent = other.end_node();
	}

	template <typename T, class Compare, class Allocator>
	void	_tree<T, Compare, Allocator>::print(node_pointer root, size_t indent)
	{
		static bool	first_call = true;

		if (!root && first_call) {
			first_call = false;
			return (print(this->root(), indent));
		} else if (!root && !first_call) {
			first_call = true;
			return ;
		}
		if (root->right)
			print(static_cast<node_pointer>(root->right), indent + 4);
		if (indent)
			std::cout << std::setw(indent) << ' ';
		if (root->right)
			std::cout << " /\n"
				<< std::setw(indent) << ' ';
		if (!root->is_black)
			std::cout << "\033[0;31m";
		std::cout << root->data << "\033[0m" << "\n ";
		if (root->left) {
			std::cout << std::setw(indent) << ' ' << " \\\n";
			print(static_cast<node_pointer>(root->left), indent + 4);
		}
	}

/*	template <typename T, class Compare, class Allocator>
	typename _tree<T, Compare, Allocator>::node_pointer&
		_tree<T, Compare, Allocator>::_find_leaf_low(node_pointer& parent,
														const value_type& value)
	{
	}

	template <typename T, class Compare, class Allocator>
	typename _tree<T, Compare, Allocator>::node_pointer&
		_tree<T, Compare, Allocator>::_find_leaf_high(node_pointer& parent,
														const value_type& value)
	{
	}

	template <typename T, class Compare, class Allocator>
	typename _tree<T, Compare, Allocator>::node_pointer&
		_tree<T, Compare, Allocator>::_find_leaf(iterator hint,
													node_pointer& parent,
													const value_type& value)	// hint before!
	{
	}

	template <typename T, class Compare, class Allocator>
	template <typename Key>
	typename _tree<T, Compare, Allocator>::node_pointer&
		_tree<T, Compare, Allocator>::_find_equal(node_pointer& parent,
													const Key& key)
	{
		node_pointer	node = root();

		while (node) {
			if (_comp(key, node->data)) {
				if (node->left)
					node = static_cast<node_pointer>(node->left);
				else {
					parent = node;
					return (static_cast<node_pointer>(node->left));	// ref?
				}
			} else if (_comp(node->data, key)) {
				if (node->right)
					node = static_cast<node_pointer>(node->right);
				else {
					parent = node;
					return (static_cast<node_pointer>(node->right));	// ref?
				}
			} else {
				parent = node;
				return (parent);
			}
		}
		parent = end_node();
		return (parent->left);
	}

	template <typename T, class Compare, class Allocator>
	template <typename Key>
	typename _tree<T, Compare, Allocator>::node_pointer&
		_tree<T, Compare, Allocator>::_find_equal_after(iterator hint,
														node_pointer& parent,
														const Key& key)
	{
		iterator	next = std::advance(hint, 1);

		if (next == end() || _comp(key, *next)) {
			if (!hint._ptr->right) {
				parent = static_cast<node_pointer>(hint._ptr);
				return (static_cast<node_pointer>(parent->right));
			} else {
				parent = static_cast<node_pointer>(next._ptr);
				return (static_cast<node_pointer>(parent->left));
			}
		}
		return (_find_equal(parent, key));
	}
	template <typename T, class Compare, class Allocator>
	template <typename Key>
	typename _tree<T, Compare, Allocator>::node_pointer&
		_tree<T, Compare, Allocator>::_find_equal(iterator hint,
													node_pointer& parent,
													const Key& key)	// hint before!
	{
		iterator	prev = hint;

		if (hint != end() && _comp(*hint, key))
			return (_find_equal_after(hint, parent, key));
		else if (hint == end() || _comp(key, *hint)) {
			if (hint == begin() || _comp(*--prev, key)) {
				if (!hint._ptr->left) {
					parent = static_cast<node_pointer>(hint._ptr);
					return (static_cast<node_pointer>(parent->left));
				} else {
					parent = static_cast<node_pointer>(prev._ptr);
					return (static_cast<node_pointer>(parent->right));
				}
			}
			return (_find_equal(parent, key));
		}
		parent = static_cast<node_pointer>(hint._ptr);
		return (parent);
	}*/

	template <typename T, class Compare, class Allocator>
	typename _tree<T, Compare, Allocator>::iterator
		_tree<T, Compare, Allocator>::_insert_node_at(node_pointer parent,
														node_pointer& child,
														node_pointer new_node)
	{
		new_node->left = NULL;
		new_node->right = NULL;
		new_node->parent = parent;
		child = new_node;
		if (_begin_node()->left)
			_begin_node() = static_cast<node_pointer>(_begin_node()->left);
		_tree_insert_rebalance(root(), child);
		++_size;
		return (iterator(child));
	}

	template <typename T, class Compare, class Allocator>
	ft::pair<typename _tree<T, Compare, Allocator>::iterator, bool>
		_tree<T, Compare, Allocator>::insert_unique(const value_type& value)
	{
		node_pointer	parent = end_node();
		node_pointer&	child = static_cast<node_pointer>(parent->left);	// ref?
		bool			is_less = true;
		iterator		parent_it;

		while (child) {
			parent = child;
			child = static_cast<node_pointer>(parent->left);
			is_less = _comp(value, parent->data);
			if (!is_less)
				child = parent->right;
		}
		parent_it = iterator(parent);
		if (is_less) {
			if (parent_it == begin())
				return (ft::pair<iterator, bool>(_insert_node_at(parent, child,
								_construct_node(value)), true));
			else
				--parent_it;
		}
		if (_comp(*parent_it, value))
			return (ft::pair<iterator, bool>(_insert_node_at(parent, child,
							_construct_node(value)), true));
		return (ft::pair<iterator, bool>(parent_it, false));
	}

	template <typename T, class Compare, class Allocator>
	typename _tree<T, Compare, Allocator>::iterator
		_tree<T, Compare, Allocator>::_insert_unique_after(iterator hint,
														const value_type& value)
	{
		iterator		next = std::advance(hint, 1);
		node_pointer	parent;

		if (next == end() || _comp(value, *next)) {
			if (!hint._ptr->right) {
				parent = static_cast<node_pointer>(hint._ptr);
				return (_insert_node_at(parent, parent->right,
							_construct_node(value)));
			} else {
				parent = static_cast<node_pointer>(next._ptr);
				return (_insert_node_at(parent, parent->left,
							_construct_node(value)));
			}
		}
		return (insert_unique(value).first);
	}
	template <typename T, class Compare, class Allocator>
	typename _tree<T, Compare, Allocator>::iterator
		_tree<T, Compare, Allocator>::insert_unique(iterator hint,
													const value_type& value)
	{	// no hint < val < next check in libstdcpp!?!
		iterator		prev = hint;
		node_pointer	parent;

		if (hint != end() && _comp(*hint, value))
			return (_insert_unique_after(hint, value));
		else if (hint == end() || _comp(value, *hint)) {
			if (hint == begin() || _comp(*--prev, value)) {
				if (!hint._ptr->left) {
					parent = static_cast<node_pointer>(hint._ptr);
					return (_insert_node_at(parent, parent->left,
								_construct_node(value)));
				} else {
					parent = static_cast<node_pointer>(prev._ptr);
					return (_insert_node_at(parent, parent->right,
								_construct_node(value)));
				}
			}
			return (insert_unique(value).first);
		}
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
		node_pointer	node = root();
		node_pointer	result = end_node();

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
		node_const_pointer	node = root();
		node_const_pointer	result = end_node();

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
		node_pointer	node = root();
		node_pointer	result = end_node();

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
		node_const_pointer	node = root();
		node_const_pointer	result = end_node();

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

		node_pointer	node = root();
		node_pointer	result = end_node();

		while (node) {
			if (_comp(key, node->data)) {
				result = node;
				node = static_cast<node_pointer>(node->left);
			}
			else if (_comp(node->data, key))
				node = static_cast<node_pointer>(node->right);
			else if (node->right) {
				result = static_cast<node_pointer>(_tree_min(node->right));
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

		node_const_pointer	node = root();
		node_const_pointer	result = end_node();

		while (node) {
			if (_comp(key, node->data)) {
				result = node;
				node = static_cast<node_const_pointer>(node->left);
			}
			else if (_comp(node->data, key))
				node = static_cast<node_const_pointer>(node->right);
			else if (node->right) {
				result = static_cast<node_const_pointer>(_tree_next(node));
				return (pair_t(const_iterator(node), const_iterator(result)));
			} else
				return (pair_t(const_iterator(node), const_iterator(result)));
		}
		return (pair_t(const_iterator(result), const_iterator(result)));
	}
}

#endif
