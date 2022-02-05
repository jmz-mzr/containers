#ifndef IS_INTEGRAL_HPP
# define IS_INTEGRAL_HPP

namespace	ft
{
	template <typename T, T v>
	struct	integral_constant {
		typedef T					value_type;
		typedef integral_constant	type;
		static const T				value = v;
		operator					value_type(void) { return (value); }
	};

	typedef integral_constant<bool, false>	false_type;
	typedef integral_constant<bool, true>	true_type;

	template<typename T>
	struct	remove_cv {
		typedef T	type;
	};
	template<typename T>
	struct	remove_cv<const T> {
		typedef T	type;
	};
	template<typename T>
	struct	remove_cv<volatile T> {
		typedef T	type;
	};
	template<typename T>
	struct	remove_cv<const volatile T> {
		typedef T	type;
	};

	template <typename T>
	struct	is_integral_base: public false_type {
	};
	template <>
	struct	is_integral_base<bool>: public true_type {
	};
	template <>
	struct	is_integral_base<char>: public true_type {
	};
	template <>
	struct	is_integral_base<signed char>: public true_type {
	};
	template <>
	struct	is_integral_base<unsigned char>: public true_type {
	};
	template <>
	struct	is_integral_base<char16_t>: public true_type {
	};
	template <>
	struct	is_integral_base<char32_t>: public true_type {
	};
	template <>
	struct	is_integral_base<wchar_t>: public true_type {
	};
	template <>
	struct	is_integral_base<short>: public true_type {
	};
	template <>
	struct	is_integral_base<unsigned short>: public true_type {
	};
	template <>
	struct	is_integral_base<int>: public true_type {
	};
	template <>
	struct	is_integral_base<unsigned int>: public true_type {
	};
	template <>
	struct	is_integral_base<long>: public true_type {
	};
	template <>
	struct	is_integral_base<unsigned long>: public true_type {
	};
	template <>
	struct	is_integral_base<long long>: public true_type {
	};
	template <>
	struct	is_integral_base<unsigned long long>: public true_type {
	};

	template <typename T>
	struct	is_integral: public is_integral_base<typename remove_cv<T>::type> {
	};
}

#endif
