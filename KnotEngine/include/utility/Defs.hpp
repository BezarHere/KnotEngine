#pragma once

#include <string>
#include <inttypes.h>
#include <limits>

#define KT_HAS_FLAG(field, flag) ((int(field) & int(flag)) == int(flag))

#ifdef _MSC_VER
#define KT_NODISCARD _NODISCARD
#define KT_NORETURN __declspec(noreturn)
#define KT_ALWAYS_INLINE inline __forceinline
#elif defined(__GNUC__) || defined(__clang__)
#define KT_NODISCARD [[__nodiscard__]]
#define KT_NORETURN __attribute__ ((__noreturn__))
#define KT_ALWAYS_INLINE inline __attribute__((__always_inline__))
#endif

namespace kt
{
	static constexpr size_t npos = static_cast<size_t>(-1);

	using std::string;

	typedef string::value_type string_char;

	typedef float real_t;
}