#ifndef _SHARED_HPP_
#define _SHARED_HPP_

#include <unistd.h>     // access
#include <stdint.h>     // uint16_t
#include <cstdio>       // printf, snprintf
#include <cstring>      // strcasecmp, strlen, strcmp, strerror, strncpy...
#include <stdlib.h>     // atoi
#include <stdarg.h>     // va
#include <string>

#include "declarations.hpp"
#include "functions.hpp"
#include "utils.hpp"
#include "proxy.h"
#include "gsc_player.hpp"
#include "gsc_bots.hpp"
#include "gsc_utils.hpp"

#define STACK_UNDEFINED 0
#define STACK_STRING 1
#define STACK_LOCALIZED_STRING 2
#define STACK_VECTOR 3
#define STACK_FLOAT 4
#define STACK_INT 5
#define STACK_OBJECT 7
#define STACK_FUNCTION 9

#define stackPushUndefined Scr_AddUndefined
#define stackPushBool Scr_AddBool
#define stackPushInt Scr_AddInt
#define stackPushFloat Scr_AddFloat
#define stackPushString Scr_AddString
#define stackPushVector Scr_AddVector
#define stackPushArray Scr_MakeArray
#define stackPushArrayLast Scr_AddArray

extern customPlayerState_t customPlayerState[MAX_CLIENTS];

xfunction_t Scr_GetCustomFunction(const char **fname, qboolean *fdev);
xmethod_t Scr_GetCustomMethod(const char **fname, qboolean *fdev);

int Q_vsnprintf(char *s0, size_t size, const char *fmt, va_list args);

int stackGetParams(const char *params, ...);
void stackError(const char *format, ...);

int stackGetParamInt(int param, int *value);
int stackGetParamString(int param, char **value);
int stackGetParamFloat(int param, float *value);


// For tests
void gsc_testfunction();
void gsc_testmethod(scr_entref_t ref);
#endif