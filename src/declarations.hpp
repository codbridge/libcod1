#define qboolean int
#define qtrue   1
#define qfalse  0

#define PACKET_BACKUP 32
#define PACKET_MASK (PACKET_BACKUP - 1)

#define MAX_CLIENTS                 64
#define MAX_DOWNLOAD_WINDOW         8
#define MAX_INFO_STRING             0x400
#define MAX_MSGLEN                  0x4000
#define MAX_NETNAME                 36
#define MAX_OSPATH                  256
#define MAX_QPATH                   64
#define	MAX_NAME_LENGTH             32
#define MAX_RELIABLE_COMMANDS       64
#define MAX_STRINGLENGTH            1024

#define CVAR_NOFLAG         0               // 0x0000
#define CVAR_ARCHIVE        (1 << 0)        // 0x0001
#define CVAR_SERVERINFO     (1 << 2)        // 0x0004
#define CVAR_SYSTEMINFO     (1 << 3)        // 0x0008
#define CVAR_CHEAT          (1 << 9)        // 0x0200

#define KEY_MASK_NONE           0
#define KEY_MASK_FORWARD        127
#define KEY_MASK_BACK           -127
#define KEY_MASK_MOVERIGHT      127
#define KEY_MASK_MOVELEFT       -127
#define KEY_MASK_JUMP           127 // upmove. prone and jump = -KEY_MASK_JUMP
#define KEY_MASK_FIRE           0x1
#define KEY_MASK_RELOAD         0x8
#define KEY_MASK_LEANLEFT       0x10
#define KEY_MASK_ADS_MODE       0x10
#define KEY_MASK_LEANRIGHT      0x20
#define KEY_MASK_MELEE          0x20
#define KEY_MASK_USE            0x40
#define KEY_MASK_PRONE          0x40 // wbuttons
#define KEY_MASK_CROUCH         0x80 // wbuttons

#define EF_CROUCHING    0x20
#define EF_PRONE        0x40

typedef void (*xcommand_t)(void);

typedef unsigned char byte;
typedef struct gentity_s gentity_t;

typedef struct scr_entref_s
{
    uint16_t entnum;
    uint16_t classnum;
} scr_entref_t;

typedef enum
{
    CS_FREE,
    CS_ZOMBIE,
    CS_CONNECTED,
    CS_PRIMED,
    CS_ACTIVE
} clientConnectState_t;

typedef enum
{
    NA_BOT = 0,
    NA_BAD = 0,
    NA_LOOPBACK = 2,
    NA_BROADCAST = 3,
    NA_IP = 4,
    NA_IPX = 5,
    NA_BROADCAST_IPX = 6
} netadrtype_t;

typedef struct
{
    netadrtype_t type;
    byte ip[4];
    byte ipx[10];
    unsigned short port;
} netadr_t;

typedef enum
{
    NS_CLIENT,
    NS_SERVER
} netsrc_t;

typedef float vec_t;
typedef vec_t vec2_t[2];
typedef vec_t vec3_t[3];

typedef struct cvar_s
{
    char *name;
    char *string;
    char *resetString;
    char *latchedString;
    int flags;
    qboolean modified;
    int modificationCount;
    float value;
    int integer;
    struct cvar_s *next;
    struct cvar_s *hashNext;
} cvar_t;

union VariableUnion
{
    int intValue;
    float floatValue;
    unsigned int stringValue;
    const float *vectorValue;
    const char *codePosValue;
    unsigned int pointerValue;
    //...
};

typedef struct
{
    union VariableUnion u;
    int type;
} VariableValue;

typedef struct
{
    unsigned int *localVars;
    byte gap[356];
    VariableValue *top;
    //...
} scrVmPub_t; // TODO: verify

typedef int fileHandle_t;
typedef void *unzFile;
typedef void (*xfunction_t)();
typedef void (*xmethod_t)(scr_entref_t);

typedef struct scr_function_s
{
    const char      *name;
    xfunction_t     call;
    qboolean        developer;
} scr_function_t;

typedef struct scr_method_s
{
    const char     *name;
    xmethod_t      call;
    qboolean       developer;
} scr_method_t;

typedef struct usercmd_s
{
    int serverTime;
    byte buttons; // console, chat, ads, attack, use
    byte wbuttons; // lean left, lean right, reload
    byte weapon;
    byte flags;
    int angles[3];
    signed char forwardmove;
    signed char rightmove;
    signed char upmove;
    byte unknown;
} usercmd_t;

typedef void netProfileInfo_t;

typedef struct
{
    netsrc_t sock;
    int dropped;
    netadr_t remoteAddress;
    int qport;
    int incomingSequence;
    int outgoingSequence;
    int fragmentSequence;
    int fragmentLength;
    byte fragmentBuffer[MAX_MSGLEN];
    qboolean unsentFragments;
    int unsentFragmentStart;
    int unsentLength;
    byte unsentBuffer[MAX_MSGLEN];
    netProfileInfo_t *netProfile;
} netchan_t;

typedef struct
{
    char command[MAX_STRINGLENGTH];
    int cmdTime;
    int cmdType;
} reliableCommands_t;

typedef enum
{
    PM_NORMAL = 0x0,
    PM_NORMAL_LINKED = 0x1,
    PM_NOCLIP = 0x2,
    PM_UFO = 0x3,
    PM_SPECTATOR = 0x4,
    PM_INTERMISSION = 0x5,
    PM_DEAD = 0x6,
    PM_DEAD_LINKED = 0x7,
} pmtype_t;

typedef enum
{
    WEAPON_READY = 0x0,
    WEAPON_RAISING = 0x1,
    WEAPON_DROPPING = 0x2,
    WEAPON_FIRING = 0x3,
    WEAPON_RECHAMBERING = 0x4,
    WEAPON_RELOADING = 0x5,
    WEAPON_RELOADING_INTERUPT = 0x6,
    WEAPON_RELOAD_START = 0x7,
    WEAPON_RELOAD_START_INTERUPT = 0x8,
    WEAPON_RELOAD_END = 0x9,
    WEAPON_MELEE_INIT = 0xA,
    WEAPON_MELEE_FIRE = 0xB,
    WEAPONSTATES_NUM = 0xC,
} weaponstate_t;

typedef struct playerState_s
{
    int commandTime;
    pmtype_t pm_type;
    int bobCycle;
    int pm_flags;
    int pm_time;
    vec3_t origin;
    vec3_t velocity;
    vec2_t oldVelocity;
    int weaponTime;
    int weaponDelay;
    int gravity;
    float leanf;
    int speed;
    vec3_t delta_angles;
    int groundEntityNum;
    vec3_t vLadderVec;
    int jumpTime;
    float jumpOriginZ;
    int legsTimer;
    int legsAnim;
    int torsoTimer;
    int torsoAnim;
    int movementDir;
    int eFlags;
    int eventSequence;
    int events[4];
    unsigned int eventParms[4];
    int oldEventSequence;
    int clientNum;
    unsigned int weapon;
    weaponstate_t weaponstate;
    float fWeaponPosFrac;
    int viewmodelIndex;
    vec3_t viewangles;
    int viewHeightTarget;
    float viewHeightCurrent;
    byte gap[8184];
} playerState_t;

struct gentity_s
{
    //...
};

typedef struct
{
    playerState_t ps;
    int num_entities;
    int num_clients;
    int first_entity;
    int first_client;
    unsigned int messageSent;
    unsigned int messageAcked;
    int messageSize;
} clientSnapshot_t;

typedef struct client_s
{
    clientConnectState_t state;
    qboolean sendAsActive;
    const char *dropReason;
    char userinfo[MAX_INFO_STRING];
    reliableCommands_t reliableCommands[MAX_RELIABLE_COMMANDS];
    int reliableSequence;
    int reliableAcknowledge;
    int reliableSent;
    int messageAcknowledge;
    int gamestateMessageNum;
    int challenge;
    usercmd_t lastUsercmd;
    int lastClientCommand;
    char lastClientCommandString[MAX_STRINGLENGTH];
    gentity_t *gentity;
    char name[MAX_NAME_LENGTH];
    char downloadName[MAX_QPATH];
    fileHandle_t download;
    int downloadSize;
    int downloadCount;
    int downloadClientBlock;
    int downloadCurrentBlock;
    int downloadXmitBlock;
    unsigned char *downloadBlocks[MAX_DOWNLOAD_WINDOW];
    int downloadBlockSize[MAX_DOWNLOAD_WINDOW];
    qboolean downloadEOF;
    int downloadSendTime;
    char downloadURL[MAX_OSPATH];
    qboolean wwwOk;
    qboolean downloadingWWW;
    qboolean clientDownloadingWWW;
    qboolean wwwFallback;
    int deltaMessage;
    int nextReliableTime;
    int lastPacketTime;
    int lastConnectTime;
    int nextSnapshotTime;
    qboolean rateDelayed;
    int timeoutCount;
    clientSnapshot_t frames[PACKET_BACKUP];
    int ping;
    int rate;
    int snapshotMsec;
    int pureAuthentic;
    netchan_t netchan;
    int guid;
    unsigned short clscriptid;
    int bIsTestClient;
    int serverId;
    char PBguid[33];
} client_t;

typedef struct
{
    qboolean initialized;
    int time;
    int snapFlagServerBit;
    byte gap[2];
    client_t *clients;
    //...
} serverStatic_t;

static const int svs_offset = 0x083ccd80;
static const int vmpub_offset = 0x0830acc0;

#define scrVmPub (*((scrVmPub_t*)(vmpub_offset)))
#define svs (*((serverStatic_t*)(svs_offset)))

// Require structure sizes to match
#if __GNUC__ >= 6
static_assert((sizeof(netchan_t) == 32832), "ERROR: netchan_t size is invalid");
static_assert((sizeof(client_t) == 371124), "ERROR: client_t size is invalid");
static_assert((sizeof(playerState_t) == 8396), "ERROR: playerState_t size is invalid");
static_assert((sizeof(usercmd_t) == 24), "ERROR: usercmd_t size is invalid");
static_assert((sizeof(netadr_t) == 20), "ERROR: netadr_t size is invalid");
#endif