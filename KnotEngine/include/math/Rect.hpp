#pragma once
#include "Vector.hpp"

namespace kt
{
	template <typename T>
	struct TRectangle
	{
		typedef T value_type;
		typedef TVector2<value_type> vector_type;
		

		vector_type position;
		vector_type size;
	};

	template <typename T>
	struct TBox
	{
		typedef T value_type;
		typedef TVector3<value_type> vector_type;
		

		vector_type position;
		vector_type size;
	};

	typedef TRectangle<real_t> Rect;
	typedef TRectangle<int> RectI;
	typedef TRectangle<short> RectS;
	typedef TRectangle<unsigned> RectU;

	typedef TBox<real_t> Box;
	typedef TBox<int> BoxI;
	typedef TBox<short> BoxS;
	typedef TBox<unsigned> BoxU;

}
