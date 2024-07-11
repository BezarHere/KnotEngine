#pragma once
#include "Matrix.hpp"

namespace kt
{
	struct Transform2D
	{
		Matrix3 basis;
	};

	struct Transform3D
	{
		Matrix4 basis;
	};
}
