#pragma once
#include "Base.hpp"

namespace kt
{

	template <typename T>
	struct TVector2
	{
		typedef T value_type;

		inline bool operator<(const TVector2 &other) {
			return x < other.x && y < other.y;
		}

		value_type x, y;
	};

	template <typename T>
	struct TVector3
	{
		typedef T value_type;

		inline bool operator<(const TVector3 &other) {
			return x < other.x && y < other.y && z < other.z;
		}

		value_type x, y, z;
	};

	typedef TVector2<real_t> Vec2;
	typedef TVector2<int> Vec2i;
	typedef TVector2<unsigned> Vec2u;
	typedef TVector2<short> Vec2s;
	typedef TVector2<unsigned short> Vec2us;

	typedef TVector3<real_t> Vec3;
	typedef TVector3<int> Vec3i;
	typedef TVector3<unsigned> Vec3u;
	typedef TVector3<short> Vec3s;
	typedef TVector3<unsigned short> Vec3us;
};
