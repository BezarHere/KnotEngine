#pragma once
#include "Defs.hpp"

namespace indir
{
	static constexpr size_t ExceptionBufferSz = 1024;

	// a struct that can be called with any arguments, but it does not do anything
	struct DryCallable
	{

		template <typename... _Args>
		inline void operator()(_Args &&...) const noexcept {
		}

	};

	struct EmptyCTor
	{
		template <class T>
		inline void operator()(T &val) const noexcept {
			new (&val) T();
		}
	};

	struct DTor
	{
		template <class T>
		inline void operator()(T &val) const noexcept {
			val.~T();
		}
	};

	struct GenericCTor
	{
		inline GenericCTor(const void *p_ptr = nullptr) : ptr{p_ptr} {}

		template <class T>
		inline void operator()(T &val) const noexcept {
			new (&val) T(*reinterpret_cast<const T *>(ptr));
		}

		const void *ptr;
	};

	struct GenericAssign
	{
		inline GenericAssign(const void *p_ptr = nullptr) : ptr{p_ptr} {}

		template <class T>
		inline void operator()(T &val) const noexcept {
			val = *reinterpret_cast<const T *>(ptr);
		}

		const void *ptr;
	};

	template <typename T, typename NameType = const char *>
	struct NamedValue
	{
		NameType name;
		T value;
	};

	template <typename T>
	struct EqualTo
	{
		inline EqualTo(T &&_value) : value{_value} {}

		template <typename E>
		inline bool operator()(E &&other) const { return other == value; }

		template <typename E>
		inline bool operator()(const E &other) const { return other == value; }

		T value;
	};

	template <typename Op>
	struct InvertOp
	{
		inline InvertOp(Op &&_op) : _operator{_op} {}

		template <typename T>
		inline bool operator()(T &&arg) const { return !_operator(arg); }

		Op _operator;
	};

}



