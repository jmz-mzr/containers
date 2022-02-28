#ifndef STACK_HPP
# define STACK_HPP

# include "vector.hpp"

namespace	ft
{
/*
** In order for the compiler and the linker to find the instantiated operators
** definitions, we either forward declare the class and the operators templates
** before declaring the operators as friend function template specializations,
** or we declare all the instantiations of the operator templates as friend
** (see https://bit.ly/3MggOiw and https://bit.ly/3IDt6Q2)
** The former protects the operators from being specialized (in another header)
** to access all protected/private members of other types thanks to the
** 'universal' friendship granted to all template instantiations, an exploit
** against which the libc++ and libstdc++ implementations aren't protected
** (see examples in "hackFtStack.hpp" and "hackStdStack.hpp")
*/

//	template <typename T, class Container>
//	class	stack;
//	template <typename T, class Container>
//	bool	operator==(const stack<T, Container>&, const stack<T, Container>&);
//	template <typename T, class Container>
//	bool	operator<(const stack<T, Container>&, const stack<T, Container>&);

	template <typename T, class Container = ft::vector<T> >
	class	stack {
	public:
		typedef Container						container_type;
		typedef typename Container::value_type	value_type;
		typedef typename Container::size_type	size_type;

		explicit stack(const Container& ctnr = Container()): c(ctnr) { }

		bool				empty(void) const { return (c.empty()); }
		size_type			size(void) const { return (c.size()); }
		value_type&			top(void) { return (c.back()); }
		const value_type&	top(void) const { return (c.back()); }
		void				push(const value_type& value) {
														c.push_back(value); }
		void				pop(void) { c.pop_back(); }

//		friend bool	operator== <>(const stack& lhs, const stack& rhs);
//		friend bool	operator< <>(const stack& lhs, const stack& rhs);

		template <typename T1, class C1>
		friend bool	operator==(const stack<T1, C1>& lhs,
								const stack<T1, C1>&rhs);
		template <typename T1, class C1>
		friend bool	operator<(const stack<T1, C1>& lhs,
								const stack<T1, C1>&rhs);
	protected:
		Container	c;
	};

	/**************************************************************************/
	/*                     NON-MEMBER OPERATOR OVERLOADS                      */
	/**************************************************************************/

	template <typename T, class Container>
	bool	operator==(const stack<T, Container>& lhs,
						const stack<T, Container>& rhs)
	{
		return (lhs.c == rhs.c);
	}
	template <typename T, class Container>
	bool	operator!=(const stack<T, Container>& lhs,
						const stack<T, Container>& rhs)
	{
		return (!(lhs == rhs));
	}
	template <typename T, class Container>
	bool	operator<(const stack<T, Container>& lhs,
						const stack<T, Container>& rhs)
	{
		return (lhs.c < rhs.c);
	}
	template <typename T, class Container>
	bool	operator<=(const stack<T, Container>& lhs,
						const stack<T, Container>& rhs)
	{
		return (!(rhs < lhs));
	}
	template <typename T, class Container>
	bool	operator>(const stack<T, Container>& lhs,
						const stack<T, Container>& rhs)
	{
		return (rhs < lhs);
	}
	template <typename T, class Container>
	bool	operator>=(const stack<T, Container>& lhs,
						const stack<T, Container>& rhs)
	{
		return (!(lhs < rhs));
	}
}

#endif
