#pragma once
#include "Defs.hpp"

#include <stdio.h>
#include <stdarg.h>

#include "Error.h"

#define KT_LOG_ERR(err_code, msg) ::kt::Log::error(err_code, "%s:%d:" msg, __FUNCTION__, __LINE__);
#define KT_LOG_ERR_V(err_code, msg, ...) ::kt::Log::error(err_code, "%s:%d:" msg, __FUNCTION__, __LINE__, __VA_ARGS__);

namespace kt
{

	class Log
	{
	public:

		static inline errno_t error(errno_t code, const string_char *str, ...) {
			static FILE *const output = stderr;

			if (s_UseColor)
			{
				fputs("\033[31m", output);
			}

			if (code == 0)
			{
				fputs("ERR", output);
			}
			else
			{
				fprintf(output, "ERR[%s]:", KT_GetErrorName(code));
			}

			va_list list;
			va_start(list, str);
			vfprintf(output, str, list);
			va_end(list);

			if (s_UseColor)
			{
				fputs("\n\033[0m", output);
			}

			return code;
		};

		static inline errno_t warning(errno_t code, const string_char *str, ...) {
			static FILE *const output = stdout;

			if (s_UseColor)
			{
				fputs("\033[33m", output);
			}

			if (code == 0)
			{
				fputs("WRN", output);
			}
			else
			{
				fprintf(output, "WRN[%s]:", KT_GetErrorName(code));
			}

			va_list list;
			va_start(list, str);
			vfprintf(output, str, list);
			va_end(list);

			if (s_UseColor)
			{
				fputs("\n\033[0m", output);
			}

			return code;
		};

	private:
		static inline bool s_UseColor = true;
	};

}