#ifndef STACK2_HPP
# define STACK2_HPP

# include "stack.hpp"

namespace	ft
{
	struct	uniq: public std::string {
	};

	template<>
	bool	operator==(const stack<ft::uniq, ft::vector<ft::uniq> >&,
						const stack<ft::uniq, ft::vector<ft::uniq> >&)
	{
		ft::stack<int>		stack;

		stack.c.push_back(11);
		std::cout << "[stack.c.back() = " << stack.c.back() << "] ";
		return (false);
	}
}

#endif
