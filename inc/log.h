#ifndef TRON_LOG_H
#define TRON_LOG_H

#include <iostream>

namespace tron
{
	template <char... seq>
	using tstring = std::integer_sequence<char, seq...>;

	template<typename T, T... seq>
	constexpr tstring<seq...> operator ""_t() { return {}; }

	template<typename>
	struct Log;

	template<char... header>
	struct Log<tstring<header...>>
	{
		static constexpr char value[sizeof...(header) + 1] = { header..., 0 };

		std::ostream& err()
		{
			return (std::cout << "\x1b[31m[ERR] [" << value << "] ");
		}

		std::ostream& wrn()
		{
			return (std::cout << "\x1b[33m[WRN] [" << value << "] ");
		}

		std::ostream& out()
		{
			return (std::cout << "\x1b[32m[INF] [" << value << "] ");
		}
	};

	namespace log
	{
		static Log<decltype("GL"_t)>   gl;
		static Log<decltype("GLFW"_t)> fw;

		void LogGLParameters();
	}
}

#endif
