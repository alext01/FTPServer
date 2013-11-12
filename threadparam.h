#ifndef __THREADPARAM_H__
#define __THREADPARAM_H__

#include <stdbool.h>

typedef struct{
	int sock;
	bool *complete;
} threadparam;

#endif //__THREADPARAM_H__
