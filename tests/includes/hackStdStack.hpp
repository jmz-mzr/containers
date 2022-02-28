#ifndef STACK3_HPP
# define STACK3_HPP

# include <stack>

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
}

#endif
