#include "libcod.hpp"

#include "cracking.hpp"
#include "shared.hpp"

//// Cvars
cvar_t *com_timescale;
cvar_t *fs_game;
cvar_t *g_debugProneCheck;
cvar_t *sv_maxclients;
cvar_t *sv_maxPing;
cvar_t *sv_minPing;
cvar_t *sv_privateClients;
cvar_t *sv_privatePassword;
cvar_t *sv_reconnectlimit;
// VM
vmCvar_t *bg_viewheight_crouched;
vmCvar_t *bg_viewheight_prone;
vmCvar_t *bg_viewheight_standing;
vmCvar_t *g_inactivity;
// Custom
cvar_t *fs_callbacks;
cvar_t *fs_callbacks_additional;
cvar_t *sv_botHook;
cvar_t *sv_cracked;
////

//// Game lib
void* libHandle;
// Objects
gentity_t *g_entities;
level_locals_t *level;
vec3_t *playerMins;
vec3_t *playerMaxs;
pmove_t **pm;
pml_t *pml;
scr_const_t *scr_const;
// Functions
ClientCommand_t ClientCommand;
Scr_IsSystemActive_t Scr_IsSystemActive;
Scr_GetNumParam_t Scr_GetNumParam;
Scr_GetInt_t Scr_GetInt;
Scr_GetString_t Scr_GetString;
Scr_AddBool_t Scr_AddBool;
Scr_AddInt_t Scr_AddInt;
Scr_AddFloat_t Scr_AddFloat;
Scr_AddString_t Scr_AddString;
Scr_AddUndefined_t Scr_AddUndefined;
Scr_AddVector_t Scr_AddVector;
Scr_MakeArray_t Scr_MakeArray;
Scr_AddArray_t Scr_AddArray;
Scr_GetFunction_t Scr_GetFunction;
Scr_GetMethod_t Scr_GetMethod;
Scr_Error_t Scr_Error;
Scr_LoadScript_t Scr_LoadScript;
Scr_GetFunctionHandle_t Scr_GetFunctionHandle;
Scr_ExecEntThread_t Scr_ExecEntThread;
Scr_FreeThread_t Scr_FreeThread;
trap_Argv_t trap_Argv;
va_t va;
VectorNormalize_t VectorNormalize;
BG_AddPredictableEventToPlayerstate_t BG_AddPredictableEventToPlayerstate;
BG_CheckProne_t BG_CheckProne;
BG_PlayAnim_t BG_PlayAnim;
PitchForYawOnNormal_t PitchForYawOnNormal;
AngleDelta_t AngleDelta;
PM_CheckJump_t PM_CheckJump;
PM_AirMove_t PM_AirMove;
PM_Friction_t PM_Friction;
PM_CmdScale_Walk_t PM_CmdScale_Walk;
PM_ClipVelocity_t PM_ClipVelocity;
PM_GetEffectiveStance_t PM_GetEffectiveStance;
PM_Accelerate_t PM_Accelerate;
PM_StepSlideMove_t PM_StepSlideMove;
PM_SetMovementDir_t PM_SetMovementDir;
PM_ViewHeightAdjust_t PM_ViewHeightAdjust;
PM_ClearAimDownSightFlag_t PM_ClearAimDownSightFlag;
PM_GroundSurfaceType_t PM_GroundSurfaceType;
PM_trace_t PM_trace;
PM_AddEvent_t PM_AddEvent;
PM_FootstepType_t PM_FootstepType;
G_ClientStopUsingTurret_t G_ClientStopUsingTurret;
G_EntUnlink_t G_EntUnlink;
trap_UnlinkEntity_t trap_UnlinkEntity;
Scr_SetString_t Scr_SetString;
G_SetClientContents_t G_SetClientContents;
player_die_t player_die;
trap_GetUsercmd_t trap_GetUsercmd;
G_SetOrigin_t G_SetOrigin;
SetClientViewAngle_t SetClientViewAngle;
ClientEndFrame_t ClientEndFrame;
ClientThink_real_t ClientThink_real;
BG_PlayerStateToEntityState_t BG_PlayerStateToEntityState;
G_ClientCanSpectateTeam_t G_ClientCanSpectateTeam;
Cmd_FollowCycle_f_t Cmd_FollowCycle_f;
trap_GetArchivedClientInfo_t trap_GetArchivedClientInfo;
StopFollowing_t StopFollowing;
HudElem_UpdateClient_t HudElem_UpdateClient;
AddLeanToPosition_t AddLeanToPosition;
UnGetLeanFraction_t UnGetLeanFraction;
PM_GetLandFactor_t PM_GetLandFactor;
AngleNormalize180Accurate_t AngleNormalize180Accurate;
AngleNormalize180_t AngleNormalize180;
BG_CheckProneValid_t BG_CheckProneValid;
////

//// Callbacks
int codecallback_startgametype = 0;
int codecallback_playerconnect = 0;
int codecallback_playerdisconnect = 0;
int codecallback_playerdamage = 0;
int codecallback_playerkilled = 0;
int codecallback_playercommand = 0;
callback_t callbacks[] =
{
    // Stock
    {&codecallback_startgametype, "CodeCallback_StartGameType", false},
    {&codecallback_playerconnect, "CodeCallback_PlayerConnect", false},
    {&codecallback_playerdisconnect, "CodeCallback_PlayerDisconnect", false},
    {&codecallback_playerdamage, "CodeCallback_PlayerDamage", false},
    {&codecallback_playerkilled, "CodeCallback_PlayerKilled", false},
    // Custom
    {&codecallback_playercommand, "CodeCallback_PlayerCommand", true}
};
////

customPlayerState_t customPlayerState[MAX_CLIENTS];
std::map<std::string, std::map<int, customWeaponinfo>> customWeaponinfo;

cHook *hook_Com_Init;
cHook *hook_GScr_LoadGameTypeScript;
cHook *hook_SV_BotUserMove;
cHook *hook_Sys_LoadDll;
cHook *hook_SV_SpawnServer;

void custom_SV_SpawnServer(char *server)
{
    hook_SV_SpawnServer->unhook();
    void (*SV_SpawnServer)(char *server);
    *(int*)&SV_SpawnServer = hook_SV_SpawnServer->from;
    SV_SpawnServer(server);
    hook_SV_SpawnServer->hook();

    if (customWeaponinfo.empty())
    {
        customWeaponinfo["kar98k_sniper_mp"][6] = { 199, 449, 0.1, 0.6, 0.2, 0, 1.2, 1.4 };
        customWeaponinfo["kar98k_sniper_mp"][1] = { 199, 299, 0.42, 0.2, 0.085, 1, 0, 0 };

        customWeaponinfo["mosin_nagant_sniper_mp"][6] = { 1339, 449, 0.1, 0.6, 0.2, 0, 1.2, 1.4 };
        customWeaponinfo["mosin_nagant_sniper_mp"][1] = { 339, 299, 0.42, 0.2, 0.085, 1, 0, 0 };

        customWeaponinfo["springfield_mp"][6] = { 199, 449, 0.1, 0.6, 0.2, 0, 1.2, 1.4 };
        customWeaponinfo["springfield_mp"][1] = { 199, 299, 0.05, 0.2, 0.085, 1, 0, 0 };
    }
}

void custom_Com_Init(char *commandLine)
{
    hook_Com_Init->unhook();
    void (*Com_Init)(char *commandLine);
    *(int*)&Com_Init = hook_Com_Init->from;
    Com_Init(commandLine);
    hook_Com_Init->hook();
    
    // Get references to stock cvars
    com_timescale = Cvar_FindVar("timescale");
    fs_game = Cvar_FindVar("fs_game");
    g_debugProneCheck = Cvar_FindVar("g_debugProneCheck");
    sv_maxclients = Cvar_FindVar("sv_maxclients");
    sv_maxPing = Cvar_FindVar("sv_maxPing");
    sv_minPing = Cvar_FindVar("sv_minPing");
    sv_privateClients = Cvar_FindVar("sv_privateClients");
    sv_privatePassword = Cvar_FindVar("sv_privatePassword");
    sv_reconnectlimit = Cvar_FindVar("sv_reconnectlimit");

    // Register custom cvars
    Cvar_Get("libcod", "1", CVAR_SERVERINFO);

    fs_callbacks = Cvar_Get("fs_callbacks", "maps/mp/gametypes/_callbacksetup", CVAR_ARCHIVE);
    fs_callbacks_additional = Cvar_Get("fs_callbacks_additional", "", CVAR_ARCHIVE);
    sv_botHook = Cvar_Get("sv_botHook", "0", CVAR_ARCHIVE);
    sv_cracked = Cvar_Get("sv_cracked", "0", CVAR_ARCHIVE);
}

void custom_GScr_LoadGameTypeScript()
{
    hook_GScr_LoadGameTypeScript->unhook();
    void (*GScr_LoadGameTypeScript)();
    *(int*)&GScr_LoadGameTypeScript = hook_GScr_LoadGameTypeScript->from;
    GScr_LoadGameTypeScript();
    hook_GScr_LoadGameTypeScript->hook();

    unsigned int i;
    
    if(*fs_callbacks_additional->string && !Scr_LoadScript(fs_callbacks_additional->string))
        Com_DPrintf("custom_GScr_LoadGameTypeScript: Scr_LoadScript for fs_callbacks_additional failed.\n");

    for (i = 0; i < sizeof(callbacks) / sizeof(callbacks[0]); i++)
    {
        if(callbacks[i].custom)
            *callbacks[i].pos = Scr_GetFunctionHandle(fs_callbacks_additional->string, callbacks[i].name);
        else
            *callbacks[i].pos = Scr_GetFunctionHandle(fs_callbacks->string, callbacks[i].name);

        /*if (*callbacks[i].pos && g_debugCallbacks->integer)
            Com_Printf("%s found @ %p\n", callbacks[i].name, scrVarPub.programBuffer + *callbacks[i].pos);*/ //TODO: verify scrVarPub_t
    }
}

const char* hook_AuthorizeState(int arg)
{
    const char* s = Cmd_Argv(arg);
    if(sv_cracked->integer && !strcmp(s, "deny"))
        return "accept";
    return s;
}

const char *getPatchFromProtocol(int protocol)
{
    switch (protocol)
    {
        case 1: return "1.1";
        case 6: return "1.5";
        default: return "unknown";
    }
}

void custom_SV_DirectConnect(netadr_t from)
{
    char userinfo[MAX_INFO_STRING];
    int i;
    client_t *cl, *newcl;
    gentity_t *ent;
    int clientNum;
    int protocol;
    int challenge;
    int qport;
    const char *PBAuthAddress;
    const char *PBAuthResult;
    const char *password;
    int startIndex;
    const char *denied;
    int count;
    int guid;
    
    Com_DPrintf("SV_DirectConnect()\n");
    
    I_strncpyz(userinfo, Cmd_Argv(1), sizeof(userinfo));
    protocol = atoi(Info_ValueForKey(userinfo, "protocol"));
    
    if (protocol != 6 && protocol != 1)
    {
        NET_OutOfBandPrint(NS_SERVER, from, va("error\nEXE_SERVER_IS_DIFFERENT_VER\x15%s\n", "1.5"));
        Com_DPrintf("    rejected connect from protocol version %i (should be %i or %i)\n", protocol, 6, 1);
        return;
    }
    
    challenge = atoi(Info_ValueForKey(userinfo, "challenge"));
    qport = atoi(Info_ValueForKey(userinfo, "qport"));
    
    for (i = 0, cl = svs.clients; i < sv_maxclients->integer; i++, cl++)
    {
        if (NET_CompareBaseAdr(from, cl->netchan.remoteAddress)
            && (cl->netchan.qport == qport || from.port == cl->netchan.remoteAddress.port))
        {
            if ((svs.time2 - cl->lastConnectTime) < (sv_reconnectlimit->integer * 1000))
            {
                Com_DPrintf("%s:reconnect rejected : too soon\n", NET_AdrToString(from));
                return;
            }
            break;
        }
    }

    guid = 0;
    if (!NET_IsLocalAddress(from))
    {
        int ping;
        for (i = 0 ; i < MAX_CHALLENGES ; i++)
        {
            if (NET_CompareAdr(from, svs.challenges[i].adr))
            {
                if (challenge == svs.challenges[i].challenge)
                {
                    guid = svs.challenges[i].guid;
                    break;
                }
            }
        }

        if (i == MAX_CHALLENGES)
        {
            NET_OutOfBandPrint(NS_SERVER, from, "error\nEXE_BAD_CHALLENGE");
            return;
        }

        if (svs.challenges[i].firstPing == 0)
        {
            ping = svs.time2 - svs.challenges[i].pingTime;
            svs.challenges[i].firstPing = ping;
        }
        else
        {
            ping = svs.challenges[i].firstPing;
        }

        Com_Printf("Client %i connecting with %i challenge ping from %s\n", i, ping, NET_AdrToString(from));
        svs.challenges[i].connected = qtrue;

        if (!Sys_IsLANAddress(from))
        {
            if (sv_minPing->integer && ping < sv_minPing->integer)
            {
                NET_OutOfBandPrint(NS_SERVER, from, "error\nEXE_ERR_HIGH_PING_ONLY");
                Com_DPrintf("Client %i rejected on a too low ping\n", i);
                return;
            }
            if (sv_maxPing->integer && ping > sv_maxPing->integer)
            {
                NET_OutOfBandPrint(NS_SERVER, from, "error\nEXE_ERR_LOW_PING_ONLY");
                Com_DPrintf("Client %i rejected on a too high ping: %i\n", i, ping);
                return;
            }
        }
    }

    if(!NET_IsLocalAddress(from))
        PBAuthAddress = NET_AdrToString(from);
    else
        PBAuthAddress = "localhost";
    PBAuthResult = PBAuthClient(PBAuthAddress, atoi(Info_ValueForKey(userinfo, "cl_punkbuster")), Info_ValueForKey(userinfo, "cl_guid"));
    if (!PBAuthResult)
    {
        for (i = 0, cl = svs.clients; i < sv_maxclients->integer; i++, cl++)
        {
            if(cl->state == CS_FREE)
                continue;

            if (NET_CompareBaseAdr(from, cl->netchan.remoteAddress)
                && (cl->netchan.qport == qport || from.port == cl->netchan.remoteAddress.port))
            {
                Com_Printf("%s:reconnect\n", NET_AdrToString(from));

                if(cl->state > CS_ZOMBIE)
                    SV_FreeClient(cl);

                newcl = cl;
                goto LAB_0808a83b;
            }
        }

        password = Info_ValueForKey(userinfo, "password");
        if (!strcmp(password, sv_privatePassword->string))
            startIndex = 0;
        else
            startIndex = sv_privateClients->integer;

        newcl = NULL;

        for (i = startIndex; i < sv_maxclients->integer; i++)
        {
            cl = &svs.clients[i];
            if (cl->state == CS_FREE)
            {
                newcl = cl;
                break;
            }
        }
        if (!newcl)
        {
            NET_OutOfBandPrint(NS_SERVER, from, "error\nEXE_SERVERISFULL");
            Com_DPrintf("Rejected a connection.\n");
            return;
        }
        
        cl->reliableAcknowledge = 0;
        cl->reliableSequence = 0;
LAB_0808a83b:
        memset(newcl, 0, sizeof(client_t));
        clientNum = newcl - svs.clients;
        ent = SV_GentityNum(clientNum);
        newcl->gentity = ent;
        newcl->clscriptid = Scr_AllocArray();
        newcl->challenge = challenge;

        // Save client protocol
        customPlayerState[clientNum].protocol = protocol;
        Com_Printf("Connecting player #%i from patch %s\n", clientNum, getPatchFromProtocol(protocol));

        newcl->guid = guid;

        Netchan_Setup(NS_SERVER, &newcl->netchan, from, qport);
        I_strncpyz(newcl->userinfo, userinfo, sizeof(newcl->userinfo));

        denied = (char *)VM_Call(gvm, 4, clientNum, newcl->clscriptid);
        if (!denied)
        {
            SV_UserinfoChanged(newcl);
            svs.challenges[i].firstPing = 0;
            NET_OutOfBandPrint(NS_SERVER, from, "connectResponse");
            Com_Printf("Going from CS_FREE to CS_CONNECTED for %s (num %i guid %i)\n", newcl->name, clientNum, newcl->guid);

            newcl->state = CS_CONNECTED;
            newcl->nextSnapshotTime = svs.time2;
            newcl->lastPacketTime = svs.time2;
            newcl->lastConnectTime = svs.time2;
            newcl->gamestateMessageNum = -1;
            
            count = 0;
            for(i = 0, cl = svs.clients; i < sv_maxclients->integer; i++, cl++)
                if(svs.clients[i].state >= CS_CONNECTED)
                    count++;

            if(count == 1 || count == sv_maxclients->integer)
                SV_Heartbeat_f();
        }
        else
        {
            NET_OutOfBandPrint(NS_SERVER, from, va("error\n%s", denied));
            Com_DPrintf("Game rejected a connection: %s.\n", denied);
            SV_FreeClientScriptId(newcl);
            return;
        }
    }
    else
    {
        if(!strncasecmp(PBAuthResult, "error\n", 6))
            NET_OutOfBandPrint(NS_SERVER, from, PBAuthResult);
    }
}

void custom_SV_SendClientGameState(client_t *client)
{
    int start;
    entityState_t *base, nullstate;
    msg_t msg;
    byte msgBuffer[MAX_MSGLEN];
    int clientNum = client - svs.clients;
    int protocol;
    
    while(client->state != CS_FREE && client->netchan.unsentFragments)
        SV_Netchan_TransmitNextFragment(&client->netchan);

    Com_DPrintf("SV_SendClientGameState() for %s\n", client->name);
    Com_DPrintf("Going from CS_CONNECTED to CS_PRIMED for %s\n", client->name);

    client->state = CS_PRIMED;
    client->pureAuthentic = 0;
    client->gamestateMessageNum = client->netchan.outgoingSequence;

    // Backup some customPlayerState data
    protocol = customPlayerState[clientNum].protocol;
    
    // Reset customPlayerState
    memset(&customPlayerState[clientNum], 0, sizeof(customPlayerState_t));

    // Restore saved customPlayerState data
    customPlayerState[clientNum].protocol = protocol;

    MSG_Init(&msg, msgBuffer, sizeof(msgBuffer));
    MSG_WriteLong(&msg, client->lastClientCommand);
    SV_UpdateServerCommandsToClient(client, &msg);
    MSG_WriteByte(&msg, svc_gamestate);
    MSG_WriteLong(&msg, client->reliableSequence);

    for (start = 0; start < MAX_CONFIGSTRINGS; start++)
    {
        if (sv.configstrings[start][0])
        {
            MSG_WriteByte(&msg, svc_configstring);
            MSG_WriteShort(&msg, start);
            MSG_WriteBigString(&msg, sv.configstrings[start]);
        }
    }

    memset(&nullstate, 0, sizeof(nullstate));
    for (start = 0; start < MAX_GENTITIES; start++)
    {
        base = &sv.svEntities[start].baseline.s;
        if(!base->number)
            continue;
        MSG_WriteByte(&msg, svc_baseline);
        MSG_WriteDeltaEntity(&msg, &nullstate, base, qtrue);
    }

    MSG_WriteByte(&msg, svc_EOF);
    MSG_WriteLong(&msg, clientNum);
    MSG_WriteLong(&msg, sv.checksumFeed);
    MSG_WriteByte(&msg, svc_EOF);

    Com_DPrintf("Sending %i bytes in gamestate to client: %i\n", msg.cursize, clientNum);

    SV_SendMessageToClient(&msg, client);

    if(com_timescale->integer == 0 && client->state != CS_FREE && client->netchan.unsentFragments)
        SV_Netchan_TransmitNextFragment(&client->netchan);
}

void custom_MSG_WriteDeltaPlayerstate(msg_t *msg, playerState_t *from, playerState_t *to)
{
    int i, j, lc;
    int *toF, *fromF;
    netField_t *field;
    playerState_t dummy;
    int32_t numBits;
    uint8_t abs3bits;
    int clipbits;
    int ammobits[7];
    int statsbits;
    int32_t bitmask;
    float floatbits;
    int32_t signedbits;
    uint32_t unsignedbits;
    
    int clientProtocol_to = customPlayerState[to->clientNum].protocol;
    int clientProtocol_from = 0;

    /*client_t *client_to;
    client_t *client_from;*/
    
    if (!from)
    {
        from = &dummy;
        memset(&dummy, 0, sizeof(dummy));
        //client_to = &svs.clients[to->clientNum];
    }
    else
    {
        /*client_to = &svs.clients[to->clientNum];
        client_from = &svs.clients[from->clientNum];*/
        clientProtocol_from = customPlayerState[from->clientNum].protocol;
    }

    lc = 0;
    for (i = 0, field = &playerStateFields; i < 0x67; i++, field++)
    {
        fromF = (int *)((byte *)from + field->offset);
        /*if(clientProtocol_to == 1 && !strcmp(field->name, "deltaTime"))
            toF = (int *)((byte *)to + (field->offset + 4));
        else
            */toF = (int *)((byte *)to + field->offset);


        /*if (!strcmp(field->name, "deltaTime")
            && clientProtocol_from
            && clientProtocol_from == 6 && clientProtocol_to == 1)
        {
            toF = (int *)((byte *)to + field->offset + 4);
            printf("##### toF: %i\n", (unsigned int)(uintptr_t)toF);
        }*/


        /*if (!strcmp(field->name, "deltaTime"))
        {
            printf("##### _deltaTime: %i\n", *toF);
        }*/






        if(*fromF != *toF)
            lc = i + 1;
    }

    MSG_WriteByte(msg, lc);

    for (i = 0, field = &playerStateFields; i < lc; i++, field++)
    {
        fromF = (int *)((byte *)from + field->offset);
        /*if(clientProtocol_to == 1 && !strcmp(field->name, "deltaTime"))
            toF = (int *)((byte *)to + (field->offset + 4));
        else
            */toF = (int *)((byte *)to + field->offset);


        /*if (!strcmp(field->name, "deltaTime"))
        {
            printf("##### _ _deltaTime: %i\n", *toF);
        }*/





        
        floatbits = *(float *)toF;
        signedbits = *(int32_t *)toF;
        unsignedbits = *(uint32_t *)toF;
        
        if (*fromF == *toF)
        {
            MSG_WriteBit0(msg);
        }
        else
        {
            MSG_WriteBit1(msg);

            if (!field->bits)
            {
                if ( (float)signedbits == floatbits && (unsigned int)(signedbits + 4096) <= 0x1FFF )
                {
                    MSG_WriteBit0(msg);
                    MSG_WriteBits(msg, (signedbits + 4096), 5);
                    MSG_WriteByte(msg, (signedbits + 4096) >> 5);
                }
                else
                {
                    MSG_WriteBit1(msg);
                    MSG_WriteLong(msg, unsignedbits);
                }
            }
            else
            {
                numBits = abs(field->bits);
                if(!strcmp(field->name, "pm_flags") && clientProtocol_to == 1)
                    numBits -=2;
                bitmask = unsignedbits;


                /*if (!strcmp(field->name, "pm_flags")
                    && clientProtocol_from
                    && clientProtocol_from == 6 && clientProtocol_to == 1)
                {
                    if (bitmask & PMF_FOLLOW)
                    {
                        printf("##### bitmask: %X\n", bitmask);




                    }
                }
                else if (!strcmp(field->name, "pm_flags")
                    && clientProtocol_from
                    && clientProtocol_from == 1 && clientProtocol_to == 6)
                {
                    printf("##### 1to6 bitmask: %X\n", bitmask);
                }*/


                if (!strcmp(field->name, "pm_flags")
                    && clientProtocol_to == 1)
                {
                    //printf("##### before: %X\n", bitmask);
                    if(bitmask & PMF_JUMPING)
                        bitmask &= ~PMF_JUMPING;
                    if(bitmask & PMF_DISABLEWEAPON)
                        bitmask &= ~PMF_DISABLEWEAPON;
                    if (bitmask & PMF_FOLLOW)
                    {
                        bitmask &= ~PMF_FOLLOW;
                        bitmask &= ~PMF_SPECTATOR;
                    }
                    else if (bitmask == PMF_SPECTATOR)
                    {
                        bitmask &= ~PMF_SPECTATOR;
                    }
                    else if (bitmask & 0x20000) // Playing
                    {
                        bitmask &= ~0x20000;
                        bitmask |= 0x40000;
                    }
                    else if (bitmask & 0x10000) // Killcam
                    {
                        bitmask &= ~0x10000;
                        bitmask |= 0x30000;
                    }
                    //printf("##### after:  %X\n", bitmask);
                }
                
                
                abs3bits = numBits & 7;
                if (abs3bits)
                {
                    MSG_WriteBits(msg, bitmask, abs3bits);
                    numBits -= abs3bits;
                    bitmask >>= abs3bits;
                }
                while (numBits)
                {
                    MSG_WriteByte(msg, bitmask);
                    bitmask >>= 8;
                    numBits -= 8;
                }
            }
        }
    }

    statsbits = 0;
    i = 0;
    while (i < 6)
    {
        if (to->stats[i] != from->stats[i])
            statsbits = statsbits | 1 << ((byte)i & 0x1f);
        i++;
    }
    if (!statsbits)
    {
        MSG_WriteBit0(msg);
    }
    else
    {
        MSG_WriteBit1(msg);
        MSG_WriteBits(msg, statsbits, 6);
        if (statsbits & 1U)
            MSG_WriteShort(msg, to->stats[0]);
        if (statsbits & 2U)
            MSG_WriteShort(msg, to->stats[1]);
        if (statsbits & 4U)
            MSG_WriteShort(msg, to->stats[2]);
        if (statsbits & 8U)
            MSG_WriteBits(msg, to->stats[3], 6);
        if (statsbits & 0x10U)
            MSG_WriteShort(msg, to->stats[4]);
        if (statsbits & 0x20U)
            MSG_WriteByte(msg, (char)to->stats[5]);
    }

    j = 0;
    while (j < 4)
    {
        ammobits[j] = 0;
        i = 0;
        while (i < 0x10)
        {
            if (to->ammo[j * 0x10 + i] != from->ammo[j * 0x10 + i])
                ammobits[j] = 1 << ((byte)i & 0x1f) | ammobits[j];
            i++;
        }
        j++;
    }
    if (!ammobits[0] && !ammobits[1] && !ammobits[2] && !ammobits[3])
    {
        MSG_WriteBit0(msg);
    }
    else
    {
        MSG_WriteBit1(msg);
        j = 0;
        while (j < 4)
        {
            if (ammobits[j] == 0)
            {
                MSG_WriteBit0(msg);
            }
            else
            {
                MSG_WriteBit1(msg);
                MSG_WriteShort(msg,ammobits[j]);
                i = 0;
                while (i < 0x10)
                {
                    if((ammobits[j] >> ((byte)i & 0x1F) & 1) != 0)
                        MSG_WriteShort(msg, to->ammo[j * 0x10 + i]);
                    i++;
                }
            }
            j++;
        }
    }

    j = 0;
    while (j < 4)
    {
        clipbits = 0;
        i = 0;
        while (i < 0x10)
        {
            if (to->ammoclip[j * 0x10 + i] != from->ammoclip[j * 0x10 + i])
                clipbits = clipbits | 1 << ((byte)i & 0x1F);
            i++;
        }
        if (!clipbits)
        {
            MSG_WriteBit0(msg);
        }
        else
        {
            MSG_WriteBit1(msg);
            MSG_WriteShort(msg,clipbits);
            i = 0;
            while (i < 0x10)
            {
                if (clipbits >> ((byte)i & 0x1F) & 1)
                    MSG_WriteShort(msg, to->ammoclip[j * 0x10 + i]);
                i++;
            }
        }
        j++;
    }

    if (!memcmp(from->objective, to->objective, sizeof(from->objective)))
    {
        MSG_WriteBit0(msg);
    }
    else
    {
        MSG_WriteBit1(msg);
        i = 0;
        while (i < 0x10)
        {
            MSG_WriteBits(msg, to->objective[i].state, 3);
            MSG_WriteDeltaObjective(msg, &from->objective[i], &to->objective[i], 0, 6, &objectiveFields);
            i++;
        }
    }

    if (!memcmp(&from->hud, &to->hud, sizeof(from->hud)))
    {
        MSG_WriteBit0(msg);
    }
    else
    {
        MSG_WriteBit1(msg);
        MSG_WriteDeltaHudElems(msg, from->hud.archival, to->hud.archival, MAX_HUDELEMS_ARCHIVAL);
        MSG_WriteDeltaHudElems(msg, from->hud.current, to->hud.current, MAX_HUDELEMS_CURRENT);
    }

#if 0
    if ((clientProtocol_from && clientProtocol_from != clientProtocol_to)
        || !clientProtocol_from)
    {
        printf("-------------------------- END\n");
    }
#endif
}

void custom_MSG_ReadDeltaPlayerstate(msg_t *msg, playerState_t *from, playerState_t *to)
{
    int i, j, k, lc;
    netField_t *field;
    int *fromF, *toF;
    playerState_t dummy;
    int readbits;
    int readbyte;
    uint32_t unsignedbits;

    int clientProtocol_to = customPlayerState[to->clientNum].protocol;
    
    if (!from)
    {
        from = &dummy;
        memset(&dummy, 0, sizeof(dummy));
    }
    memcpy(to, from, sizeof(playerState_t));
    
    lc = MSG_ReadByte(msg);
    for (i = 0, field = &playerStateFields ; i < lc ; i++, field++)
    {
        fromF = (int32_t *)((byte *)from + field->offset);
        toF = (int32_t *)((byte *)to + field->offset);

        if (!MSG_ReadBit(msg))
        {
            *toF = *fromF;
            continue;
        }
        

        /*if (!strcmp(field->name, "deltaTime"))
        {
            printf("##### _toF: %i\n", *toF);
        }
        if (!strcmp(field->name, "deltaTime"))
        {
            printf("##### fromF: %i\n", *fromF);
        }*/

        
        if (!field->bits)
        {
            if (MSG_ReadBit(msg))
            {
                *toF = MSG_ReadLong(msg);
            }
            else
            {
                readbits = MSG_ReadBits(msg, 5);
                readbyte = 32 * MSG_ReadByte(msg) + readbits - 4096;
                *(float *)toF = (float)readbyte;
            }
        }
        else
        {
            unsignedbits = (unsigned int)field->bits >> 31;
            readbits = abs(field->bits);

            if(!strcmp(field->name, "pm_flags") && clientProtocol_to == 1)
                readbits -= 2;

            if((readbits & 7) != 0)
                readbyte = MSG_ReadBits(msg, readbits & 7);
            else
                readbyte = 0;

            for(k = readbits & 7; k < readbits; k += 8)
                readbyte |= MSG_ReadByte(msg) << k;

            if(unsignedbits && ((readbyte >> (readbits - 1)) & 1) != 0)
                readbyte |= ~((1 << readbits) - 1);

            /*if (!strcmp(field->name, "deltaTime"))
            {
                printf("##### readbyte: %i\n", readbyte);
            }*/

            *toF = readbyte;
        }
    }

    for (i = lc, field = &playerStateFields + lc; i < 0x67; i++, field++)
    {
        fromF = (int32_t *)((byte *)from + field->offset);
        toF = (int32_t *)((byte *)to + field->offset);

        if (!strcmp(field->name, "deltaTime"))
        {
            printf("##### (read) toF: %i\n", *toF);
        }
        
        *toF = *fromF;
    }
    
    int statsbits = 0;
    if (MSG_ReadBit(msg))
    {
        statsbits = MSG_ReadBits(msg, 6);
        if((statsbits & 1) != 0)
            to->stats[0] = MSG_ReadShort(msg);
        if((statsbits & 2) != 0)
            to->stats[1] = MSG_ReadShort(msg);
        if((statsbits & 4) != 0)
            to->stats[2] = MSG_ReadShort(msg);
        if((statsbits & 8) != 0)
            to->stats[3] = MSG_ReadBits(msg, 6);
        if((statsbits & 0x10) != 0)
            to->stats[4] = MSG_ReadShort(msg);
        if((statsbits & 0x20) != 0)
            to->stats[5] = MSG_ReadByte(msg);
    }

    int ammobits = 0;
    if (MSG_ReadBit(msg))
    {
        for (i = 0; i < 4; ++i)
        {
            if (MSG_ReadBit(msg))
            {
                ammobits = MSG_ReadShort(msg);
                for (j = 0; j < 16; ++j)
                {
                    if (((ammobits >> j) & 1) != 0)
                    {
                        to->ammo[j + 16 * i] = MSG_ReadShort(msg);
                    }
                }
            }
        }
    }

    int clipbits = 0;
    for (i = 0; i < 4; ++i)
    {
        if (MSG_ReadBit(msg))
        {
            clipbits = MSG_ReadShort(msg);
            for (j = 0; j < 16; ++j)
            {
                if (((clipbits >> j) & 1) != 0)
                {
                    to->ammoclip[j + 16 * i] = MSG_ReadShort(msg);
                }
            }
        }
    }

    if (MSG_ReadBit(msg))
    {
        for (i = 0; i < MAX_OBJECTIVES; ++i)
        {
            to->objective[i].state = MSG_ReadBits(msg, 3);
            MSG_ReadDeltaObjective(msg, &from->objective[i], &to->objective[i], 6, &objectiveFields);
        }
    }

    if (MSG_ReadBit(msg))
    {
        MSG_ReadDeltaHudElems(msg, from->hud.archival, to->hud.archival, MAX_HUDELEMS_ARCHIVAL);
        MSG_ReadDeltaHudElems(msg, from->hud.current, to->hud.current, MAX_HUDELEMS_CURRENT);
    }
}

void hook_ClientCommand(int clientNum)
{
    if(!Scr_IsSystemActive())
        return;

    if (!codecallback_playercommand)
    {
        ClientCommand(clientNum);
        return;
    }

    stackPushArray();
    int args = Cmd_Argc();
    for (int i = 0; i < args; i++)
    {
        char tmp[MAX_STRINGLENGTH];
        trap_Argv(i, tmp, sizeof(tmp));
        if (i == 1 && tmp[0] >= 20 && tmp[0] <= 22)
        {
            char *part = strtok(tmp + 1, " ");
            while (part != NULL)
            {
                stackPushString(part);
                stackPushArrayLast();
                part = strtok(NULL, " ");
            }
        }
        else
        {
            stackPushString(tmp);
            stackPushArrayLast();
        }
    }
    
    short ret = Scr_ExecEntThread(&g_entities[clientNum], codecallback_playercommand, 1);
    Scr_FreeThread(ret);
}

void custom_SV_BotUserMove(client_t *client)
{
    if (!sv_botHook->integer)
    {
        hook_SV_BotUserMove->unhook();
        void (*SV_BotUserMove)(client_t *client);
        *(int*)&SV_BotUserMove = hook_SV_BotUserMove->from;
        SV_BotUserMove(client);
        hook_SV_BotUserMove->hook();
        return;
    }
    
    int num;
    usercmd_t ucmd = {0};

    if(client->gentity == NULL)
        return;

    num = client - svs.clients;
    ucmd.serverTime = svs.time;

    playerState_t *ps = SV_GameClientNum(num);
    gentity_t *ent = &g_entities[num];

    ucmd.weapon = (byte)(ps->weapon & 0xFF);

    if(ent->client == NULL)
        return;

    if (ent->client->sess.archiveTime == 0)
    {
        ucmd.buttons = customPlayerState[num].botButtons;
        VectorCopy(ent->client->sess.cmd.angles, ucmd.angles);
    }

    client->deltaMessage = client->netchan.outgoingSequence - 1;
    SV_ClientThink(client, &ucmd);
}

float custom_PM_GetReducedFriction()
{
    playerState_t *ps;
    int clientProtocol;

    ps = (*pm)->ps;
    clientProtocol = customPlayerState[ps->clientNum].protocol;
    
    if (clientProtocol != 1)
    {
        if(ps->pm_time < 1700)
            return (float)ps->pm_time * 1.5 * 0.00058823527 + 1.0;
        else
            return 2.5;
    }
    else
        return 1.0;
}

float custom_PM_GetLandFactor()
{
    playerState_t *ps;
    int clientProtocol;

    ps = (*pm)->ps;
    clientProtocol = customPlayerState[ps->clientNum].protocol;
    
    if (clientProtocol != 1)
    {
        if(ps->pm_time < 1700)
            return (float)ps->pm_time * 1.5 * 0.00058823527 + 1.0;
        else
            return 2.5;
    }
    else
        return 1.0;
}

void Jump_ClearState(playerState_s *ps)
{
    ps->pm_flags &= ~PMF_JUMPING;
    ps->fJumpOriginZ = 0.0;
}

void Jump_ApplySlowdown(playerState_s *ps)
{
    float scale;
    
    scale = 1.0;
    
    if (ps->pm_time <= 1800)
    {
        if (!ps->pm_time)
        {
            if (ps->fJumpOriginZ + 18.0 <= ps->origin[2])
            {
                ps->pm_time = 1200;
                scale = 0.5;
            }
            else
            {
                ps->pm_time = 1800;
                scale = 0.64999998;
            }
        }
    }
    else
    {
        Jump_ClearState(ps);
        scale = 0.64999998;
    }
    
    VectorScale(ps->velocity, scale, ps->velocity);
}

vec_t VectorLength(const vec3_t v)
{
    return sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
}

void custom_PM_WalkMove()
{
    playerState_t *ps;
    int stance;
    float len;
    float accel;
    usercmd_t cmd;
    float cmdscale;
    float wishspeed;
    vec3_t wishdir;
    float rightmove;
    float forwardmove;
    vec3_t wishvel;
    vec3_t dir;
    int i;
    int clientProtocol;
    
    ps = (*pm)->ps;
    clientProtocol = customPlayerState[ps->clientNum].protocol;
    
    if (ps->pm_flags & PMF_JUMPING)
    {
        if(clientProtocol != 1)
            Jump_ApplySlowdown(ps);
    }
    
    if (PM_CheckJump())
    {
        PM_AirMove();
        ps->jumpTime = (*pm)->cmd.serverTime;
    }
    else
    {
        PM_Friction();
        forwardmove = (float)(*pm)->cmd.forwardmove;
        rightmove = (float)(*pm)->cmd.rightmove;
        cmd = (*pm)->cmd;
        cmdscale = PM_CmdScale_Walk(&cmd);

        pml->forward[2] = 0.0;
        pml->right[2] = 0.0;

        PM_ClipVelocity(pml->forward, pml->groundTrace.normal, pml->forward, OVERCLIP);
        PM_ClipVelocity(pml->right, pml->groundTrace.normal, pml->right, OVERCLIP);

        VectorNormalize(pml->forward);
        VectorNormalize(pml->right);

        for(i = 0; i < 3; ++i)
            dir[i] = pml->forward[i] * forwardmove + pml->right[i] * rightmove;

        VectorCopy(dir, wishdir);
        wishspeed = VectorNormalize(wishdir);
        wishspeed = wishspeed * cmdscale;
        stance = PM_GetEffectiveStance(ps);

        if ((pml->groundTrace.surfaceFlags & 2) || (ps->pm_flags & 0x200))
        {
            accel = 1.0;
        }
        else if (stance == STANCE_EFFECTIVE_PRONE)
        {
            accel = 19.0;
        }
        else if (stance == STANCE_EFFECTIVE_CROUCH)
        {
            accel = 12.0;
        }
        else
        {
            accel = 9.0;
        }

        if(ps->pm_flags & PMF_SLIDING)
            accel = accel * 0.25;

        PM_Accelerate(wishdir, wishspeed, accel);

        if((pml->groundTrace.surfaceFlags & 2) || (ps->pm_flags & 0x200))
            ps->velocity[2] = ps->velocity[2] - (float)ps->gravity * pml->frametime;
        
        len = VectorLength(ps->velocity);
        VectorCopy(ps->velocity, wishvel);
        PM_ClipVelocity(ps->velocity, pml->groundTrace.normal, ps->velocity, OVERCLIP);

        if (DotProduct(ps->velocity, wishvel) > 0.0)
        {
            VectorNormalize(ps->velocity);
            VectorScale(ps->velocity, len, ps->velocity);
        }

        if(ps->velocity[0] != 0.0 || ps->velocity[1] != 0.0)
            PM_StepSlideMove(0);
        
        PM_SetMovementDir();
    }
}

void custom_PM_UpdateLean(playerState_s *ps, usercmd_s *cmd, void (*capsuleTrace)(trace_t *, const vec3_t, const vec3_t, const vec3_t, const vec3_t, int, int))
{
    float fLeanFrac;
    float fLean;
    float fLeanMax;
    trace_t trace;
    float leanofs;
    int leaning;
    vec3_t tmaxs;
    vec3_t tmins;
    vec3_t end;
    vec3_t start;
    int stance;
    int clientProtocol;

    clientProtocol = customPlayerState[ps->clientNum].protocol;

    leaning = 0;
    leanofs = 0.0;

    if ((cmd->wbuttons & (WBUTTON_LEANLEFT | WBUTTON_LEANRIGHT)) != 0
        && (ps->pm_flags & PMF_FREEZE) == 0
        && ps->pm_type <= PM_INTERMISSION
        && (ps->groundEntityNum != 1023 || ps->pm_type == PM_NORMAL_LINKED))
    {
        if((cmd->wbuttons & WBUTTON_LEANLEFT) != 0)
            --leaning;
        if((cmd->wbuttons & WBUTTON_LEANRIGHT) != 0)
            ++leaning;
    }

    if((ps->eFlags & EF_MG42_ACTIVE) != 0)
        leaning = 0;

    stance = PM_GetEffectiveStance(ps);
    if(stance == STANCE_EFFECTIVE_STAND && clientProtocol == 1)
        fLeanMax = 1.0;
    else if(stance == STANCE_EFFECTIVE_PRONE)
        fLeanMax = 0.25;
    else
        fLeanMax = 0.5;
    
    leanofs = ps->leanf;

    if (leaning)
    {
        if (leaning <= 0)
        {
            if(leanofs > -fLeanMax)
                leanofs = leanofs - (float)pml->msec / 350.0 * fLeanMax;
            if(-fLeanMax > (float)leanofs)
                leanofs = -fLeanMax;
        }
        else
        {
            if(fLeanMax > (float)leanofs)
                leanofs = (float)pml->msec / 350.0 * fLeanMax + leanofs;
            if(leanofs > (float)fLeanMax)
                leanofs = fLeanMax;
        }
    }
    else if (leanofs <= 0.0)
    {
        if (leanofs < 0.0)
        {
            leanofs = (float)pml->msec / 280.0 * fLeanMax + leanofs;
            if(leanofs > 0.0)
                leanofs = 0.0;
        }
    }
    else
    {
        leanofs = leanofs - (float)pml->msec / 280.0 * fLeanMax;
        if(leanofs < 0.0)
            leanofs = 0.0;
    }

    ps->leanf = leanofs;

    if (ps->leanf != 0.0)
    {
        auto unknown_func = (int (*)(float))((int)dlsym(libHandle, "_init") + 0x15D8A);
        fLeanFrac = (float)unknown_func(ps->leanf);
        VectorCopy(ps->origin, start);
        start[2] = start[2] + ps->viewHeightCurrent;
        VectorCopy(start, end);
        AddLeanToPosition(end, ps->viewangles[1], fLeanFrac, 16.0, 20.0);
        VectorSet(tmins, -8.0, -8.0, -8.0);
        VectorSet(tmaxs, 8.0, 8.0, 8.0);
        capsuleTrace(&trace, start, tmins, tmaxs, end, ps->clientNum, 42008593);
        fLean = UnGetLeanFraction(trace.fraction);
        if(fabs(ps->leanf) > fLean)
            ps->leanf = (float)unknown_func(ps->leanf) * fLean;
    }
}








void custom_PM_CheckDuck()
{
    vec3_t vEnd;
    float delta;
    vec3_t vPoint;
    playerState_t *ps;
    int stance;
    trace_t trace;
    int bWasProne;
    int clientProtocol;

    ps = (*pm)->ps;
    clientProtocol = customPlayerState[ps->clientNum].protocol;

    float BG_CheckProne_a15;
    if(clientProtocol == 6)
        BG_CheckProne_a15 = 60.0;
    else if(clientProtocol == 1)
        BG_CheckProne_a15 = 0;

    (*pm)->proneChange = 0;
    if (ps->pm_type == PM_SPECTATOR)
    {
        (*pm)->mins[0] = -8.0;
        (*pm)->mins[1] = -8.0;
        (*pm)->mins[2] = -8.0;
        (*pm)->maxs[0] = 8.0;
        (*pm)->maxs[1] = 8.0;
        (*pm)->maxs[2] = 16.0;
        if(clientProtocol == 6)
            ps->pm_flags &= 0xFFFFFFFC;
        else if(clientProtocol == 1)
            ps->pm_flags &= 0xFC;

        if (((*pm)->cmd.wbuttons & WBUTTON_PRONE) != 0)
        {
            (*pm)->cmd.wbuttons &= ~WBUTTON_PRONE;
            BG_AddPredictableEventToPlayerstate(EV_STANCE_FORCE_STAND, 0, ps);
        }

        (*pm)->trace = (*pm)->trace3;
        ps->eFlags |= 0x10;
        ps->viewHeightTarget = 0;
        ps->viewHeightCurrent = 0.0;
    }
    else
    {
        bWasProne = (ps->pm_flags & PMF_PRONE);

        (*pm)->mins[0] = ps->mins[0];
        (*pm)->mins[1] = ps->mins[1];
        (*pm)->maxs[0] = ps->maxs[0];
        (*pm)->maxs[1] = ps->maxs[1];
        (*pm)->mins[2] = ps->mins[2];

        if (ps->pm_type > PM_INTERMISSION)
        {
            (*pm)->maxs[2] = ps->maxs[2];
            ps->viewHeightTarget = ps->deadViewHeight;
            if ((ps->pm_flags & PMF_PRONE) != 0)
            {
                (*pm)->trace = (*pm)->trace2;
            }
            else
            {
                (*pm)->trace = (*pm)->trace3;
            }
            ps->eFlags |= 0x10;
            PM_ViewHeightAdjust();
            return;
        }

        if ((ps->eFlags & EF_MG42_ACTIVE) != 0)
        {
            if ((ps->eFlags & 0x4000) != 0 && (ps->eFlags & 0x8000) == 0)
            {
                ps->pm_flags |= PMF_PRONE;
                ps->pm_flags &= ~PMF_CROUCH;
            }
            else if ((((ps->eFlags & 0x8000) != 0 || ps->eFlags & 0x4000) != 0))
            {
                if(clientProtocol == 6)
                    ps->pm_flags &= 0xFFFFFFFC;
                else if(clientProtocol == 1)
                    ps->pm_flags &= 0xFCu;
            }
            else
            {
                ps->pm_flags |= PMF_CROUCH;
                ps->pm_flags &= ~PMF_PRONE;
            }
        }
        else if ((ps->eFlags & 0x4000) == 0)
        {
            if (((ps->pm_flags & PMF_LADDER) != 0) && (((*pm)->cmd.wbuttons & 0xC0) != 0))
            {
                (*pm)->cmd.wbuttons &= 0x3Fu;
                BG_AddPredictableEventToPlayerstate(EV_STANCE_FORCE_STAND, 0, ps);
            }
            if (((*pm)->cmd.wbuttons & WBUTTON_PRONE) != 0)
            {
                if ((ps->pm_flags & PMF_PRONE) != 0
                    || ((ps->groundEntityNum != 1023 || clientProtocol == 1)
                    && BG_CheckProne(
                        ps->clientNum,
                        ps->origin,
                        (*pm)->maxs[0],
                        30.0,
                        ps->viewangles[1],
                        &ps->fTorsoHeight,
                        &ps->fTorsoPitch,
                        &ps->fWaistPitch,
                        0,
                        ps->groundEntityNum != 1023,
                        0,
                        (*pm)->trace3,
                        (*pm)->trace2,
                        0,
                        BG_CheckProne_a15)))
                {
                    ps->pm_flags |= PMF_PRONE;
                    if(clientProtocol == 6)
                        ps->pm_flags &= ~PMF_CROUCH;
                }
                else if (ps->groundEntityNum != 1023 || clientProtocol == 1)
                {
                    if(clientProtocol == 6)
                        ps->pm_flags |= 0x8000u;
                    else if(clientProtocol == 1)
                    {
                        ps->pm_flags |= 0x80u;
                        ps->pm_flags &= ~4u;
                    }
                    
                    if (((*pm)->cmd.wbuttons & 2) == 0)
                    {
                        if((ps->pm_flags & PMF_CROUCH) != 0)
                            BG_AddPredictableEventToPlayerstate(EV_STANCE_FORCE_CROUCH, 0, ps);
                        else
                            BG_AddPredictableEventToPlayerstate(EV_STANCE_FORCE_STAND, 0, ps);
                    }
                }
            }
            else if (((*pm)->cmd.wbuttons & WBUTTON_CROUCH) != 0)
            {
                if ((ps->pm_flags & PMF_PRONE) == 0)
                {
                    ps->pm_flags |= PMF_CROUCH;
                }
                else
                {
                    (*pm)->maxs[2] = 50.0;
                    (*pm)->trace3(
                        &trace,
                        ps->origin,
                        (*pm)->mins,
                        (*pm)->maxs,
                        ps->origin,
                        ps->clientNum,
                        (*pm)->tracemask & 0xFDFFFFFF);
                    if (!trace.allsolid)
                    {
                        ps->pm_flags &= ~PMF_PRONE;
                        ps->pm_flags |= PMF_CROUCH;
                    }
                    else if (((*pm)->cmd.wbuttons & 2) == 0)
                    {
                        BG_AddPredictableEventToPlayerstate(EV_STANCE_FORCE_PRONE, 0, ps);
                    }
                }
            }
            else
            {
                if ((ps->pm_flags & PMF_PRONE) == 0)
                {
                    if ((ps->pm_flags & PMF_CROUCH) != 0)
                    {
                        (*pm)->maxs[2] = ps->maxs[2];
                        (*pm)->trace3(
                            &trace,
                            ps->origin,
                            (*pm)->mins,
                            (*pm)->maxs,
                            ps->origin,
                            ps->clientNum,
                            (*pm)->tracemask & 0xFDFFFFFF);
                        if (trace.allsolid)
                        {
                            if(((*pm)->cmd.wbuttons & 2) == 0)
                                BG_AddPredictableEventToPlayerstate(EV_STANCE_FORCE_CROUCH, 0, ps);
                        }
                        else
                        {
                            ps->pm_flags &= ~PMF_CROUCH;
                        }
                    }
                }
                else
                {
                    (*pm)->maxs[2] = ps->maxs[2];
                    (*pm)->trace3(
                        &trace,
                        ps->origin,
                        (*pm)->mins,
                        (*pm)->maxs,
                        ps->origin,
                        ps->clientNum,
                        (*pm)->tracemask & 0xFDFFFFFF);
                    
                    if (!trace.allsolid)
                    {
                        ps->pm_flags &= 0xFFFFFFFC;
                    }
                    else
                    {
                        (*pm)->maxs[2] = 50.0;
                        (*pm)->trace3(
                            &trace,
                            ps->origin,
                            (*pm)->mins,
                            (*pm)->maxs,
                            ps->origin,
                            ps->clientNum,
                            (*pm)->tracemask & 0xFDFFFFFF);
                        
                        if (!trace.allsolid)
                        {
                            ps->pm_flags &= ~PMF_PRONE;
                            ps->pm_flags |= PMF_CROUCH;
                        }
                        else if(((*pm)->cmd.wbuttons & 2) == 0)
                        {
                            BG_AddPredictableEventToPlayerstate(EV_STANCE_FORCE_PRONE, 0, ps);
                        }
                    }
                }
            }
        }
        
        if (!ps->viewHeightLerpTime)
        {
            if ((ps->pm_flags & PMF_PRONE) != 0)
            {
                if (ps->viewHeightTarget == ps->standViewHeight)
                {
                    ps->viewHeightTarget = ps->crouchViewHeight;
                }
                else
                {
                    if (g_debugProneCheck && g_debugProneCheck->integer == 2)
                    {
                        BG_CheckProne(
                            ps->clientNum,
                            ps->origin,
                            (*pm)->maxs[0],
                            30.0,
                            ps->viewangles[1],
                            0,
                            0,
                            0,
                            0,
                            ps->groundEntityNum != 1023,
                            0,
                            (*pm)->trace3,
                            (*pm)->trace2,
                            0,
                            BG_CheckProne_a15);
                    }
                    if (ps->viewHeightTarget != ps->proneViewHeight)
                    {
                        ps->viewHeightTarget = ps->proneViewHeight;
                        (*pm)->proneChange = 1;
                        BG_PlayAnim(ps, 0, ANIM_BP_TORSO, 0, 0, 1, 1);
                        if (clientProtocol == 6)
                        {
                            // Jump_ActivateSlowdown
                            ps->pm_flags |= PMF_JUMPING;
                            ps->pm_time = JUMP_LAND_SLOWDOWN_TIME;
                        }
                    }
                }
            }
            else if (ps->viewHeightTarget == ps->proneViewHeight)
            {
                ps->viewHeightTarget = ps->crouchViewHeight;
                (*pm)->proneChange = 1;
                BG_PlayAnim(ps, 0, ANIM_BP_TORSO, 0, 0, 1, 1);
            }
            else
            {
                if((ps->pm_flags & PMF_CROUCH) != 0)
                    ps->viewHeightTarget = ps->crouchViewHeight;
                else
                    ps->viewHeightTarget = ps->standViewHeight;
            }
        }

        PM_ViewHeightAdjust();
        stance = PM_GetEffectiveStance(ps);
        if (stance == STANCE_EFFECTIVE_PRONE)
        {
            (*pm)->maxs[2] = 30.0;
            ps->eFlags |= EF_PRONE;
            ps->eFlags &= ~EF_CROUCHING;
        }
        else
        {
            if (stance == STANCE_EFFECTIVE_CROUCH)
            {
                (*pm)->maxs[2] = 50.0;
                ps->eFlags |= EF_CROUCHING;
                ps->eFlags &= 0xFFFFFFBF;
            }
            else
            {
                (*pm)->maxs[2] = ps->maxs[2];
                ps->eFlags &= 0xFFFFFF9F;
            }
        }

        if ((ps->pm_flags & PMF_PRONE) != 0)
        {
            (*pm)->trace = (*pm)->trace2;
            ps->eFlags |= 0x10;

            if (!bWasProne)
            {
                if ((*pm)->cmd.forwardmove || (*pm)->cmd.rightmove)
                {
                    ps->pm_flags &= ~EF_FIRING;
                    PM_ClearAimDownSightFlag();
                }
                
                VectorCopy(ps->origin, vEnd);
                vEnd[2] += 10.0;
                (*pm)->trace2(
                    &trace,
                    ps->origin,
                    (*pm)->mins,
                    (*pm)->maxs,
                    vEnd,
                    ps->clientNum,
                    (*pm)->tracemask & 0xFDFFFFFF);

                VectorCopy(trace.endpos, vEnd);
                (*pm)->trace2(
                    &trace,
                    vEnd,
                    (*pm)->mins,
                    (*pm)->maxs,
                    ps->origin,
                    ps->clientNum,
                    (*pm)->tracemask & 0xFDFFFFFF);

                VectorCopy(trace.endpos, ps->origin);

                ps->proneDirection = ps->viewangles[1];

                VectorCopy(ps->origin, vPoint);
                vPoint[2] -= 0.25;
                (*pm)->trace2(
                    &trace,
                    ps->origin,
                    (*pm)->mins,
                    (*pm)->maxs,
                    vPoint,
                    ps->clientNum,
                    (*pm)->tracemask & 0xFDFFFFFF);
                
                if (trace.startsolid || trace.fraction >= 1.0)
                {
                    ps->proneDirectionPitch = 0.0;
                }
                else
                {
                    ps->proneDirectionPitch = PitchForYawOnNormal(ps->proneDirection, trace.normal);
                }

                delta = AngleDelta(ps->proneDirectionPitch, ps->viewangles[0]);
                if (delta >= -45.0)
                {
                    if(delta <= 45.0)
                        ps->proneTorsoPitch = ps->proneDirectionPitch;
                    else
                        ps->proneTorsoPitch = ps->viewangles[0] + 45.0;
                }
                else
                {
                    ps->proneTorsoPitch = ps->viewangles[0] - 45.0;
                }
            }
        }
        else
        {
            (*pm)->trace = (*pm)->trace3;
            ps->eFlags |= 0x10;
        }
    }
}

int custom_BG_CheckProne(int passEntityNum, const float *const vPos, float fSize, float fHeight, float fYaw, float * pfTorsoHeight, float * pfTorsoPitch, float * pfWaistPitch, int bAlreadyProne, int bOnGround, float *const vGroundNormal,
    void (*traceFunc)(trace_t *, const vec3_t, const vec3_t, const vec3_t, const vec3_t, int, int),
    void (*traceFunc2)(trace_t *, const vec3_t, const vec3_t, const vec3_t, const vec3_t, int, int),
    int proneCheckType, float prone_feet_dist)
{
    if(customPlayerState[passEntityNum].protocol == 1)
        prone_feet_dist = 0;

    return BG_CheckProneValid(
        passEntityNum,
        vPos,
        fSize,
        fHeight,
        fYaw,
        pfTorsoHeight,
        pfTorsoPitch,
        pfWaistPitch,
        bAlreadyProne,
        bOnGround,
        vGroundNormal,
        traceFunc,
        traceFunc2,
        proneCheckType,
        prone_feet_dist);
}













void custom_PM_UpdateAimDownSightLerp()
{
    float lerpRate;
    playerState_t *ps;
    bool adsRequested;
    int clientProtocol;

    ps = (*pm)->ps;
    clientProtocol = customPlayerState[ps->clientNum].protocol;
    
    if (pml->weaponinfo->aimDownSight)
    {
        adsRequested = 0;

        if ((!pml->weaponinfo->segmentedReload
            && ps->weaponstate == WEAPON_RELOADING
            && ps->weaponTime - pml->weaponinfo->adsReloadTransTime > 0)
            || (pml->weaponinfo->segmentedReload
            && (ps->weaponstate == WEAPON_RELOADING
                || ps->weaponstate == WEAPON_RELOADING_INTERUPT
                || ps->weaponstate == WEAPON_RELOAD_START
                || ps->weaponstate == WEAPON_RELOAD_START_INTERUPT
                || (ps->weaponstate == WEAPON_RELOAD_END && ps->weaponTime - pml->weaponinfo->adsReloadTransTime > 0)))
            || ((!pml->weaponinfo->rechamberWhileAds && clientProtocol != 1) && ps->weaponstate == WEAPON_RECHAMBERING))
        {
            adsRequested = 0;
        }
        else if ((ps->pm_flags & PMF_ZOOMING) != 0)
        {
            adsRequested = 1;
        }

        if(pml->weaponinfo->adsFire && ps->weaponDelay && ps->weaponstate == WEAPON_FIRING)
            adsRequested = 1;

        if ((adsRequested && ps->fWeaponPosFrac != 1.0) || (!adsRequested && ps->fWeaponPosFrac != 0.0))
        {
            float OOPosAnimLength_in;
            OOPosAnimLength_in = pml->weaponinfo->OOPosAnimLength[0];
            if (clientProtocol == 1)
            {
                auto weaponIt = customWeaponinfo.find(pml->weaponinfo->name);
                if (weaponIt != customWeaponinfo.end())
                {
                    auto versionIt = weaponIt->second.find(clientProtocol);
                    if (versionIt != weaponIt->second.end())
                    {
                        float adsTransInTime = versionIt->second.adsTransInTime;
                        OOPosAnimLength_in = 1.0 / adsTransInTime;
                    }
                }
            }

            if(adsRequested)
                lerpRate = (float)pml->msec * OOPosAnimLength_in + ps->fWeaponPosFrac;
            else
                lerpRate = ps->fWeaponPosFrac - (float)pml->msec * pml->weaponinfo->OOPosAnimLength[1];

            ps->fWeaponPosFrac = lerpRate;

            if (ps->fWeaponPosFrac < 1.0)
            {
                if(ps->fWeaponPosFrac <= 0.0)
                    ps->fWeaponPosFrac = 0;
            }
            else
            {
                ps->fWeaponPosFrac = 1.0;
            }
        }
    }
    else
    {
        ps->fWeaponPosFrac = 0.0;
    }
}














void ServerCrash(int sig)
{
    int fd;
    FILE *fp;
    void *array[20];
    size_t size = backtrace(array, 20);

    // Write to crash log
    fp = fopen("./crash.log", "a");
    if (fp)
    {
        fd = fileno(fp);
        fseek(fp, 0, SEEK_END);
        fprintf(fp, "Error: Server crashed with signal 0x%x {%d}\n", sig, sig);
        fflush(fp);
        backtrace_symbols_fd(array, size, fd);
    }
    
    // Write to stderr
    fprintf(stderr, "Error: Server crashed with signal 0x%x {%d}\n", sig, sig);
    backtrace_symbols_fd(array, size, STDERR_FILENO);
    
    system("stty sane");
    exit(1);
}

void *custom_Sys_LoadDll(const char *name, char *fqpath, int (**entryPoint)(int, ...), int (*systemcalls)(int, ...))
{
    hook_Sys_LoadDll->unhook();
    void *(*Sys_LoadDll)(const char *name, char *fqpath, int (**entryPoint)(int, ...), int (*systemcalls)(int, ...));
    *(int*)&Sys_LoadDll = hook_Sys_LoadDll->from;
    libHandle = Sys_LoadDll(name, fqpath, entryPoint, systemcalls);
    hook_Sys_LoadDll->hook();

    //// Unprotect game.mp.i386.so
    // See https://github.com/xtnded/codextended/blob/855df4fb01d20f19091d18d46980b5fdfa95a712/src/librarymodule.c#L55
    char libPath[512];
    char buf[512];
    char flags[4];
    void *low, *high;
    FILE *fp;
    
    if(*fs_game->string)
        sprintf(libPath, "%s/game.mp.i386.so", fs_game->string);
    else
        sprintf(libPath, "main/game.mp.i386.so");
    
    fp = fopen("/proc/self/maps", "r");
    if(!fp)
        return 0;

    while (fgets(buf, sizeof(buf), fp))
    {
        if(!strstr(buf, libPath))
            continue;
        if(sscanf (buf, "%p-%p %4c", &low, &high, flags) != 3)
            continue;
        mprotect((void *)low, (int)high-(int)low, PROT_READ | PROT_WRITE | PROT_EXEC);
    }
    fclose(fp);
    ////

    //// Objects
    g_entities = (gentity_t*)dlsym(libHandle, "g_entities");
    level = (level_locals_t*)dlsym(libHandle, "level");
    playerMins = (vec3_t*)dlsym(libHandle, "playerMins");
    playerMaxs = (vec3_t*)dlsym(libHandle, "playerMaxs");
    pm = (pmove_t**)dlsym(libHandle, "pm");
    pml = (pml_t*)dlsym(libHandle, "pml");
    scr_const = (scr_const_t*)dlsym(libHandle, "scr_const");
    // VM cvars
    bg_viewheight_crouched = (vmCvar_t*)dlsym(libHandle, "bg_viewheight_crouched");
    bg_viewheight_prone = (vmCvar_t*)dlsym(libHandle, "bg_viewheight_prone");
    bg_viewheight_standing = (vmCvar_t*)dlsym(libHandle, "bg_viewheight_standing");
    g_inactivity = (vmCvar_t*)dlsym(libHandle, "g_inactivity");
    ////

    //// Functions
    ClientCommand = (ClientCommand_t)dlsym(libHandle, "ClientCommand");
    Scr_IsSystemActive = (Scr_IsSystemActive_t)dlsym(libHandle, "Scr_IsSystemActive");
    Scr_GetFunctionHandle = (Scr_GetFunctionHandle_t)dlsym(libHandle, "Scr_GetFunctionHandle");
    Scr_GetNumParam = (Scr_GetNumParam_t)dlsym(libHandle, "Scr_GetNumParam");
    Scr_GetFunction = (Scr_GetFunction_t)dlsym(libHandle, "Scr_GetFunction");
    Scr_GetMethod = (Scr_GetMethod_t)dlsym(libHandle, "Scr_GetMethod");
    Scr_Error = (Scr_Error_t)dlsym(libHandle, "Scr_Error");
    Scr_GetInt = (Scr_GetInt_t)dlsym(libHandle, "Scr_GetInt");
    Scr_GetString = (Scr_GetString_t)dlsym(libHandle, "Scr_GetString");
    Scr_AddBool = (Scr_AddBool_t)dlsym(libHandle, "Scr_AddBool");
    Scr_AddInt = (Scr_AddInt_t)dlsym(libHandle, "Scr_AddInt");
    Scr_AddFloat = (Scr_AddFloat_t)dlsym(libHandle, "Scr_AddFloat");
    Scr_AddString = (Scr_AddString_t)dlsym(libHandle, "Scr_AddString");
    Scr_AddUndefined = (Scr_AddUndefined_t)dlsym(libHandle, "Scr_AddUndefined");
    Scr_AddVector = (Scr_AddVector_t)dlsym(libHandle, "Scr_AddVector");
    Scr_MakeArray = (Scr_MakeArray_t)dlsym(libHandle, "Scr_MakeArray");
    Scr_AddArray = (Scr_AddArray_t)dlsym(libHandle, "Scr_AddArray");
    Scr_LoadScript = (Scr_LoadScript_t)dlsym(libHandle, "Scr_LoadScript");
    Scr_ExecEntThread = (Scr_ExecEntThread_t)dlsym(libHandle, "Scr_ExecEntThread");
    Scr_FreeThread = (Scr_FreeThread_t)dlsym(libHandle, "Scr_FreeThread");
    trap_Argv = (trap_Argv_t)dlsym(libHandle, "trap_Argv");
    va = (va_t)dlsym(libHandle, "va");
    VectorNormalize = (VectorNormalize_t)dlsym(libHandle, "VectorNormalize");
    BG_AddPredictableEventToPlayerstate = (BG_AddPredictableEventToPlayerstate_t)dlsym(libHandle, "BG_AddPredictableEventToPlayerstate");
    BG_CheckProne = (BG_CheckProne_t)dlsym(libHandle, "BG_CheckProne");
    BG_PlayAnim = (BG_PlayAnim_t)dlsym(libHandle, "BG_PlayAnim");
    PitchForYawOnNormal = (PitchForYawOnNormal_t)dlsym(libHandle, "PitchForYawOnNormal");
    AngleDelta = (AngleDelta_t)dlsym(libHandle, "AngleDelta");
    PM_CheckJump = (PM_CheckJump_t)((int)dlsym(libHandle, "_init") + 0xCB85);
    PM_AirMove = (PM_AirMove_t)((int)dlsym(libHandle, "_init") + 0xD08D);
    PM_Friction = (PM_Friction_t)((int)dlsym(libHandle, "_init") + 0xBCB3);
    PM_CmdScale_Walk = (PM_CmdScale_Walk_t)((int)dlsym(libHandle, "_init") + 0xC28B);
    PM_ClipVelocity = (PM_ClipVelocity_t)dlsym(libHandle, "PM_ClipVelocity");
    PM_GetEffectiveStance = (PM_GetEffectiveStance_t)dlsym(libHandle, "PM_GetEffectiveStance");
    PM_Accelerate = (PM_Accelerate_t)((int)dlsym(libHandle, "_init") + 0xBF62);
    PM_StepSlideMove = (PM_StepSlideMove_t)dlsym(libHandle, "PM_StepSlideMove");
    PM_SetMovementDir = (PM_SetMovementDir_t)((int)dlsym(libHandle, "_init") + 0xC68F);
    PM_ViewHeightAdjust = (PM_ViewHeightAdjust_t)((int)dlsym(libHandle, "_init") + 0xF8A6);
    PM_ClearAimDownSightFlag = (PM_ClearAimDownSightFlag_t)dlsym(libHandle, "PM_ClearAimDownSightFlag");
    PM_GroundSurfaceType = (PM_GroundSurfaceType_t)((int)dlsym(libHandle, "_init") + 0xC9D7);
    PM_trace = (PM_trace_t)dlsym(libHandle, "PM_trace");
    PM_AddEvent = (PM_AddEvent_t)dlsym(libHandle, "PM_AddEvent");
    PM_FootstepType = (PM_FootstepType_t)((int)dlsym(libHandle, "_init") + 0xE25C);
    G_ClientStopUsingTurret = (G_ClientStopUsingTurret_t)dlsym(libHandle, "G_ClientStopUsingTurret");
    G_EntUnlink = (G_EntUnlink_t)dlsym(libHandle, "G_EntUnlink");
    trap_UnlinkEntity = (trap_UnlinkEntity_t)dlsym(libHandle, "trap_UnlinkEntity");
    Scr_SetString = (Scr_SetString_t)dlsym(libHandle, "Scr_SetString");
    G_SetClientContents = (G_SetClientContents_t)dlsym(libHandle, "G_SetClientContents");
    player_die = (player_die_t)dlsym(libHandle, "player_die");
    trap_GetUsercmd = (trap_GetUsercmd_t)dlsym(libHandle, "trap_GetUsercmd");
    G_SetOrigin = (G_SetOrigin_t)dlsym(libHandle, "G_SetOrigin");
    SetClientViewAngle = (SetClientViewAngle_t)dlsym(libHandle, "SetClientViewAngle");
    ClientEndFrame = (ClientEndFrame_t)dlsym(libHandle, "ClientEndFrame");
    ClientThink_real = (ClientThink_real_t)dlsym(libHandle, "ClientThink_real");
    BG_PlayerStateToEntityState = (BG_PlayerStateToEntityState_t)dlsym(libHandle, "BG_PlayerStateToEntityState");
    G_ClientCanSpectateTeam = (G_ClientCanSpectateTeam_t)dlsym(libHandle, "G_ClientCanSpectateTeam");
    Cmd_FollowCycle_f = (Cmd_FollowCycle_f_t)dlsym(libHandle, "Cmd_FollowCycle_f");
    trap_GetArchivedClientInfo = (trap_GetArchivedClientInfo_t)dlsym(libHandle, "trap_GetArchivedClientInfo");
    StopFollowing = (StopFollowing_t)dlsym(libHandle, "StopFollowing");
    HudElem_UpdateClient = (HudElem_UpdateClient_t)dlsym(libHandle, "HudElem_UpdateClient");
    AddLeanToPosition = (AddLeanToPosition_t)dlsym(libHandle, "AddLeanToPosition");
    UnGetLeanFraction = (UnGetLeanFraction_t)dlsym(libHandle, "UnGetLeanFraction");
    PM_GetLandFactor = (PM_GetLandFactor_t)((int)dlsym(libHandle, "_init") + 0xBC52);
    AngleNormalize180Accurate = (AngleNormalize180Accurate_t)dlsym(libHandle, "AngleNormalize180Accurate");
    AngleNormalize180 = (AngleNormalize180_t)dlsym(libHandle, "AngleNormalize180");
    BG_CheckProneValid = (BG_CheckProneValid_t)dlsym(libHandle, "BG_CheckProneValid");
    ////

    hook_call((int)dlsym(libHandle, "vmMain") + 0xF0, (int)hook_ClientCommand);

    hook_jmp((int)dlsym(libHandle, "_init") + 0xBBF1, (int)custom_PM_GetReducedFriction);
    hook_jmp((int)dlsym(libHandle, "_init") + 0xBC52, (int)custom_PM_GetLandFactor);
    hook_jmp((int)dlsym(libHandle, "_init") + 0xD23D, (int)custom_PM_WalkMove);
    hook_jmp((int)dlsym(libHandle, "PM_UpdateLean"), (int)custom_PM_UpdateLean);


    hook_jmp((int)dlsym(libHandle, "PM_UpdateAimDownSightLerp"), (int)custom_PM_UpdateAimDownSightLerp);


    





    //hook_jmp((int)dlsym(libHandle, "_init") + 0x104C4, (int)custom_PM_CheckDuck);
    //hook_jmp((int)dlsym(libHandle, "BG_CheckProne"), (int)custom_BG_CheckProne);
    





    hook_GScr_LoadGameTypeScript = new cHook((int)dlsym(libHandle, "GScr_LoadGameTypeScript"), (int)custom_GScr_LoadGameTypeScript);
    hook_GScr_LoadGameTypeScript->hook();

    return libHandle;
}

class libcod
{
    public:
    libcod()
    {
#if 0
        printf("sizeof(gclient_t) = %i\n", sizeof(gclient_t));
        exit(0);
#endif
        
        printf("------------- libcod -------------\n");
        printf("Compiled on %s %s using g++ %s\n", __DATE__, __TIME__, __VERSION__);

        // Don't inherit lib of parent
        unsetenv("LD_PRELOAD");

        // Crash handlers for debugging
        signal(SIGSEGV, ServerCrash);
        signal(SIGABRT, ServerCrash);
        
        // Otherwise the printf()'s are printed at crash/end on older os/compiler versions
        // See https://github.com/M-itch/libcod/blob/e58d6a01b11c911fbf886659b6ea67795776cf4a/libcod.cpp#L1346
        setbuf(stdout, NULL);

        // Allow to write in executable memory
        mprotect((void *)0x08048000, 0x135000, PROT_READ | PROT_WRITE | PROT_EXEC);
        
        hook_call(0x080894c5, (int)hook_AuthorizeState);
        hook_call(0x0809d8f5, (int)Scr_GetCustomFunction);
        hook_call(0x0809db31, (int)Scr_GetCustomMethod);

        hook_jmp(0x08089e7e, (int)custom_SV_DirectConnect);
        hook_jmp(0x0808ae44, (int)custom_SV_SendClientGameState);
        hook_jmp(0x08081dd3, (int)custom_MSG_WriteDeltaPlayerstate);
        hook_jmp(0x08082640, (int)custom_MSG_ReadDeltaPlayerstate);

        
        hook_Sys_LoadDll = new cHook(0x080d3cdd, (int)custom_Sys_LoadDll);
        hook_Sys_LoadDll->hook();
        hook_Com_Init = new cHook(0x08070ef8, (int)custom_Com_Init);
        hook_Com_Init->hook();
        hook_SV_BotUserMove = new cHook(0x080940d2, (int)custom_SV_BotUserMove);
        hook_SV_BotUserMove->hook();
        hook_SV_SpawnServer = new cHook(0x08090d0f, (int)custom_SV_SpawnServer);
        hook_SV_SpawnServer->hook();
        
        printf("Loading complete\n");
        printf("-----------------------------------\n");
    }

    ~libcod()
    {
        printf("Libcod unloaded\n");
        system("stty sane");
    }
};

libcod *lc;
void __attribute__ ((constructor)) lib_load(void)
{
    lc = new libcod;
}
void __attribute__ ((destructor)) lib_unload(void)
{
    delete lc;
}