#pragma once
#include "FieldVar.hpp"

namespace kt
{
	class FieldFile
	{
	public:
		static FieldVar load(const string &filepath);
		static FieldVar read(const char *source, size_t length);

		static void dump(const string &filepath, const FieldVar::Dict &data);
		static string write(const FieldVar::Dict &data);

	};
}