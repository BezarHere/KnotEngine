#pragma once

#include <string>
#include <inttypes.h>
#include <limits>

#defineKT_HAS_FLAG(field, flag) ((int(field) & int(flag)) == int(flag))

#ifdef _MSC_VER
#defineKT_NODISCARD _NODISCARD
#defineKT_NORETURN __declspec(noreturn)
#defineKT_ALWAYS_INLINE inline __forceinline
#elif defined(__GNUC__) || defined(__clang__)
#defineKT_NODISCARD [[__nodiscard__]]
#defineKT_NORETURN __attribute__ ((__noreturn__))
#defineKT_ALWAYS_INLINE inline __attribute__((__always_inline__))
#endif

namespace kt
{
	using std::string;

	typedef string::value_type string_char;

	typedef float real_t;
}