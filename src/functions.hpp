typedef int (*VM_Call_t)(vm_t *vm, int callnum, ...);
static const VM_Call_t VM_Call = (VM_Call_t)0x0809afbc;

typedef char* (*va_t)(const char *format, ...);
extern va_t va;

typedef char* (*SL_ConvertToString_t)(unsigned int index);
static const SL_ConvertToString_t SL_ConvertToString = (SL_ConvertToString_t)0x080a3460;

typedef void (*ClientCommand_t)(int clientNum);
extern ClientCommand_t ClientCommand;

typedef void (*trap_Argv_t)(int arg, char *buffer, int bufferLength);

typedef void (*I_strncpyz_t)(char *dest, const char *src, int destsize);
static const I_strncpyz_t I_strncpyz = (I_strncpyz_t)0x08085e32;

typedef qboolean (*Sys_IsLANAddress_t)(netadr_t adr);
static const Sys_IsLANAddress_t Sys_IsLANAddress = (Sys_IsLANAddress_t)0x080d4ebc;

typedef const char * (*PbAuthClient_t)(const char *clientAddress, int cl_punkbuster, const char *PBguid);
static const PbAuthClient_t PbAuthClient = (PbAuthClient_t)0x080c191c;

typedef void (*Netchan_Setup_t)(netsrc_t src, netchan_t *chan, netadr_t adr, unsigned int qport);
static const Netchan_Setup_t Netchan_Setup = (Netchan_Setup_t)0x0808346f;

typedef float (*PitchForYawOnNormal_t)(const float fYaw, const vec3_t normal);

typedef float (*AngleDelta_t)(float angle1, float angle2);
////

//// BG
typedef void (*BG_AddPredictableEventToPlayerstate_t)(int newEvent, int eventParm, playerState_t *ps);
typedef int (*BG_CheckProne_t)(
    int passEntityNum, const float *const vPos, float fSize, float fHeight, float fYaw, float * pfTorsoHeight, float * pfTorsoPitch, float * pfWaistPitch, int bAlreadyProne, int bOnGround, float *const vGroundNormal,
    void (*)(trace_t *, const vec3_t, const vec3_t, const vec3_t, const vec3_t, int, int),
    void (*)(trace_t *, const vec3_t, const vec3_t, const vec3_t, const vec3_t, int, int),
    int proneCheckType, float prone_feet_dist);
typedef int (*BG_PlayAnim_t)(playerState_s *ps, int animNum, int bodyPart, int forceDuration, qboolean setTimer, qboolean isContinue, qboolean force);
////

//// Cmd
typedef char* (*Cmd_Argv_t)(int arg);
static const Cmd_Argv_t Cmd_Argv = (Cmd_Argv_t)0x080600f4;

typedef int (*Cmd_Argc_t)();
static const Cmd_Argc_t Cmd_Argc = (Cmd_Argc_t)0x080600ea;
////

//// Com
typedef void (*Com_DPrintf_t)(const char *format, ...);
static const Com_DPrintf_t Com_DPrintf = (Com_DPrintf_t)0x0806fc5f;

typedef void (*Com_PrintMessage_t)(int channel, const char *message);
static const Com_PrintMessage_t Com_PrintMessage = (Com_PrintMessage_t)0x0806f827;

typedef void (*Com_Printf_t)(const char *format, ...);
static const Com_Printf_t Com_Printf = (Com_Printf_t)0x0806fc10;
////

//// Cvar
typedef cvar_t* (*Cvar_Get_t)(const char *var_name, const char *var_value, unsigned short flags);
static const Cvar_Get_t Cvar_Get = (Cvar_Get_t)0x08072a7c;

typedef cvar_t* (*Cvar_FindVar_t)(const char *var_name);
static const Cvar_FindVar_t Cvar_FindVar = (Cvar_FindVar_t)0x08072916;
////

//// Info
typedef void (*Info_SetValueForKey_t)(char *s, const char *key, const char *value);
static const Info_SetValueForKey_t Info_SetValueForKey = (Info_SetValueForKey_t)0x08086855;

typedef char * (*Info_ValueForKey_t)(const char *s, const char *key);
static const Info_ValueForKey_t Info_ValueForKey = (Info_ValueForKey_t)0x08086397;
////

//// MSG
typedef void (*MSG_Init_t)(msg_t *buf, byte *data, int length);
static const MSG_Init_t MSG_Init = (MSG_Init_t)0x0807f928;

typedef void (*MSG_WriteLong_t)(msg_t *msg, int c);
static const MSG_WriteLong_t MSG_WriteLong = (MSG_WriteLong_t)0x0807fdb5;

typedef void (*MSG_WriteByte_t)(msg_t *msg, int c);
static const MSG_WriteByte_t MSG_WriteByte = (MSG_WriteByte_t)0x0807fcd8;

typedef void (*MSG_WriteBit0_t)(msg_t* msg);
static const MSG_WriteBit0_t MSG_WriteBit0 = (MSG_WriteBit0_t)0x0807fa4a;

typedef void (*MSG_WriteBit1_t)(msg_t* msg);
static const MSG_WriteBit1_t MSG_WriteBit1 = (MSG_WriteBit1_t)0x0807faa7;

typedef void (*MSG_WriteBits_t)(msg_t *msg, int value, unsigned int bits);
static const MSG_WriteBits_t MSG_WriteBits = (MSG_WriteBits_t)0x0807f984;

typedef void (*MSG_WriteShort_t)(msg_t *msg, int c);
static const MSG_WriteShort_t MSG_WriteShort = (MSG_WriteShort_t)0x0807fd65;

typedef void (*MSG_WriteDeltaObjective_t)(msg_t *msg,objective_t *from,objective_t *to, int lcfield, int numFields, netField_t *objFields);
static const MSG_WriteDeltaObjective_t MSG_WriteDeltaObjective = (MSG_WriteDeltaObjective_t)0x08081210;

typedef void (*MSG_WriteDeltaHudElems_t)(msg_t *msg, hudelem_t *from, hudelem_t *to, int count);
static const MSG_WriteDeltaHudElems_t MSG_WriteDeltaHudElems = (MSG_WriteDeltaHudElems_t)0x08081aff;

typedef int (*MSG_ReadByte_t)(msg_t *msg);
static const MSG_ReadByte_t MSG_ReadByte = (MSG_ReadByte_t)0x08080053;

typedef int (*MSG_ReadBit_t)(msg_t *msg);
static const MSG_ReadBit_t MSG_ReadBit = (MSG_ReadBit_t)0x0807fbc2;

typedef int (*MSG_ReadLong_t)(msg_t *msg);
static const MSG_ReadLong_t MSG_ReadLong = (MSG_ReadLong_t)0x080800e8;

typedef int (*MSG_ReadBits_t)(msg_t *msg, int bits);
static const MSG_ReadBits_t MSG_ReadBits = (MSG_ReadBits_t)0x0807fb40;

typedef int (*MSG_ReadShort_t)(msg_t *msg);
static const MSG_ReadShort_t MSG_ReadShort = (MSG_ReadShort_t)0x08080093;

typedef void (*MSG_ReadDeltaObjective_t)(msg_t *msg, objective_t *from, objective_t *to, int numFields, netField_t *objFields);
static const MSG_ReadDeltaObjective_t MSG_ReadDeltaObjective = (MSG_ReadDeltaObjective_t)0x0808179b;

typedef void (*MSG_ReadDeltaHudElems_t)(msg_t *msg, hudelem_t *from, hudelem_t *to, int count);
static const MSG_ReadDeltaHudElems_t MSG_ReadDeltaHudElems = (MSG_ReadDeltaHudElems_t)0x08081c74;

typedef void (*MSG_WriteBigString_t)(msg_t *msg, const char *s);
static const MSG_WriteBigString_t MSG_WriteBigString = (MSG_WriteBigString_t)0x0807fedb;
////

//// NET
typedef const char * (*NET_AdrToString_t)(netadr_t a);
static const NET_AdrToString_t NET_AdrToString = (NET_AdrToString_t)0x08083e10;

typedef int (*NET_CompareBaseAdr_t)(netadr_t a,netadr_t b);
static const NET_CompareBaseAdr_t NET_CompareBaseAdr = (NET_CompareBaseAdr_t)0x08083de3;

typedef qboolean (*NET_IsLocalAddress_t)(netadr_t from);
static const NET_IsLocalAddress_t NET_IsLocalAddress = (NET_IsLocalAddress_t)0x0808403c;

typedef void (*NET_OutOfBandPrint_t)(netsrc_t net_socket, netadr_t adr, const char *format, ...);
static const NET_OutOfBandPrint_t NET_OutOfBandPrint = (NET_OutOfBandPrint_t)0x0808428e;

typedef qboolean (*NET_CompareAdr_t)(netadr_t a, netadr_t b);
static const NET_CompareAdr_t NET_CompareAdr = (NET_CompareAdr_t)0x0808400f;

typedef vec_t (*VectorNormalize_t)(vec3_t inout);
////

//// PM
typedef bool (*PM_CheckJump_t)();
typedef void (*PM_AirMove_t)();
typedef void (*PM_Friction_t)();
typedef float (*PM_CmdScale_Walk_t)(usercmd_t *cmd);
typedef void (*PM_ClipVelocity_t)(const float *in, const float *normal, float *out, float overbounce);
typedef int (*PM_GetEffectiveStance_t)(playerState_t *ps);
typedef void (*PM_Accelerate_t)(float *wishdir, float wishspeed, float accel);
typedef void (*PM_StepSlideMove_t)(int gravity);
typedef void (*PM_SetMovementDir_t)();
typedef void (*PM_ViewHeightAdjust_t)();
typedef void (*PM_ClearAimDownSightFlag_t)();
typedef int (*PM_GroundSurfaceType_t)();
typedef void (*PM_trace_t)(trace_t *results, const vec3_t start, const vec3_t mins, const vec3_t maxs, const vec3_t end, int passEntityNum, int contentMask);
typedef void (*PM_AddEvent_t)(int newEvent);
typedef int (*PM_FootstepType_t)(int pm_flags);
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

typedef void (*Scr_MakeArray_t)(void);
extern Scr_MakeArray_t Scr_MakeArray;

typedef void (*Scr_AddArray_t)(void);
extern Scr_AddArray_t Scr_AddArray;

typedef unsigned int (*Scr_LoadScript_t)(const char *filename);

typedef int (*Scr_GetFunctionHandle_t)(const char* scriptName, const char* labelName);

typedef int (*Scr_IsSystemActive_t)();

typedef short (*Scr_ExecEntThread_t)(gentity_t* ent, int callbackHook, unsigned int numArgs);
extern Scr_ExecEntThread_t Scr_ExecEntThread;

typedef short (*Scr_FreeThread_t)(short thread_id);
extern Scr_FreeThread_t Scr_FreeThread;

typedef unsigned short (*Scr_AllocArray_t)(void);
static const Scr_AllocArray_t Scr_AllocArray = (Scr_AllocArray_t)0x080a5ab4;
////

//// SV
typedef playerState_t* (*SV_GameClientNum_t)(int num);
static const SV_GameClientNum_t SV_GameClientNum = (SV_GameClientNum_t)0x0808d331;

typedef gentity_t* (*SV_ClientThink_t)(client_t *cl, usercmd_t *cmd);
static const SV_ClientThink_t SV_ClientThink = (SV_ClientThink_t)0x0808ca58;

typedef void (*SV_FreeClient_t)(client_t *client);
static const SV_FreeClient_t SV_FreeClient = (SV_FreeClient_t)0x08089daf;

typedef gentity_t * (*SV_GentityNum_t)(int num);
static const SV_GentityNum_t SV_GentityNum = (SV_GentityNum_t)0x0808d314;

typedef void (*SV_UserinfoChanged_t)(client_t *client);
static const SV_UserinfoChanged_t SV_UserinfoChanged = (SV_UserinfoChanged_t)0x0808c4fd;

typedef void (*SV_Heartbeat_f_t)(void);
static const SV_Heartbeat_f_t SV_Heartbeat_f = (SV_Heartbeat_f_t)0x08088535;

typedef void (*SV_FreeClientScriptId_t)(client_t *client);
static const SV_FreeClientScriptId_t SV_FreeClientScriptId = (SV_FreeClientScriptId_t)0x08093e86;

typedef void (*SV_BuildClientSnapshot_t)(client_t *client);
static const SV_BuildClientSnapshot_t SV_BuildClientSnapshot = (SV_BuildClientSnapshot_t)0x080972d8;

typedef void (*SV_UpdateServerCommandsToClient_t)(client_t *client, msg_t *msg);
static const SV_UpdateServerCommandsToClient_t SV_UpdateServerCommandsToClient = (SV_UpdateServerCommandsToClient_t)0x080959d0;

typedef void (*SV_WriteSnapshotToClient_t)(client_t *client, msg_t *msg);
static const SV_WriteSnapshotToClient_t SV_WriteSnapshotToClient = (SV_WriteSnapshotToClient_t)0x0809570c;

typedef void (*SV_WriteDownloadToClient_t)(client_t *client, msg_t *msg);
static const SV_WriteDownloadToClient_t SV_WriteDownloadToClient = (SV_WriteDownloadToClient_t)0x0808b8a9;

typedef void (*SV_PrintServerCommandsForClient_t)(client_t *client);
static const SV_PrintServerCommandsForClient_t SV_PrintServerCommandsForClient = (SV_PrintServerCommandsForClient_t)0x08095b2d;

typedef void (*SV_UpdateServerCommandsToClient_PreventOverflow_t)(client_t *client, msg_t *msg, int iMsgSize);
static const SV_UpdateServerCommandsToClient_PreventOverflow_t SV_UpdateServerCommandsToClient_PreventOverflow = (SV_UpdateServerCommandsToClient_PreventOverflow_t)0x08095a5a;

typedef void (*SV_DropClient_t)(client_t *drop, const char *reason);
static const SV_DropClient_t SV_DropClient = (SV_DropClient_t)0x0808ac11;

typedef void (*SV_SendMessageToClient_t)(msg_t *msg, client_t *cl);
static const SV_SendMessageToClient_t SV_SendMessageToClient = (SV_SendMessageToClient_t)0x08097a2f;

typedef void (*SV_Netchan_TransmitNextFragment_t)(netchan_t *chan);
static const SV_Netchan_TransmitNextFragment_t SV_Netchan_TransmitNextFragment = (SV_Netchan_TransmitNextFragment_t)0x080948d0;

typedef void (*MSG_WriteDeltaEntity_t)(msg_t *msg, struct entityState_s *from, struct entityState_s *to, qboolean force);
static const MSG_WriteDeltaEntity_t MSG_WriteDeltaEntity = (MSG_WriteDeltaEntity_t)0x0808149a;
////