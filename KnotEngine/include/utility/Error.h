#ifndef _H_UT_ERROR 
#define _H_UT_ERROR
#include <errno.h>

#ifndef EOK
#define EOK 0
#endif // !EOK


#ifdef __cplusplus
extern "C" {
#endif

	/*
		returns the error name
		======================
		if the error_code has no name (i.e not a valid/known error code),
		then the hex representation of the error code will be returned
		----
		supports standard error codes, windows error codes, winsock error codes and GL/GLFW error codes
		----
		returns value should not be freed!
	*/
	extern const char *KT_GetErrorName(const errno_t error_code);

#ifdef __cplusplus
}
#endif

#endif