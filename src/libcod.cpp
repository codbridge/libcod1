//test

#include "gsc.hpp"
#include "libcod.hpp"

#include <signal.h>

// Stock cvars
cvar_t* cl_paused;
cvar_t* com_dedicated;
cvar_t* com_logfile;
cvar_t* com_sv_running;
cvar_t* sv_allowDownload;
cvar_t* sv_pure;
cvar_t* sv_rconPassword;
cvar_t* sv_serverid;

// Custom cvars
cvar_t* fs_callbacks;
cvar_t* g_deadChat;
cvar_t* g_debugCallbacks;
cvar_t* g_legacyStyle;
cvar_t* g_playerEject;
cvar_t* jump_slowdownEnable;
cvar_t* sv_cracked;

cHook* hook_com_init;
cHook* hook_cvar_set2;
cHook* hook_g_localizedstringindex;
cHook* hook_gametype_scripts;
cHook* hook_pm_walkmove;
cHook* hook_sv_spawnserver;
cHook* hook_sv_begindownload_f;
cHook* hook_sv_maprestart_f;
cHook* hook_sys_loaddll;

// Stock callbacks
int codecallback_startgametype = 0;
int codecallback_playerconnect = 0;
int codecallback_playerdisconnect = 0;
int codecallback_playerdamage = 0;
int codecallback_playerkilled = 0;

// Custom callbacks
int codecallback_client_spam = 0;
int codecallback_playercommand = 0;

callback_t callbacks[] =
{
    { &codecallback_startgametype, "CodeCallback_StartGameType" }, // g_scr_data.gametype.startupgametype
    { &codecallback_playerconnect, "CodeCallback_PlayerConnect" }, // g_scr_data.gametype.playerconnect
    { &codecallback_playerdisconnect, "CodeCallback_PlayerDisconnect" }, // g_scr_data.gametype.playerdisconnect
    { &codecallback_playerdamage, "CodeCallback_PlayerDamage" }, // g_scr_data.gametype.playerdamage
    { &codecallback_playerkilled, "CodeCallback_PlayerKilled" }, // g_scr_data.gametype.playerkilled

    { &codecallback_client_spam, "CodeCallback_CLSpam"},
    { &codecallback_playercommand, "CodeCallback_PlayerCommand"},
};

// Game lib objects
gentity_t* g_entities;
pmove_t* pm;

// Game lib functions
Scr_GetFunctionHandle_t Scr_GetFunctionHandle;
Scr_GetNumParam_t Scr_GetNumParam;
SV_Cmd_ArgvBuffer_t SV_Cmd_ArgvBuffer;
ClientCommand_t ClientCommand;
Scr_GetFunction_t Scr_GetFunction;
Scr_GetMethod_t Scr_GetMethod;
SV_GameSendServerCommand_t SV_GameSendServerCommand;
Scr_ExecThread_t Scr_ExecThread;
Scr_ExecEntThread_t Scr_ExecEntThread;
Scr_ExecEntThreadNum_t Scr_ExecEntThreadNum;
Scr_FreeThread_t Scr_FreeThread;
Scr_Error_t Scr_Error;
G_Say_t G_Say;
SV_GetConfigstringConst_t SV_GetConfigstringConst;
SV_GetConfigstring_t SV_GetConfigstring;
#if COD_VERSION == COD1_1_5
BG_GetInfoForWeapon_t BG_GetInfoForWeapon;
#endif
BG_GetWeaponIndexForName_t BG_GetWeaponIndexForName;
Scr_IsSystemActive_t Scr_IsSystemActive;
Scr_GetInt_t Scr_GetInt;
Scr_GetString_t Scr_GetString;
Scr_GetType_t Scr_GetType;
Scr_GetEntity_t Scr_GetEntity;
Scr_AddBool_t Scr_AddBool;
Scr_AddInt_t Scr_AddInt;
Scr_AddFloat_t Scr_AddFloat;
Scr_AddString_t Scr_AddString;
Scr_AddUndefined_t Scr_AddUndefined;
Scr_AddVector_t Scr_AddVector;
Scr_MakeArray_t Scr_MakeArray;
Scr_AddArray_t Scr_AddArray;
Scr_AddObject_t Scr_AddObject;
Scr_LoadScript_t Scr_LoadScript;
StuckInClient_t StuckInClient;
Q_strlwr_t Q_strlwr;
Q_strupr_t Q_strupr;
Q_strcat_t Q_strcat;

// Resume addresses
uintptr_t resume_addr_PM_WalkMove;
uintptr_t resume_addr_PM_SlideMove;

#if COD_VERSION == COD1_1_5
#include <map>
#include <string>
std::map<std::string, std::map<std::string, WeaponProperties>> weapons_properties;

void toggleLegacyStyle(bool enable)
{
    if(enable)
        Cvar_Set2("jump_slowdownEnable", "0", qfalse);
    else
        Cvar_Set2("jump_slowdownEnable", "1", qfalse);

    int id_kar98k_sniper = BG_GetWeaponIndexForName("kar98k_sniper_mp");
    WeaponDef_t* weapon_kar98k_sniper = BG_GetInfoForWeapon(id_kar98k_sniper);
    int id_springfield = BG_GetWeaponIndexForName("springfield_mp");
    WeaponDef_t* weapon_springfield = BG_GetInfoForWeapon(id_springfield);
    int id_mosin_nagant_sniper = BG_GetWeaponIndexForName("mosin_nagant_sniper_mp");
    WeaponDef_t* weapon_mosin_nagant_sniper = BG_GetInfoForWeapon(id_mosin_nagant_sniper);

    if (weapon_kar98k_sniper)
    {
        const WeaponProperties* properties_kar98k_sniper = nullptr;
        if(enable)
            properties_kar98k_sniper = &weapons_properties[weapon_kar98k_sniper->name]["legacy"];
        else
            properties_kar98k_sniper = &weapons_properties[weapon_kar98k_sniper->name]["default"];
        weapon_kar98k_sniper->adsTransInTime = properties_kar98k_sniper->adsTransInTime;
        weapon_kar98k_sniper->OOPosAnimLength[0] = 1.0 / (float)weapon_kar98k_sniper->adsTransInTime;
        weapon_kar98k_sniper->adsZoomInFrac = properties_kar98k_sniper->adsZoomInFrac;
        weapon_kar98k_sniper->idleCrouchFactor = properties_kar98k_sniper->idleCrouchFactor;
        weapon_kar98k_sniper->idleProneFactor = properties_kar98k_sniper->idleProneFactor;
        weapon_kar98k_sniper->rechamberWhileAds = properties_kar98k_sniper->rechamberWhileAds;
        weapon_kar98k_sniper->adsViewErrorMin = properties_kar98k_sniper->adsViewErrorMin;
        weapon_kar98k_sniper->adsViewErrorMax = properties_kar98k_sniper->adsViewErrorMax;
    }

    if (weapon_mosin_nagant_sniper)
    {
        const WeaponProperties* properties_mosin_nagant_sniper = nullptr;
        if(enable)
            properties_mosin_nagant_sniper = &weapons_properties[weapon_mosin_nagant_sniper->name]["legacy"];
        else
            properties_mosin_nagant_sniper = &weapons_properties[weapon_mosin_nagant_sniper->name]["default"];
        weapon_mosin_nagant_sniper->reloadAddTime = properties_mosin_nagant_sniper->reloadAddTime;
        weapon_mosin_nagant_sniper->adsTransInTime = properties_mosin_nagant_sniper->adsTransInTime;
        weapon_mosin_nagant_sniper->OOPosAnimLength[0] = 1.0 / (float)weapon_mosin_nagant_sniper->adsTransInTime;
        weapon_mosin_nagant_sniper->adsZoomInFrac = properties_mosin_nagant_sniper->adsZoomInFrac;
        weapon_mosin_nagant_sniper->idleCrouchFactor = properties_mosin_nagant_sniper->idleCrouchFactor;
        weapon_mosin_nagant_sniper->idleProneFactor = properties_mosin_nagant_sniper->idleProneFactor;
        weapon_mosin_nagant_sniper->rechamberWhileAds = properties_mosin_nagant_sniper->rechamberWhileAds;
        weapon_mosin_nagant_sniper->adsViewErrorMin = properties_mosin_nagant_sniper->adsViewErrorMin;
        weapon_mosin_nagant_sniper->adsViewErrorMax = properties_mosin_nagant_sniper->adsViewErrorMax;
    }

    if (weapon_springfield)
    {
        const WeaponProperties* properties_springfield = nullptr;
        if(enable)
            properties_springfield = &weapons_properties[weapon_springfield->name]["legacy"];
        else
            properties_springfield = &weapons_properties[weapon_springfield->name]["default"];
        weapon_springfield->adsTransInTime = properties_springfield->adsTransInTime;
        weapon_springfield->OOPosAnimLength[0] = 1.0 / (float)weapon_springfield->adsTransInTime;
        weapon_springfield->adsZoomInFrac = properties_springfield->adsZoomInFrac;
        weapon_springfield->idleCrouchFactor = properties_springfield->idleCrouchFactor;
        weapon_springfield->idleProneFactor = properties_springfield->idleProneFactor;
        weapon_springfield->rechamberWhileAds = properties_springfield->rechamberWhileAds;
        weapon_springfield->adsViewErrorMin = properties_springfield->adsViewErrorMin;
        weapon_springfield->adsViewErrorMax = properties_springfield->adsViewErrorMax;
    }
}
#endif

#if COD_VERSION == COD1_1_5
void custom_Cvar_Set2(const char *var_name, const char *value, qboolean force)
{
    bool check_g_legacyStyle = false;
    bool g_legacyStyle_before;
    bool g_legacyStyle_after;

    if(com_sv_running != NULL && com_sv_running->integer)
    {
        if(!strcasecmp(var_name, g_legacyStyle->name))
        {
            check_g_legacyStyle = true;
            g_legacyStyle_before = g_legacyStyle->integer ? true : false;
        }
    }
    
    hook_cvar_set2->unhook();
    cvar_t* (*Cvar_Set2)(const char *var_name, const char *value, qboolean force);
    *(int *)&Cvar_Set2 = hook_cvar_set2->from;

    if(check_g_legacyStyle)
    {
        cvar_t* var = Cvar_Set2(var_name, value, force);
        if(var)
        {
            g_legacyStyle_after = var->integer ? true : false;
            if(g_legacyStyle_before != g_legacyStyle_after)
                toggleLegacyStyle(var->integer);
        }
    }
    else
        Cvar_Set2(var_name, value, force);

    hook_cvar_set2->hook();
}
#endif

void custom_Com_Init(char *commandLine)
{
    hook_com_init->unhook();
    void (*Com_Init)(char *commandLine);
    *(int *)&Com_Init = hook_com_init->from;
    Com_Init(commandLine);
    hook_com_init->hook();
    
    // Get references to stock cvars
    cl_paused = Cvar_FindVar("cl_paused");
    com_dedicated = Cvar_FindVar("dedicated");
    com_logfile = Cvar_FindVar("logfile");
    com_sv_running = Cvar_FindVar("sv_running");
    sv_allowDownload = Cvar_FindVar("sv_allowDownload");
    sv_pure = Cvar_FindVar("sv_pure");
    sv_rconPassword = Cvar_FindVar("rconpassword");
    sv_serverid = Cvar_FindVar("sv_serverid");

    // Register custom cvars
    Cvar_Get("libcod", "1", CVAR_SERVERINFO);

    fs_callbacks = Cvar_Get("fs_callbacks", "", CVAR_ARCHIVE);
#if COD_VERSION == COD1_1_1
    g_deadChat = Cvar_Get("g_deadChat", "0", CVAR_ARCHIVE);
#endif
    g_debugCallbacks = Cvar_Get("g_debugCallbacks", "0", CVAR_ARCHIVE);
#if COD_VERSION == COD1_1_5
    g_legacyStyle = Cvar_Get("g_legacyStyle", "0", CVAR_SYSTEMINFO | CVAR_ARCHIVE);
#endif
    g_playerEject = Cvar_Get("g_playerEject", "1", CVAR_ARCHIVE);
#if COD_VERSION == COD1_1_5
    jump_slowdownEnable =  Cvar_Get("jump_slowdownEnable", "1", CVAR_SYSTEMINFO | CVAR_ARCHIVE);
#endif
    sv_cracked = Cvar_Get("sv_cracked", "0", CVAR_ARCHIVE);
}

#if COD_VERSION == COD1_1_1
void hook_G_Say(gentity_s *ent, gentity_s *target, int mode, const char *chatText)
{
    // 1.1 deadchat support
    /* See:
    - https://github.com/xtnded/codextended/blob/855df4fb01d20f19091d18d46980b5fdfa95a712/src/sv_client.c#L940
    */

    int unknown_var = *(int*)((int)ent->client + 8400);
    if(unknown_var && !g_deadChat->integer)
        return;

    G_Say(ent, NULL, mode, chatText);
}
#endif

#if COD_VERSION == COD1_1_1
qboolean FS_svrPak(const char *base)
{
    if(strstr(base, "_svr_"))
        return qtrue;
    return qfalse;
}
#endif

const char* custom_FS_ReferencedPakNames(void)
{
    static char info[BIG_INFO_STRING];
    searchpath_t *search;

    info[0] = 0;
    
    for ( search = fs_searchpaths ; search ; search = search->next )
    {
        if (!search->pak)
            continue;

        if(FS_svrPak(search->pak->pakBasename))
            continue;

        if(*info)
            Q_strcat(info, sizeof( info ), " ");
        Q_strcat(info, sizeof( info ), search->pak->pakGamename);
        Q_strcat(info, sizeof( info ), "/");
        Q_strcat(info, sizeof( info ), search->pak->pakBasename);
    }

    return info;
}

const char* custom_FS_ReferencedPakChecksums(void)
{
    static char info[BIG_INFO_STRING];
    searchpath_t *search;
    
    info[0] = 0;

    for ( search = fs_searchpaths ; search ; search = search->next )
    {
        if (!search->pak)
            continue;
        
        if(FS_svrPak(search->pak->pakBasename))
            continue;
        
        Q_strcat( info, sizeof( info ), custom_va( "%i ", search->pak->checksum ) );
    }

    return info;
}

int custom_GScr_LoadGameTypeScript()
{
    unsigned int i;
    char path_for_cb[512] = "maps/mp/gametypes/_callbacksetup";
    char path_for_cb_custom[512] = "callback"; //TODO: maybe don't use another hardcoded path for custom callbacks

    hook_gametype_scripts->unhook();
    int (*GScr_LoadGameTypeScript)();
    *(int *)&GScr_LoadGameTypeScript = hook_gametype_scripts->from;
    int ret = GScr_LoadGameTypeScript();
    hook_gametype_scripts->hook();

    if(!Scr_LoadScript(path_for_cb_custom))
        printf("####### custom_GScr_LoadGameTypeScript: Didn't detect a custom callback file. \n");

    if ( strlen(fs_callbacks->string) )
        strncpy(path_for_cb, fs_callbacks->string, sizeof(path_for_cb));
        
    for ( i = 0; i < sizeof(callbacks)/sizeof(callbacks[0]); i++ )
    {
        if(!strcmp(callbacks[i].name, "CodeCallback_PlayerCommand")) // Custom callback
            *callbacks[i].pos = Scr_GetFunctionHandle(path_for_cb_custom, callbacks[i].name);
        else
            *callbacks[i].pos = Scr_GetFunctionHandle(path_for_cb, callbacks[i].name);
        
        /*if ( *callbacks[i].pos && g_debugCallbacks->integer )
            Com_Printf("%s found @ %p\n", callbacks[i].name, scrVarPub.programBuffer + *callbacks[i].pos);*/ //TODO: verify scrVarPub_t
    }

    return ret;
}

static int localized_string_index = 128;
int custom_G_LocalizedStringIndex(const char *string)
{
    // See https://github.com/xtnded/codextended/blob/855df4fb01d20f19091d18d46980b5fdfa95a712/src/script.c#L944
    
    int i;
    char s[MAX_STRINGLENGTH];

    if(localized_string_index >= 256)
        localized_string_index = 128;

    if(!string || !*string)
        return 0;
    
    int start = 1244;

    for(i = 1; i < 256; i++)
    {
        SV_GetConfigstring(start + i, s, sizeof(s));
        if(!*s)
            break;
        if (!strcmp(s, string))
            return i;
    }
    if(i == 256)
        i = localized_string_index;

    SV_SetConfigstring(i + 1244, string);
    ++localized_string_index;
    
    return i;
}

void hook_ClientCommand(int clientNum)
{
    if ( !Scr_IsSystemActive() )
        return;

#if COD_VERSION == COD1_1_1
    char* cmd = Cmd_Argv(0);
    if(!strcmp(cmd, "gc"))
        return; // Prevent server crash
#endif
      
    if ( !codecallback_playercommand )
    {	ClientCommand(clientNum);
        return;
    }

    stackPushArray();
    int args = Cmd_Argc();
    for ( int i = 0; i < args; i++ )
    {
        char tmp[MAX_STRINGLENGTH];
        SV_Cmd_ArgvBuffer(i, tmp, sizeof(tmp));
        if( i == 1 && tmp[0] >= 20 && tmp[0] <= 22 )
        {
            char *part = strtok(tmp + 1, " ");
            while( part != NULL )
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

const char* hook_AuthorizeState(int arg)
{
    const char* s = Cmd_Argv(arg);
    if (sv_cracked->integer && !strcmp(s, "deny"))
        return "accept";
    return s;
}

void custom_SV_SpawnServer(char *server)
{
    hook_sv_spawnserver->unhook();
    void (*SV_SpawnServer)(char *server);
    *(int *)&SV_SpawnServer = hook_sv_spawnserver->from;
    SV_SpawnServer(server);
    hook_sv_spawnserver->hook();

#if COD_VERSION == COD1_1_5
    if (weapons_properties.empty())
    {
        weapons_properties["kar98k_sniper_mp"]["default"] = { 199, 449, 0.1, 0.6, 0.2, 0, 1.2, 1.4 };
        weapons_properties["kar98k_sniper_mp"]["legacy"] = { 199, 299, 0.42, 0.2, 0.085, 1, 0, 0 };

        weapons_properties["mosin_nagant_sniper_mp"]["default"] = { 1339, 449, 0.1, 0.6, 0.2, 0, 1.2, 1.4 };
        weapons_properties["mosin_nagant_sniper_mp"]["legacy"] = { 339, 299, 0.42, 0.2, 0.085, 1, 0, 0 };

        weapons_properties["springfield_mp"]["default"] = { 199, 449, 0.1, 0.6, 0.2, 0, 1.2, 1.4 };
        weapons_properties["springfield_mp"]["legacy"] = { 199, 299, 0.5, 0.2, 0.085, 1, 0, 0 };
        /*
        springfield_mp adsZoomInFrac in 1.1 patch weapon file = 0.05.
        There must be an error somewhere. Now replacing by 0.5 to fix slowness.
        */
    }

    if(g_legacyStyle->integer)
        toggleLegacyStyle(true);
#endif

#if COMPILE_SQLITE == 1
    free_sqlite_db_stores_and_tasks();
#endif
}

qboolean hook_StuckInClient(gentity_s *self)
{
    if (!g_playerEject->integer)
		return qfalse;
    return StuckInClient(self);
}

qboolean ShouldServeFile(const char *requestedFilePath)
{
    static char localFilePath[MAX_OSPATH*2+5];
    searchpath_t *search;

    localFilePath[0] = 0;

    for(search = fs_searchpaths; search; search = search->next)
    {
        if(search->pak)
        {
            snprintf(localFilePath, sizeof(localFilePath), "%s/%s.pk3", search->pak->pakGamename, search->pak->pakBasename);
            if (!strcmp(localFilePath, requestedFilePath))
                if(!FS_svrPak(search->pak->pakBasename))
                        return qtrue;
        }
    }
    return qfalse;
}

void custom_SV_BeginDownload_f(client_t *cl)
{
    // Patch q3dirtrav
    int args = Cmd_Argc();
    if(args > 1)
    {
        const char* arg1 = Cmd_Argv(1);
        if(!ShouldServeFile(arg1))
        {
            char ip[16];
            snprintf(ip, sizeof(ip), "%d.%d.%d.%d", cl->netchan.remoteAddress.ip[0], cl->netchan.remoteAddress.ip[1], cl->netchan.remoteAddress.ip[2], cl->netchan.remoteAddress.ip[3]);
            printf("####### WARNING: %s (%s) tried to download %s. \n", cl->name, ip, arg1);
            return;
        }
    }

    hook_sv_begindownload_f->unhook();
    void (*SV_BeginDownload_f)(client_t *cl);
    *(int *)&SV_BeginDownload_f = hook_sv_begindownload_f->from;
    SV_BeginDownload_f(cl);
    hook_sv_begindownload_f->hook();
}

#if COD_VERSION == COD1_1_5
void custom_SV_MapRestart_f(void)
{
    hook_sv_maprestart_f->unhook();
    void (*SV_MapRestart_f)();
    *(int *)&SV_MapRestart_f = hook_sv_maprestart_f->from;
    SV_MapRestart_f();
    hook_sv_maprestart_f->hook();
    
    if(g_legacyStyle != NULL && g_legacyStyle->integer)
        toggleLegacyStyle(true);
}
#endif

char *custom_va(const char *format, ...)
{
    // See https://github.com/xtnded/codextended/blob/855df4fb01d20f19091d18d46980b5fdfa95a712/src/shared.c#L632

    #define MAX_VA_STRING 32000
    va_list argptr;
    static char temp_buffer[MAX_VA_STRING];
    static char string[MAX_VA_STRING];
    static int index = 0;
    char *buf;
    int len;

    va_start( argptr, format );
    vsprintf( temp_buffer, format, argptr );
    va_end( argptr );

    if ( ( len = strlen( temp_buffer ) ) >= MAX_VA_STRING )
        Com_Error( ERR_DROP, "Attempted to overrun string in call to va() \n" );

    if ( len + index >= MAX_VA_STRING - 1 )
        index = 0;

    buf = &string[index];
    memcpy( buf, temp_buffer, len + 1 );

    index += len + 1;

    return buf;
}

// ioquake3 rate limit connectionless requests
// https://github.com/ioquake/ioq3/blob/master/code/server/sv_main.c

// This is deliberately quite large to make it more of an effort to DoS
#define MAX_BUCKETS	16384
#define MAX_HASHES 1024

static leakyBucket_t buckets[MAX_BUCKETS];
static leakyBucket_t* bucketHashes[MAX_HASHES];
leakyBucket_t outboundLeakyBucket;

static long SVC_HashForAddress(netadr_t address)
{
    unsigned char *ip = address.ip;
    int	i;
    long hash = 0;

    for ( i = 0; i < 4; i++ )
    {
        hash += (long)( ip[i] ) * ( i + 119 );
    }

    hash = ( hash ^ ( hash >> 10 ) ^ ( hash >> 20 ) );
    hash &= ( MAX_HASHES - 1 );

    return hash;
}

static leakyBucket_t * SVC_BucketForAddress(netadr_t address, int burst, int period)
{
    leakyBucket_t *bucket = NULL;
    int i;
    long hash = SVC_HashForAddress(address);
    uint64_t now = Sys_Milliseconds64();

    for ( bucket = bucketHashes[hash]; bucket; bucket = bucket->next )
    {
        if ( memcmp(bucket->adr, address.ip, 4) == 0 )
            return bucket;
    }

    for ( i = 0; i < MAX_BUCKETS; i++ )
    {
        int interval;

        bucket = &buckets[i];
        interval = now - bucket->lastTime;

        // Reclaim expired buckets
        if ( bucket->lastTime > 0 && ( interval > ( burst * period ) ||
                                       interval < 0 ) )
        {
            if ( bucket->prev != NULL )
                bucket->prev->next = bucket->next;
            else
                bucketHashes[bucket->hash] = bucket->next;

            if ( bucket->next != NULL )
                bucket->next->prev = bucket->prev;

            memset(bucket, 0, sizeof(leakyBucket_t));
        }

        if ( bucket->type == 0 )
        {
            bucket->type = address.type;
            memcpy(bucket->adr, address.ip, 4);

            bucket->lastTime = now;
            bucket->burst = 0;
            bucket->hash = hash;

            // Add to the head of the relevant hash chain
            bucket->next = bucketHashes[hash];
            if ( bucketHashes[hash] != NULL )
                bucketHashes[hash]->prev = bucket;

            bucket->prev = NULL;
            bucketHashes[hash] = bucket;

            return bucket;
        }
    }

    // Couldn't allocate a bucket for this address
    return NULL;
}

bool SVC_RateLimit(leakyBucket_t *bucket, int burst, int period)
{
    if ( bucket != NULL )
    {
        uint64_t now = Sys_Milliseconds64();
        int interval = now - bucket->lastTime;
        int expired = interval / period;
        int expiredRemainder = interval % period;

        if ( expired > bucket->burst || interval < 0 )
        {
            bucket->burst = 0;
            bucket->lastTime = now;
        }
        else
        {
            bucket->burst -= expired;
            bucket->lastTime = now - expiredRemainder;
        }

        if ( bucket->burst < burst )
        {
            bucket->burst++;
            return false;
        }
    }

    return true;
}

bool SVC_RateLimitAddress(netadr_t from, int burst, int period)
{
    leakyBucket_t *bucket = SVC_BucketForAddress(from, burst, period);

    return SVC_RateLimit(bucket, burst, period);
}

bool SVC_callback(const char *str, const char *ip)
{	
    if ( codecallback_client_spam && Scr_IsSystemActive() )
    {
        stackPushString(ip);
        stackPushString(str);
        short ret = Scr_ExecThread(codecallback_client_spam, 2);
        Scr_FreeThread(ret);

        return true;
    }
    
    return false;
}

bool SVC_ApplyRconLimit(netadr_t from, qboolean badRconPassword)
{
    // Prevent using rcon as an amplifier and make dictionary attacks impractical
    if ( SVC_RateLimitAddress(from, 10, 1000) )
    {
        if ( !SVC_callback("RCON:ADDRESS", NET_AdrToString(from)) )
            Com_DPrintf("SVC_RemoteCommand: rate limit from %s exceeded, dropping request\n", NET_AdrToString(from));
        return true;
    }
    
    if ( badRconPassword )
    {
        static leakyBucket_t bucket;

        // Make DoS via rcon impractical
        if ( SVC_RateLimit(&bucket, 10, 1000) )
        {
            if ( !SVC_callback("RCON:GLOBAL", NET_AdrToString(from)) )
                Com_DPrintf("SVC_RemoteCommand: rate limit exceeded, dropping request\n");
            return true;
        }
    }
    
    return false;
}

bool SVC_ApplyStatusLimit(netadr_t from)
{
    // Prevent using getstatus as an amplifier
    if ( SVC_RateLimitAddress(from, 10, 1000) )
    {
        if ( !SVC_callback("STATUS:ADDRESS", NET_AdrToString(from)) )
            Com_DPrintf("SVC_Status: rate limit from %s exceeded, dropping request\n", NET_AdrToString(from));
        return true;
    }

    // Allow getstatus to be DoSed relatively easily, but prevent
    // excess outbound bandwidth usage when being flooded inbound
    if ( SVC_RateLimit(&outboundLeakyBucket, 10, 100) )
    {
        if ( !SVC_callback("STATUS:GLOBAL", NET_AdrToString(from)) )
            Com_DPrintf("SVC_Status: rate limit exceeded, dropping request\n");
        return true;
    }

    return false;
}

void hook_SV_GetChallenge(netadr_t from)
{
    // Prevent using getchallenge as an amplifier
    if ( SVC_RateLimitAddress(from, 10, 1000) )
    {
        if ( !SVC_callback("CHALLENGE:ADDRESS", NET_AdrToString(from)) )
            Com_DPrintf("SV_GetChallenge: rate limit from %s exceeded, dropping request\n", NET_AdrToString(from));
        return;
    }

    // Allow getchallenge to be DoSed relatively easily, but prevent
    // excess outbound bandwidth usage when being flooded inbound
    if ( SVC_RateLimit(&outboundLeakyBucket, 10, 100) )
    {
        if ( !SVC_callback("CHALLENGE:GLOBAL", NET_AdrToString(from)) )
            Com_DPrintf("SV_GetChallenge: rate limit exceeded, dropping request\n");
        return;
    }

    SV_GetChallenge(from);
}

void hook_SV_DirectConnect(netadr_t from)
{
    // Prevent using connect as an amplifier
    if ( SVC_RateLimitAddress(from, 10, 1000) )
    {
        Com_DPrintf("SV_DirectConnect: rate limit from %s exceeded, dropping request\n", NET_AdrToString(from));
        return;
    }

    // Allow connect to be DoSed relatively easily, but prevent
    // excess outbound bandwidth usage when being flooded inbound
    if ( SVC_RateLimit(&outboundLeakyBucket, 10, 100) )
    {
        Com_DPrintf("SV_DirectConnect: rate limit exceeded, dropping request\n");
        return;
    }

    SV_DirectConnect(from);
}

void hook_SV_AuthorizeIpPacket(netadr_t from)
{
    // Prevent ipAuthorize log spam DoS
    if ( SVC_RateLimitAddress(from, 20, 1000) )
    {
        Com_DPrintf("SV_AuthorizeIpPacket: rate limit from %s exceeded, dropping request\n", NET_AdrToString(from));
        return;
    }

    // Allow ipAuthorize to be DoSed relatively easily, but prevent
    // excess outbound bandwidth usage when being flooded inbound
    if ( SVC_RateLimit(&outboundLeakyBucket, 10, 100) )
    {
        Com_DPrintf("SV_AuthorizeIpPacket: rate limit exceeded, dropping request\n");
        return;
    }

    SV_AuthorizeIpPacket(from);
}

void hook_SVC_Info(netadr_t from)
{
    // Prevent using getinfo as an amplifier
    if ( SVC_RateLimitAddress(from, 10, 1000) )
    {
        if ( !SVC_callback("INFO:ADDRESS", NET_AdrToString(from)) )
            Com_DPrintf("SVC_Info: rate limit from %s exceeded, dropping request\n", NET_AdrToString(from));
        return;
    }

    // Allow getinfo to be DoSed relatively easily, but prevent
    // excess outbound bandwidth usage when being flooded inbound
    if ( SVC_RateLimit(&outboundLeakyBucket, 10, 100) )
    {
        if ( !SVC_callback("INFO:GLOBAL", NET_AdrToString(from)) )
            Com_DPrintf("SVC_Info: rate limit exceeded, dropping request\n");
        return;
    }

    SVC_Info(from);
}

void hook_SVC_Status(netadr_t from)
{
    if ( SVC_ApplyStatusLimit(from) )
        return;
    
    SVC_Status(from);
}

void hook_SVC_RemoteCommand(netadr_t from, msg_t *msg)
{
    char* password = Cmd_Argv(1);
    qboolean badRconPassword = !strlen(sv_rconPassword->string) || !strcmp_constant_time(password, sv_rconPassword->string);
    
    if ( SVC_ApplyRconLimit(from, badRconPassword) )
        return;
    
    SVC_RemoteCommand(from, msg);
}

void ServerCrash(int sig)
{
    int fd;
    FILE *fp;
    void *array[20];
    size_t size = backtrace(array, 20);

    // Write to crash log ...
    fp = fopen("./crash.log", "a");
    if ( fp )
    {
        fd = fileno(fp);
        fseek(fp, 0, SEEK_END);
        fprintf(fp, "Error: Server crashed with signal 0x%x {%d}\n", sig, sig);
        fflush(fp);
        backtrace_symbols_fd(array, size, fd);
    }
    // ... and stderr
    fprintf(stderr, "Error: Server crashed with signal 0x%x {%d}\n", sig, sig);
    backtrace_symbols_fd(array, size, STDERR_FILENO);
    exit(1);
}

void *custom_Sys_LoadDll(const char *name, char *fqpath, int (**entryPoint)(int, ...), int (*systemcalls)(int, ...))
{
    hook_sys_loaddll->unhook();
    void *(*Sys_LoadDll)(const char *name, char *fqpath, int (**entryPoint)(int, ...), int (*systemcalls)(int, ...));
    *(int *)&Sys_LoadDll = hook_sys_loaddll->from;
    void *ret = Sys_LoadDll(name, fqpath, entryPoint, systemcalls);
    hook_sys_loaddll->hook();

    // Unprotect the game lib
    char libPath[512];
    cvar_t* fs_game = Cvar_FindVar("fs_game");
    if (*fs_game->string)
        sprintf(libPath, "%s/game.mp.i386.so", fs_game->string);
    else
        sprintf(libPath, "main/game.mp.i386.so");
    char buf[512];
    char flags[4];
    void *low, *high;
    FILE *fp;
    fp = fopen("/proc/self/maps", "r");
    if (!fp)
        return 0;
    while (fgets(buf, sizeof(buf), fp))
    {
        if (!strstr(buf, libPath))
            continue;
        if (sscanf (buf, "%p-%p %4c", &low, &high, flags) != 3)
            continue;
        mprotect((void *)low, (int)high-(int)low, PROT_READ | PROT_WRITE | PROT_EXEC);
    }
    fclose(fp);
    
    g_entities = (gentity_t*)dlsym(ret, "g_entities");
    pm = (pmove_t*)dlsym(ret, "pm");

    Scr_GetFunctionHandle = (Scr_GetFunctionHandle_t)dlsym(ret, "Scr_GetFunctionHandle");
    Scr_GetNumParam = (Scr_GetNumParam_t)dlsym(ret, "Scr_GetNumParam");
    SV_Cmd_ArgvBuffer = (SV_Cmd_ArgvBuffer_t)dlsym(ret, "trap_Argv");
    ClientCommand = (ClientCommand_t)dlsym(ret, "ClientCommand");
    Scr_GetFunction = (Scr_GetFunction_t)dlsym(ret, "Scr_GetFunction");
    Scr_GetMethod = (Scr_GetMethod_t)dlsym(ret, "Scr_GetMethod");
    SV_GameSendServerCommand = (SV_GameSendServerCommand_t)dlsym(ret, "trap_SendServerCommand");
    Scr_ExecThread = (Scr_ExecThread_t)dlsym(ret, "Scr_ExecThread");
    Scr_ExecEntThread = (Scr_ExecEntThread_t)dlsym(ret, "Scr_ExecEntThread");
    Scr_ExecEntThreadNum = (Scr_ExecEntThreadNum_t)dlsym(ret, "Scr_ExecEntThreadNum");
    Scr_FreeThread = (Scr_FreeThread_t)dlsym(ret, "Scr_FreeThread");
    Scr_Error = (Scr_Error_t)dlsym(ret, "Scr_Error");
    G_Say = (G_Say_t)dlsym(ret, "G_Say");
    SV_GetConfigstringConst = (SV_GetConfigstringConst_t)dlsym(ret, "trap_GetConfigstringConst");
    SV_GetConfigstring = (SV_GetConfigstring_t)dlsym(ret, "trap_GetConfigstring");
#if COD_VERSION == COD1_1_5
    BG_GetInfoForWeapon = (BG_GetInfoForWeapon_t)dlsym(ret, "BG_GetInfoForWeapon");
#endif
    BG_GetWeaponIndexForName = (BG_GetWeaponIndexForName_t)dlsym(ret, "BG_GetWeaponIndexForName");
    Scr_IsSystemActive = (Scr_IsSystemActive_t)dlsym(ret, "Scr_IsSystemActive");
    Scr_GetInt = (Scr_GetInt_t)dlsym(ret, "Scr_GetInt");
    Scr_GetString = (Scr_GetString_t)dlsym(ret, "Scr_GetString");
    Scr_GetType = (Scr_GetType_t)dlsym(ret, "Scr_GetType");
    Scr_GetEntity = (Scr_GetEntity_t)dlsym(ret, "Scr_GetEntity");
    Scr_AddBool = (Scr_AddBool_t)dlsym(ret, "Scr_AddBool");
    Scr_AddInt = (Scr_AddInt_t)dlsym(ret, "Scr_AddInt");
    Scr_AddFloat = (Scr_AddFloat_t)dlsym(ret, "Scr_AddFloat");
    Scr_AddString = (Scr_AddString_t)dlsym(ret, "Scr_AddString");
    Scr_AddUndefined = (Scr_AddUndefined_t)dlsym(ret, "Scr_AddUndefined");
    Scr_AddVector = (Scr_AddVector_t)dlsym(ret, "Scr_AddVector");
    Scr_MakeArray = (Scr_MakeArray_t)dlsym(ret, "Scr_MakeArray");
    Scr_AddArray = (Scr_AddArray_t)dlsym(ret, "Scr_AddArray");
    Scr_AddObject = (Scr_AddObject_t)dlsym(ret, "Scr_AddObject");
    Scr_LoadScript = (Scr_LoadScript_t)dlsym(ret, "Scr_LoadScript");
    StuckInClient = (StuckInClient_t)dlsym(ret, "StuckInClient");
    Q_strlwr = (Q_strlwr_t)dlsym(ret, "Q_strlwr");
    Q_strupr = (Q_strupr_t)dlsym(ret, "Q_strupr");
    Q_strcat = (Q_strcat_t)dlsym(ret, "Q_strcat");

#if COD_VERSION == COD1_1_1
    hook_call((int)dlsym(ret, "vmMain") + 0xB0, (int)hook_ClientCommand);
    hook_call((int)dlsym(ret, "ClientEndFrame") + 0x311, (int)hook_StuckInClient);
#elif COD_VERSION == COD1_1_5
    hook_call((int)dlsym(ret, "vmMain") + 0xF0, (int)hook_ClientCommand);
#endif

#if COD_VERSION == COD1_1_5
    hook_jmp((int)dlsym(ret, "PM_GetEffectiveStance") + 0x16C1, (int)hook_PM_WalkMove_Naked);
    resume_addr_PM_WalkMove = (uintptr_t)dlsym(ret, "PM_GetEffectiveStance") + 0x18AA;
    hook_jmp((int)dlsym(ret, "PM_SlideMove") + 0xB6A, (int)hook_PM_SlideMove_Naked);
    resume_addr_PM_SlideMove = (uintptr_t)dlsym(ret, "PM_SlideMove") + 0xBA5;
    hook_jmp((int)dlsym(ret, "PM_GetEffectiveStance") + 0xAD, (int)custom_Jump_GetLandFactor);
    hook_jmp((int)dlsym(ret, "PM_GetEffectiveStance") + 0x4C, (int)custom_PM_GetReducedFriction);
#endif
    hook_jmp((int)dlsym(ret, "G_LocalizedStringIndex"), (int)custom_G_LocalizedStringIndex);

#if COD_VERSION == COD1_1_1
    // Patch codmsgboom
    /* See:
    - https://aluigi.altervista.org/adv/codmsgboom-adv.txt
    - https://github.com/xtnded/codextended/blob/855df4fb01d20f19091d18d46980b5fdfa95a712/src/librarymodule.c#L146
    */
    *(int*)((int)dlsym(ret, "G_Say") + 0x50e) = 0x37f;
    *(int*)((int)dlsym(ret, "G_Say") + 0x5ca) = 0x37f;

    // 1.1 deadchat support
    /* See:
    - https://github.com/xtnded/codextended/blob/855df4fb01d20f19091d18d46980b5fdfa95a712/src/librarymodule.c#L161
    */
    *(byte*)((int)dlsym(ret, "G_Say") + 0x2B3) = 0xeb;
    *(byte*)((int)dlsym(ret, "G_Say") + 0x3B6) = 0xeb;
    hook_call((int)dlsym(ret, "G_Say") + 0x5EA, (int)hook_G_Say);
    hook_call((int)dlsym(ret, "G_Say") + 0x77D, (int)hook_G_Say);
    hook_call((int)dlsym(ret, "G_Say") + 0x791, (int)hook_G_Say);
#endif

    hook_gametype_scripts = new cHook((int)dlsym(ret, "GScr_LoadGameTypeScript"), (int)custom_GScr_LoadGameTypeScript);
    hook_gametype_scripts->hook();
    
    return ret;
}

class cCallOfDuty1Pro
{
public:
    cCallOfDuty1Pro()
    {
        // Don't inherit lib of parent
        unsetenv("LD_PRELOAD");

        // Crash handlers for debugging
        signal(SIGSEGV, ServerCrash);
        signal(SIGABRT, ServerCrash);
        
        // Otherwise the printf()'s are printed at crash/end on older os/compiler versions
        setbuf(stdout, NULL);

#if COD_VERSION == COD1_1_1
        printf("> [LIBCOD] Compiled for: CoD1 1.1\n");
#elif COD_VERSION == COD1_1_5
        printf("> [LIBCOD] Compiled for: CoD1 1.5\n");
#endif

        printf("> [LIBCOD] Compiled %s %s using GCC %s\n", __DATE__, __TIME__, __VERSION__);

        // Allow to write in executable memory
        mprotect((void *)0x08048000, 0x135000, PROT_READ | PROT_WRITE | PROT_EXEC);

#if COD_VERSION == COD1_1_1
        hook_call(0x08085213, (int)hook_AuthorizeState);
        hook_call(0x08094c54, (int)Scr_GetCustomFunction);
        hook_call(0x080951c4, (int)Scr_GetCustomMethod);
        hook_call(0x0808c780, (int)hook_SV_GetChallenge);
        hook_call(0x0808c7b8, (int)hook_SV_DirectConnect);
        hook_call(0x0808c7ea, (int)hook_SV_AuthorizeIpPacket);
        hook_call(0x0808c74e, (int)hook_SVC_Info);
        hook_call(0x0808c71c, (int)hook_SVC_Status);
        hook_call(0x0808c81d, (int)hook_SVC_RemoteCommand);

        hook_jmp(0x080717a4, (int)custom_FS_ReferencedPakChecksums);
        hook_jmp(0x080716cc, (int)custom_FS_ReferencedPakNames);

        // Patch q3infoboom
        /* See:
        - https://aluigi.altervista.org/adv/q3infoboom-adv.txt
        - https://github.com/xtnded/codextended/blob/855df4fb01d20f19091d18d46980b5fdfa95a712/src/codextended.c#L295
        */
        *(byte*)0x807f459 = 1;

        // Patch RCON half-second limit //TODO: Do like zk_libcod instead
        /* See:
        - https://aluigi.altervista.org/patches/q3rconz.lpatch
        - https://github.com/xtnded/codextended/blob/855df4fb01d20f19091d18d46980b5fdfa95a712/src/codextended.c#L291
        - https://github.com/ibuddieat/zk_libcod/blob/0f07cacf303d104a0375bf6235b8013e30b668ca/code/libcod.cpp#L3486
        */
        *(unsigned char*)0x808C41F = 0xeb;

        hook_sys_loaddll = new cHook(0x080c5fe4, (int)custom_Sys_LoadDll);
        hook_sys_loaddll->hook();
        hook_com_init = new cHook(0x0806c654, (int)custom_Com_Init);
        hook_com_init->hook();
        hook_sv_spawnserver = new cHook(0x0808a220, (int)custom_SV_SpawnServer);
        hook_sv_spawnserver->hook();
        hook_sv_begindownload_f = new cHook(0x08087a64, (int)custom_SV_BeginDownload_f);
        hook_sv_begindownload_f->hook();
#elif COD_VERSION == COD1_1_5
        hook_call(0x080894c5, (int)hook_AuthorizeState);
        hook_call(0x0809d8f5, (int)Scr_GetCustomFunction);
        hook_call(0x0809db31, (int)Scr_GetCustomMethod);
        hook_call(0x08093651, (int)hook_SV_GetChallenge);
        hook_call(0x0809370b, (int)hook_SV_DirectConnect);
        hook_call(0x0809374e, (int)hook_SV_AuthorizeIpPacket);
        hook_call(0x0809360e, (int)hook_SVC_Info);
        hook_call(0x080935cb, (int)hook_SVC_Status);
        hook_call(0x08093798, (int)hook_SVC_RemoteCommand);

        // Patch RCON half-second limit
        *(unsigned char*)0x080930e9 = 0xeb;

        hook_sys_loaddll = new cHook(0x080d3cdd, (int)custom_Sys_LoadDll);
        hook_sys_loaddll->hook();
        hook_cvar_set2 = new cHook(0x08072da8, (int)custom_Cvar_Set2);
        hook_cvar_set2->hook();
        hook_com_init = new cHook(0x08070ef8, (int)custom_Com_Init);
        hook_com_init->hook();
        hook_sv_spawnserver = new cHook(0x08090d0f, (int)custom_SV_SpawnServer);
        hook_sv_spawnserver->hook();
        hook_sv_begindownload_f = new cHook(0x0808b456, (int)custom_SV_BeginDownload_f);
        hook_sv_begindownload_f->hook();
        hook_sv_maprestart_f = new cHook(0x0808773a, (int)custom_SV_MapRestart_f);
        hook_sv_maprestart_f->hook();
#endif

        printf("> [PLUGIN LOADED]\n");
    }

    ~cCallOfDuty1Pro()
    {
        printf("> [PLUGIN UNLOADED]\n");
    }
};

cCallOfDuty1Pro *pro;
void __attribute__ ((constructor)) lib_load(void)
{
    pro = new cCallOfDuty1Pro;
}
void __attribute__ ((destructor)) lib_unload(void)
{
    delete pro;
}