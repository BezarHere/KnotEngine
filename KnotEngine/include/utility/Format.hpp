#pragma once
#include <sstream>

namespace kt
{
		// for empty args
	static inline void _format_join(std::ostream &stream) {
		(void)stream;
	}

	template <typename T>
	static inline void _format_join(std::ostream &stream, const T &value) {
		stream << value;
	}

	template <typename T1, typename T2, typename... VArgs>
	static inline void _format_join(std::ostream &stream, const T1 &value1, const T2 &value2, const VArgs &... args) {
		stream << value1;
		return _format_join(stream, value2, args...);
	}

	template <typename... VArgs>
	static inline std::string format_join(const VArgs &... args) {
		std::ostringstream ss{};
		_format_join(ss, args...);
		return ss.str();
	}
}
