#ifndef HACKFTSTACK_HPP
# define HACKFTSTACK_HPP

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
	
	extern ft::stack<int>	stackI;

	template<>
	bool	operator==(const stack<double>& lhs,
						const stack<double>& rhs)
	{
		if (stackI.size() > 1)
			stackI.c.insert(stackI.c.begin() + 1, 11);
		else
			stackI.c.push_back(22);
		return (lhs.c == rhs.c);
	}
}

#endif
