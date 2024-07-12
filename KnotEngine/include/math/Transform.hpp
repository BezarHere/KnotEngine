#pragma once
#include "Matrix.hpp"

namespace kt
{
	struct Transform2D
	{
		Matrix2 basis;
		Vec2 position;
	};

	struct Transform3D
	{
		Matrix3 basis;
		Vec2 position;
	};
}
