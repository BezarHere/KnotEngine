#include "utility/Defs.hpp"
#include "math/Vector.hpp"
#include "math/Transform.hpp"
#include "math/Quaternion.hpp"

namespace kt
{
	namespace traits
	{

		template <typename _T>
		struct SpaceTraits;

		template <>
		struct SpaceTraits<Transform3D>
		{
			typedef Transform3D transform_type;
			typedef decltype(transform_type::position) direction_type;
			typedef decltype(direction_type::x) value_type;
			
			typedef Quaternion rotation_type;
		};

		template <>
		struct SpaceTraits<Transform2D>
		{
			typedef Transform2D transform_type;
			typedef decltype(transform_type::position) direction_type;
			typedef decltype(direction_type::x) value_type;
			

			typedef value_type rotation_type;
		};

	}

	typedef traits::SpaceTraits<Transform3D> Space3DTraits;
	typedef traits::SpaceTraits<Transform2D> Space2DTraits;
}
