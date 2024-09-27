typedef char* (*va_t)(const char *format, ...);
extern va_t va;

typedef char* (*SL_ConvertToString_t)(unsigned int index);
static const SL_ConvertToString_t SL_ConvertToString = (SL_ConvertToString_t)0x080a3460;

//// Cmd
typedef char* (*Cmd_Argv_t)(int arg);
static const Cmd_Argv_t Cmd_Argv = (Cmd_Argv_t)0x080600f4;
////

//// Com
typedef void (*Com_DPrintf_t)(const char *format, ...);
static const Com_DPrintf_t Com_DPrintf = (Com_DPrintf_t)0x0806fc5f;

typedef void (*Com_PrintMessage_t)(int channel, const char *message);
static const Com_PrintMessage_t Com_PrintMessage = (Com_PrintMessage_t)0x0806f827;
////

//// Cvar
typedef cvar_t* (*Cvar_Get_t)(const char *var_name, const char *var_value, unsigned short flags);
static const Cvar_Get_t Cvar_Get = (Cvar_Get_t)0x08072a7c;

typedef cvar_t* (*Cvar_FindVar_t)(const char *var_name);
static const Cvar_FindVar_t Cvar_FindVar = (Cvar_FindVar_t)0x08072916;
////

//// Scr
typedef xfunction_t (*Scr_GetFunction_t)(const char** v_functionName, qboolean *v_developer);
extern Scr_GetFunction_t Scr_GetFunction;

typedef xmethod_t (*Scr_GetMethod_t)(const char** v_methodName, qboolean *v_developer);
extern Scr_GetMethod_t Scr_GetMethod;

typedef void (*Scr_Error_t)(const char *string);
extern Scr_Error_t Scr_Error;

typedef int (*Scr_GetNumParam_t)(void);
extern Scr_GetNumParam_t Scr_GetNumParam;

typedef int (*Scr_GetInt_t)(unsigned int param);
extern Scr_GetInt_t Scr_GetInt;

typedef const char* (*Scr_GetString_t)(unsigned int param);
extern Scr_GetString_t Scr_GetString;

typedef void (*Scr_AddBool_t)(qboolean value);
extern Scr_AddBool_t Scr_AddBool;

typedef void (*Scr_AddInt_t)(int value);
extern Scr_AddInt_t Scr_AddInt;

typedef void (*Scr_AddFloat_t)(float value);
extern Scr_AddFloat_t Scr_AddFloat;

typedef void (*Scr_AddString_t)(const char *string);
extern Scr_AddString_t Scr_AddString;

typedef void (*Scr_AddUndefined_t)(void);
extern Scr_AddUndefined_t Scr_AddUndefined;

typedef void (*Scr_AddVector_t)(vec3_t vec);
extern Scr_AddVector_t Scr_AddVector;
////

//// SV
typedef playerState_t* (*SV_GameClientNum_t)(int num);
static const SV_GameClientNum_t SV_GameClientNum = (SV_GameClientNum_t)0x0808d331;
////