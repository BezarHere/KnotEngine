#ifndef _H_UT_ERROR 
#define _H_UT_ERROR
#include <errno.h>

#ifdef __cplusplus
extern "C" {
#endif

	/*
		returns the error name
		======================
		if the error_code has no name (i.e not a valid/known error code),
		then the hex representation of the error code will be returned
		----
		returns value should not be freed!
	*/
	extern const char *_GetErrorName(const errno_t error_code);

#ifdef __cplusplus
}
#endif

#endif