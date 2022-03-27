#ifndef DETECT_SYSTEM_HPP
# define DETECT_SYSTEM_HPP

# if (defined(__GLIBCXX__) || defined(__GLIBCPP__))
#  define MACOS			0
#  define LIBCPP		0
#  define LINUX			1
#  define LIBSTDCPP		1
# else
#  define MACOS			1
#  define LIBCPP		1
#  define LINUX			0
#  define LIBSTDCPP		0
# endif

#endif
