#include <stdio.h>

#ifndef TYPEDEF_H
#define TYPEDEF_H

typedef enum _Ret                                //定义函数返回  
{
	RET_OK,
	RET_OOM,
	RET_STOP,
	RET_INVALID_PARAMS,
	RET_FAIL
}Ret;

#ifdef __cplusplus
#define DECLS_BEGIN extern "C" {
#define DECLS_END   }
#else
#define DECLS_BEGIN
#define DECLS_END
#endif/*__cplusplus*/

#define return_if_fail(p) if(!(p)) \
	{printf("%s:%d Warning: "#p" failed.\n", \
		__func__, __LINE__); return;}
#define return_val_if_fail(p, ret) if(!(p)) \
	{printf("%s:%d Warning: "#p" failed.\n",\
	__func__, __LINE__); return (ret);}

#define SAFE_FREE(p) if(p != NULL) {free(p); p = NULL;}

#endif/*TYPEDEF_H*/
