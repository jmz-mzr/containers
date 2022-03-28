#ifndef VECTOR_HPP
# define VECTOR_HPP

# include <memory>
# include <algorithm>
# include <stdexcept>
# include <limits>
# include <sstream>
# include "enable_if.hpp"
# include "is_integral.hpp"
# include "iterator_traits.hpp"
# include "reverse_iterator.hpp"
# include "equal.hpp"
# include "lexicographical_compare.hpp"
# include "detect_system.hpp"

# define LENGTH_ERR		"vector"
# define RANGE_ERR		"vector"

namespace	ft
{
	template <typename T, class Allocator = std::allocator<T> >
	class	vector {
	public:
		typedef T										value_type;
		typedef Allocator								allocator_type;
		typedef typename Allocator::reference			reference;
		typedef typename Allocator::const_reference		const_reference;
		typedef typename Allocator::pointer				pointer;
		typedef typename Allocator::const_pointer		const_pointer;
		typedef typename Allocator::size_type			size_type;
		typedef typename Allocator::difference_type		difference_type;
		typedef pointer									iterator;
		typedef const_pointer							const_iterator;
		typedef ft::reverse_iterator<iterator>			reverse_iterator;
		typedef ft::reverse_iterator<const_iterator>	const_reverse_iterator;

		explicit vector(const Allocator& alloc = Allocator()):
					_alloc(alloc), _begin(NULL), _end(NULL), _limit(NULL) { }
		explicit vector(size_type n, const T& value = T(),
						const Allocator& alloc = Allocator());
		template <typename InputIt>
		vector(InputIt first, InputIt last,
				const Allocator& alloc = Allocator(),
				typename ft::enable_if<!ft::is_integral<
				InputIt>::value, void*>::type = NULL);
		vector(const vector& src);
		~vector(void);

		vector&		operator=(const vector& rhs);

		iterator				begin(void) { return (iterator(_begin)); }
		const_iterator			begin(void) const {
											return (const_iterator(_begin)); }
		iterator				end(void) { return (iterator(_end)); }
		const_iterator			end(void) const {
											return (const_iterator(_end)); }
		reverse_iterator		rbegin(void) {
									return (reverse_iterator(end())); }
		const_reverse_iterator	rbegin(void) const {
									return (const_reverse_iterator(end())); }
		reverse_iterator		rend(void) {
									return (reverse_iterator(begin())); }
		const_reverse_iterator	rend(void) const {
									return (const_reverse_iterator(begin())); }

		bool			empty(void) const { return (_begin == _end); }
		size_type		size(void) const {
							return (static_cast<size_type>(_end - _begin)); }
		size_type		max_size(void) const;
		void			resize(size_type n, T value = T());
		size_type		capacity(void) const {
							return (static_cast<size_type>(_limit - _begin)); }
		void			reserve(size_type n);

		reference				operator[](size_type i) { return (_begin[i]); }
		const_reference			operator[](size_type i) const {
														return (_begin[i]); }
		reference				at(size_type i);
		const_reference			at(size_type i) const;
		reference				front(void) { return (*_begin); }
		const_reference			front(void) const { return (*_begin); }
		reference				back(void) { return (*(_end - 1)); }
		const_reference			back(void) const { return (*(_end - 1)); }

		void			assign(size_type n, const T& value);
		template <typename InputIt>
		typename ft::enable_if<!ft::is_integral<InputIt>::value, void>::type
						assign(InputIt first, InputIt last);
		void			push_back(const T& value);
		void			pop_back(void) { _destruct_at_end(_end - 1); }
		iterator		insert(iterator pos, const T& value);
		void			insert(iterator pos, size_type n, const T& value);
		template <typename InputIt>
		typename ft::enable_if<!ft::is_integral<InputIt>::value, void>::type
						insert(iterator pos, InputIt first, InputIt last);
		void			swap(vector& other);
		iterator		erase(iterator pos);
		iterator		erase(iterator first, iterator last);
		void			clear(void) { _destruct_at_end(_begin); }

		Allocator		get_allocator(void) const { return (_alloc); }
	private:
		void			_allocate(size_type n);
		void			_deallocate(void);
		void			_reallocate(size_type new_size);
		pointer			_reallocate(size_type new_size, pointer insert_pos,
									const T& value, size_type n = 1);
		template <typename ForwardIt>
		typename ft::enable_if<!ft::is_integral<ForwardIt>::value, void>::type
						_reallocate(size_type new_size, pointer insert_pos,
									ForwardIt first, ForwardIt last);
		size_type		_optimal_size(size_type new_size) const;
		void			_construct_at_end(size_type n, const T& value);
		template <typename Iter>
		typename ft::enable_if<!ft::is_integral<Iter>::value, void>::type
						_construct_at_end(Iter first, Iter last);
		template <typename ForwardIt>
		void			_construct_at_end(ForwardIt first, ForwardIt last,
											std::forward_iterator_tag);
		void			_destruct_at_end(const_pointer new_end);
		void			_range_check(size_type i) const;
		template <typename InputIt>
		void			_assign(InputIt first, InputIt last,
								std::input_iterator_tag);
		template <typename ForwardIt>
		void			_assign(ForwardIt first, ForwardIt last,
								std::forward_iterator_tag);
		void			_advance_range(pointer first, pointer last, pointer to);
		template <typename InputIt>
		void			_insert(iterator pos, InputIt first, InputIt last,
								std::input_iterator_tag);
		template <typename ForwardIt>
		void			_insert(iterator pos, ForwardIt first, ForwardIt last,
								std::forward_iterator_tag);

		Allocator		_alloc;
		pointer			_begin;
		pointer			_end;
		pointer			_limit;
	};

	/**************************************************************************/
	/*                       CONSTRUCTORS / DESTRUCTORS                       */
	/**************************************************************************/

	template <typename T, class Alloc>
	vector<T, Alloc>::vector(size_type n, const T& value, const Alloc& alloc):
						_alloc(alloc), _begin(NULL), _end(NULL), _limit(NULL)
	{
		if (n > 0) {
			_allocate(n);
			_construct_at_end(n, value);
		}
	}

	template <typename T, class Alloc>
	template <typename InputIt>
	vector<T, Alloc>::vector(InputIt first, InputIt last, const Alloc& alloc,
		typename ft::enable_if<!ft::is_integral<InputIt>::value, void*>::type):
						_alloc(alloc), _begin(NULL), _end(NULL), _limit(NULL)
	{
		for ( ; first != last; ++first)
			push_back(*first);
	}

	template <typename T, class Alloc>
	vector<T, Alloc>::vector(const vector& src):
					_alloc(src._alloc), _begin(NULL), _end(NULL), _limit(NULL)
	{
		size_type	n = src.size();

		if (n > 0) {
			_allocate(n);
			_construct_at_end(src._begin, src._end);
		}
	}

	template <typename T, class Alloc>
	vector<T, Alloc>::~vector(void)
	{
		clear();
		_alloc.deallocate(_begin, capacity());
	}

	/**************************************************************************/
	/*                       MEMBER OPERATOR OVERLOADS                        */
	/**************************************************************************/

	template <typename T, class Alloc>
	vector<T, Alloc>&	vector<T, Alloc>::operator=(const vector& rhs)
	{
		if (this != &rhs) {
			if (_alloc != rhs._alloc)
				_deallocate();
			_alloc = rhs._alloc;
			assign(rhs._begin, rhs._end);
		}
		return (*this);
	}

	/**************************************************************************/
	/*                            MEMBER FUNCTIONS                            */
	/**************************************************************************/

	template <typename T, class Alloc>
	void	vector<T, Alloc>::_allocate(size_type n)
	{
		if (!LIBSTDCPP && n > max_size())
			throw std::length_error(LENGTH_ERR);
		_begin = _alloc.allocate(n);
		_end = _begin;
		_limit = _begin + n;
	}

	template <typename T, class Alloc>
	void	vector<T, Alloc>::_deallocate(void)
	{
		if (_begin) {
			clear();
			_alloc.deallocate(_begin, capacity());
			_begin = NULL;
			_end = NULL;
			_limit = NULL;
		}
	}

	template <typename T, class Alloc>
	void	vector<T, Alloc>::_reallocate(size_type new_size)
	{
		pointer		new_begin = _alloc.allocate(new_size);
		pointer		new_end = NULL;

		try {
			new_end = std::uninitialized_copy(_begin, _end, new_begin);
		} catch (...) {
			_alloc.deallocate(new_begin, new_size);
			throw ;
		}
		_deallocate();
		_begin = new_begin;
		_end = new_end;
		_limit = _begin + new_size;
	}
	template <typename T, class Alloc>
	typename vector<T, Alloc>::pointer
		vector<T, Alloc>::_reallocate(size_type new_size, pointer insert_pos,
										const T& value, size_type n)
	{
		pointer		new_begin = _alloc.allocate(new_size);
		pointer		new_pos = NULL;
		pointer		new_end = NULL;

		try {
			new_pos = std::uninitialized_copy(_begin, insert_pos, new_begin);
			new_end = std::uninitialized_copy(insert_pos, _end, new_pos + n);
			std::uninitialized_fill(new_pos, new_pos + n, value);
		} catch (...) {
			while (new_pos && new_pos != new_begin)
				_alloc.destroy(--new_pos);
			while (new_end && new_end != new_pos + n)
				_alloc.destroy(--new_end);
			_alloc.deallocate(new_begin, new_size);
			throw ;
		}
		_deallocate();
		_begin = new_begin;
		_end = new_end;
		_limit = _begin + new_size;
		return (new_pos);
	}
	template <typename T, class Alloc>
	template <typename ForwardIt>
	typename ft::enable_if<!ft::is_integral<ForwardIt>::value, void>::type
		vector<T, Alloc>::_reallocate(size_type new_size, pointer insert_pos,
											ForwardIt first, ForwardIt last)
	{
		pointer		new_begin = _alloc.allocate(new_size);
		pointer		new_pos = NULL;
		pointer		new_end = NULL;

		try {
			new_pos = std::uninitialized_copy(_begin, insert_pos, new_begin);
			new_end = std::uninitialized_copy(first, last, new_pos);
			new_end = std::uninitialized_copy(insert_pos, _end, new_end);
		} catch (...) {
			while (new_pos && new_pos != new_begin)
				_alloc.destroy(--new_pos);
			while (new_end && new_end != new_pos)
				_alloc.destroy(--new_end);
			_alloc.deallocate(new_begin, new_size);
			throw ;
		}
		_deallocate();
		_begin = new_begin;
		_end = new_end;
		_limit = _begin + new_size;
	}

	template <typename T, class Alloc>
	void	vector<T, Alloc>::resize(size_type n, T value)
	{
		size_type	size = this->size();

		if (n == size)
			return ;
		if (n < size)
			_destruct_at_end(_begin + n);
		else if (n <= capacity())
			_construct_at_end(n - size, value);
		else if (LIBSTDCPP && n > max_size())
			throw std::length_error("vector::_M_fill_insert");
		else
			_reallocate(_optimal_size(n), _end, value, n - size);
	}

	/*
	** Up until recently, libc++ did not correctly implemented the standard
	** for the "std::vector<T>::reserve" function and did not throw the
	** "std::length_error" exception when "n > max_size()" but instead
	** propagated the exception thrown from "allocator<T>::allocate"
	** This behaviour is reproduced to conform to the subject
	*/
	template <typename T, class Alloc>
	void	vector<T, Alloc>::reserve(size_type n)
	{
		if (LIBSTDCPP && n > max_size())
			throw std::length_error("vector::reserve");
		if (n > capacity())
			_reallocate(n);
	}

	template <typename T, class Alloc>
	typename vector<T, Alloc>::size_type vector<T, Alloc>::max_size(void) const
	{
		return (std::min(_alloc.max_size(),
					std::numeric_limits<size_type>::max() / 2));
	}

	/*
	** Libc++ expects the "new_size" parameter to be the total new_size
	** we are looking for, and it returns it optimized, whereas
	** libstdc++ expects the "new_size" parameter to be the additional
	** size we need, and it returns an optimized total new_size
	*/
	template <typename T, class Alloc>
	typename vector<T, Alloc>::size_type
		vector<T, Alloc>::_optimal_size(size_type new_size) const
	{
		const size_type		size = this->size();
		const size_type		max_size = this->max_size();
		const size_type		capacity = this->capacity();

		if (LIBSTDCPP) {
			new_size = size + std::max(size, new_size - size);
			if (new_size < size || new_size > max_size)
				return (max_size);
			return (new_size);
		}
		if (new_size > max_size)
			throw std::length_error(LENGTH_ERR);
		if (capacity >= max_size / 2)
			return (max_size);
		return (std::max(2 * capacity, new_size));
	}

	template <typename T, class Alloc>
	void	vector<T, Alloc>::_construct_at_end(size_type n, const T& value)
	{
		while (n > 0) {
			_alloc.construct(_end, value);
			++_end;
			--n;
		}
	}
	template <typename T, class Alloc>
	template <typename ForwardIt>
	void	vector<T, Alloc>::_construct_at_end(ForwardIt first, ForwardIt last,
												std::forward_iterator_tag)
	{
		for ( ; first != last; ++first) {
			_alloc.construct(_end, *first);
			++_end;
		}
	}
	template <typename T, class Alloc>
	template <typename Iter>
	typename ft::enable_if<!ft::is_integral<Iter>::value, void>::type
		vector<T, Alloc>::_construct_at_end(Iter first, Iter last)
	{
		_construct_at_end(first, last,
				typename ft::iterator_traits<Iter>::iterator_category());
	}

	template <typename T, class Alloc>
	void	vector<T, Alloc>::_destruct_at_end(const_pointer new_end)
	{
		while (_end != new_end)
			_alloc.destroy(--_end);
	}

	template <typename T, class Alloc>
	void	vector<T, Alloc>::_range_check(size_type i) const
	{
		size_type			size = this->size();
		std::ostringstream	error;

		if (LIBSTDCPP && i >= size) {
			error << "vector::_M_range_check: __n (which is " << i
				<< ") >= this->size() (which is " << size << ")";
			throw std::out_of_range(error.str());
		}
		else if (i >= size)
			throw std::out_of_range(RANGE_ERR);
	}

	template <typename T, class Alloc>
	typename vector<T, Alloc>::reference
		vector<T, Alloc>::at(size_type i)
	{
		_range_check(i);
		return (_begin[i]);
	}
	template <typename T, class Alloc>
	typename vector<T, Alloc>::const_reference
		vector<T, Alloc>::at(size_type i) const
	{
		_range_check(i);
		return (_begin[i]);
	}

	template <typename T, class Alloc>
	void	vector<T, Alloc>::assign(size_type n, const T& value)
	{
		size_type	size = this->size();

		if (n <= capacity()) {
			std::fill_n(_begin, std::min(n, size), value);
			if (n > size)
				_construct_at_end(n - size, value);
			else
				_destruct_at_end(_begin + n);
		} else {
			if (!LIBSTDCPP || n <= max_size())
				_deallocate();
			if (LIBSTDCPP)
				_allocate(n);
			else
				_allocate(_optimal_size(n));
			_construct_at_end(n, value);
		}
	}
	template <typename T, class Alloc>
	template <typename InputIt>
	void	vector<T, Alloc>::_assign(InputIt first, InputIt last,
										std::input_iterator_tag)
	{
		clear();
		for ( ; first != last; ++first)
			push_back(*first);
	}
	template <typename T, class Alloc>
	template <typename ForwardIt>
	void	vector<T, Alloc>::_assign(ForwardIt first, ForwardIt last,
										std::forward_iterator_tag)
	{
		size_type	new_size =
							static_cast<size_type>(std::distance(first, last));
		bool		growing = false;
		ForwardIt	middle = last;
		pointer		new_end;

		if (new_size <= capacity()) {
			if (new_size > size()) {
				growing = true;
				middle = first;
				std::advance(middle, size());
			}
			new_end = std::copy(first, middle, _begin);
			if (growing)
				_construct_at_end(middle, last);
			else
				_destruct_at_end(new_end);
		} else if (LIBSTDCPP && new_size > max_size()) {
			throw std::length_error("cannot create std::vector"
					"larger than max_size()");
		} else {
			_deallocate();
			_allocate(_optimal_size(new_size));
			_construct_at_end(first, last);
		}
	}
	template <typename T, class Alloc>
	template <typename InputIt>
	typename ft::enable_if<!ft::is_integral<InputIt>::value, void>::type
		vector<T, Alloc>::assign(InputIt first, InputIt last)
	{
		_assign(first, last,
				typename ft::iterator_traits<InputIt>::iterator_category());
	}

	template <typename T, class Alloc>
	void	vector<T, Alloc>::push_back(const T& value)
	{
		if (LIBSTDCPP && size() == max_size())
			throw std::length_error("_M_realloc_insert");
		if (_end == _limit)
			_reallocate(_optimal_size(size() + 1));
		_alloc.construct(_end, value);
		++_end;
	}

	template <typename T, class Alloc>
	void	vector<T, Alloc>::_advance_range(pointer first, pointer last,
											pointer to)
	{
		pointer				old_end = _end;
		difference_type		constructed = _end - to;

		for (pointer current = first + constructed; current < last;
				++current, ++_end)
			_alloc.construct(_end, *current);
		std::copy_backward(first, first + constructed, old_end);
	}

	template <typename T, class Alloc>
	typename vector<T, Alloc>::iterator
		vector<T, Alloc>::insert(iterator pos, const T& value)
	{
		const_pointer	value_ptr = &value;

		if (_end < _limit) {
			if (pos == _end) {
				_alloc.construct(_end, value);
				++_end;
			} else {
				_advance_range(pos, _end, pos + 1);
				if (pos <= value_ptr && value_ptr < _end)
					++value_ptr;
				*pos = *value_ptr;
			}
		} else if (LIBSTDCPP && size() == max_size())
			throw std::length_error("_M_realloc_insert");
		else
			pos = _reallocate(_optimal_size(size() + 1), pos, value);
		return (pos);
	}
	template <typename T, class Alloc>
	void	vector<T, Alloc>::insert(iterator pos, size_type n, const T& value)
	{
		size_type		constructed = static_cast<size_type>(_end - pos);
		pointer			old_end = _end;
		size_type		old_n = n;
		const_pointer	value_ptr = &value;

		if (n > 0) {
			if (static_cast<size_type>(_limit - _end) >= n) {
				if (constructed < n) {
					_construct_at_end(n - constructed, value);
					n = constructed;
				}
				if (n > 0) {
					_advance_range(pos, old_end, pos + old_n);
					if (pos <= value_ptr && value_ptr < _end)
						value_ptr += old_n;
					std::fill_n(pos, n, *value_ptr);
				}
			} else if (LIBSTDCPP && n > (max_size() - size()))
				throw std::length_error("vector::_M_fill_insert");
			else
				_reallocate(_optimal_size(size() + n), pos, value, n);
		}
	}
	template <typename T, class Alloc>
	template <typename InputIt>
	void	vector<T, Alloc>::_insert(iterator pos, InputIt first, InputIt last,
										std::input_iterator_tag)
	{
		difference_type		offset = pos - _begin;
		size_type			old_size = size();

		if (first != last) {
			try {
				for ( ; first != last; ++first)
					push_back(*first);
			} catch (...) {
				erase(_begin + old_size, _end);
				throw ;
			}
			std::rotate(_begin + offset, _begin + old_size, _end);
		}
	}
	template <typename T, class Alloc>
	template <typename ForwardIt>
	void	vector<T, Alloc>::_insert(iterator pos, ForwardIt first,
									ForwardIt last, std::forward_iterator_tag)
	{
		difference_type		n = std::distance(first, last);
		difference_type		constructed = _end - pos;
		ForwardIt			middle = last;
		pointer				old_end = _end;
		difference_type		old_n = n;

		if (n > 0) {
			if ((_limit - _end) >= n) {
				if (constructed < n) {
					middle = first;
					std::advance(middle, constructed);
					_construct_at_end(middle, last);
					n = constructed;
				}
				if (n > 0) {
					_advance_range(pos, old_end, pos + old_n);
					std::copy(first, middle, pos);
				}
			} else if (LIBSTDCPP && (size_type)n > (max_size() - size()))
				throw std::length_error("vector::_M_range_insert");
			else
				_reallocate(_optimal_size(size() + n), pos, first, last);
		}
	}
	template <typename T, class Alloc>
	template <typename InputIt>
	typename ft::enable_if<!ft::is_integral<InputIt>::value, void>::type
		vector<T, Alloc>::insert(iterator pos, InputIt first, InputIt last)
	{
		_insert(pos, first, last,
				typename ft::iterator_traits<InputIt>::iterator_category());
	}

	template <typename T, class Alloc>
	void	vector<T, Alloc>::swap(vector& other)
	{
		std::swap(_alloc, other._alloc);
		std::swap(_begin, other._begin);
		std::swap(_end, other._end);
		std::swap(_limit, other._limit);
	}

	template <typename T, class Alloc>
	typename vector<T, Alloc>::iterator
		vector<T, Alloc>::erase(iterator pos)
	{
		_destruct_at_end(std::copy(pos + 1, _end, pos));
		return (pos);
	}

	template <typename T, class Alloc>
	typename vector<T, Alloc>::iterator
		vector<T, Alloc>::erase(iterator first, iterator last)
	{
		_destruct_at_end(std::copy(last, _end, first));
		return (first);
	}

	/**************************************************************************/
	/*                          NON-MEMBER FUNCTIONS                          */
	/**************************************************************************/

	template <typename T, class Alloc>
	void	swap(vector<T, Alloc>& lhs, vector<T, Alloc>& rhs)
	{
		lhs.swap(rhs);
	}

	/**************************************************************************/
	/*                     NON-MEMBER OPERATOR OVERLOADS                      */
	/**************************************************************************/

	template <typename T, class Alloc>
	bool	operator==(const vector<T, Alloc>& lhs, const vector<T, Alloc>& rhs)
	{
		return (lhs.size() == rhs.size()
				&& ft::equal(lhs.begin(), lhs.end(), rhs.begin()));
	}
	template <typename T, class Alloc>
	bool	operator!=(const vector<T, Alloc>& lhs, const vector<T, Alloc>& rhs)
	{
		return (!(lhs == rhs));
	}
	template <typename T, class Alloc>
	bool	operator<(const vector<T, Alloc>& lhs, const vector<T, Alloc>& rhs)
	{
		return (ft::lexicographical_compare(lhs.begin(), lhs.end(),
					rhs.begin(), rhs.end()));
	}
	template <typename T, class Alloc>
	bool	operator<=(const vector<T, Alloc>& lhs, const vector<T, Alloc>& rhs)
	{
		return (!(rhs < lhs));
	}
	template <typename T, class Alloc>
	bool	operator>(const vector<T, Alloc>& lhs, const vector<T, Alloc>& rhs)
	{
		return (rhs < lhs);
	}
	template <typename T, class Alloc>
	bool	operator>=(const vector<T, Alloc>& lhs, const vector<T, Alloc>& rhs)
	{
		return (!(lhs < rhs));
	}
}

#endif
