#ifndef HACKSTDSTACK_HPP
# define HACKSTDSTACK_HPP

# include <stack>
# include <vector>

namespace	std
{
	struct	uniq: public std::string {
	};

	template<>
	bool	operator==(const stack<std::uniq, std::vector<std::uniq> >&,
						const stack<std::uniq, std::vector<std::uniq> >&)
	{
		std::stack<int>		stack;

		stack.c.push_back(11);
		std::cout << "[stack.c.back() = " << stack.c.back() << "] ";
		return (false);
	}
	
	extern std::stack<int>	stackI;

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
