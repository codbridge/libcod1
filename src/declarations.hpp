#define qboolean int
#define qtrue   1
#define qfalse  0

#define VectorCopy(a, b) ((b)[0] = (a)[0],(b)[1] = (a)[1],(b)[2] = (a)[2])

#define FLOAT_INT_BITS  13
#define FLOAT_INT_BIAS  (1 << (FLOAT_INT_BITS - 1)) // 0x1000
#define PACKET_BACKUP   32
#define PACKET_MASK     (PACKET_BACKUP - 1)

#define MAX_CHALLENGES              1024
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
#define MAX_WEAPONS                 64
#define MAX_OBJECTIVES              16
#define MAX_HUDELEMENTS             31
#define MAX_HUDELEMS_ARCHIVAL       MAX_HUDELEMENTS
#define MAX_HUDELEMS_CURRENT        MAX_HUDELEMENTS

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
typedef struct gclient_s gclient_t;
typedef struct gentity_s gentity_t;

struct vm_t;

typedef struct scr_entref_s
{
    uint16_t entnum;
    uint16_t classnum;
} scr_entref_t;

enum svc_ops_e
{
    svc_bad,
    svc_nop,
    svc_gamestate,
    svc_configstring,
    svc_baseline,
    svc_serverCommand,
    svc_download,
    svc_snapshot,
    svc_EOF
};

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
    STATE_PLAYING,
    STATE_DEAD,
    STATE_SPECTATOR,
    STATE_INTERMISSION
} sessionState_t;

typedef enum
{
    CON_DISCONNECTED,
    CON_CONNECTING,
    CON_CONNECTED
} clientConnected_t;

typedef enum
{
    NA_BOT = 0,
    NA_BAD = 1,
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

typedef struct
{
    qboolean overflowed;
    byte *data;
    int maxsize;
    int cursize;
    int readcount;
    int bit;
} msg_t;

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

typedef struct netField_s
{
    char *name;
    int offset;
    int bits;
} netField_t;

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

typedef struct objective_s
{
    int state;
    vec3_t origin;
    int entNum;
    int teamNum;
    int icon;
} objective_t;

typedef struct hudelem_s
{
    byte gap[112];
} hudelem_t;

typedef struct hudElemState_s
{
    hudelem_t current[31];
    hudelem_t archival[31];
} hudElemState_t;

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
    //...
} weaponstate_t;

typedef struct playerState_s
{
    int commandTime;        // 0x0
    pmtype_t pm_type;       // 0x4
    int bobCycle;           // 0x8
    int pm_flags;           // 0xC
    int pm_time;            // 0x10
    vec3_t origin;          // [0] = 0x14, [1] = 0x18, [2] = 0x1C
    vec3_t velocity;        // [0] = 0x20, [1] = 0x24, [2] = 0x28
    vec2_t oldVelocity;     // [0] = 0x2C, [1] = 0x30
    int weaponTime;         // 0x34
    int weaponDelay;        // 0x38
    int gravity;            // 0x3C
    float leanf;            // 0x40
    int speed;              // 0x44
    vec3_t delta_angles;    // [0] = 0x48, [1] = 0x4C, [2] = 0x50
    int groundEntityNum;    // 0x54
    vec3_t vLadderVec;      // [0] = 0x58, [1] = 0x5C, [2] = 0x60
    int jumpTime;           // 0x64
    float jumpOriginZ;      // 0x68
    int legsTimer;          // 0x6C
    int legsAnim;           // 0x70
    int torsoTimer;         // 0x74
    int torsoAnim;          // 0x78
    int movementDir;        // 0x7C
    int eFlags;             // 0x80
    int eventSequence;      // 0x84
    int events[4];          // 0x88
    unsigned int eventParms[4]; // 0x98
    int oldEventSequence;       // 0xA8
    int clientNum;              // 0xAC
    unsigned int weapon;        // 0xB0
    weaponstate_t weaponstate;  // 0xB4
    float fWeaponPosFrac;       // 0xB8
    int viewmodelIndex;         // 0xBC
    vec3_t viewangles;          // 0xC0
    int viewHeightTarget;       // 0xCC
    float viewHeightCurrent;    // 0xD0
    int viewHeightLerpTime;     // 0xD4
    int viewHeightLerpTarget;   // 0xD8
    int viewHeightLerpDown;     // 0xDC
    float viewHeightLerpPosAdj; // 0xE0
    int damageEvent;            // 0xe4
    int damageYaw;              // 0xe8
    int damagePitch;            // 0xec
    int damageCount;            // 0xf0
    int stats[6];               // 0xf4
    int ammo[MAX_WEAPONS];      // 0x10c
    int ammoclip[MAX_WEAPONS];  // 0x20c
    unsigned int weapons[MAX_WEAPONS / (sizeof(int) * 8)];            // 0x30c
    byte weaponslots[8];        // 0x314
    unsigned int weaponrechamber[MAX_WEAPONS / (sizeof(int) * 8)];    // 0x31c
    vec3_t mins;                // 0x324
    vec3_t maxs;                // 0x330
    byte gap_0x33C[0x2C];             // 0x33C
    float proneDirection;       // 0x368
    float proneDirectionPitch;  // 0x36c
    float proneTorsoPitch;      // 0x370
    int viewlocked;             // 0x374
    int viewlocked_entNum;      // 0x378
    byte gap_0x37C[8];          // 0x37C
    int cursorHint;             // 0x384
    byte gap_0x388[4];          // 0x388
    int cursorHintString;       // 0x38c
    byte gap_0x390[0x28];       // 0x390
    int cursorHintEntIndex;     // 0x3b8
    byte gap_0x3BC[4];          // 0x3BC
    int iCompassFriendInfo;     // 0x3C0
    float fTorsoHeight;         // 0x3c4
    float fTorsoPitch;          // 0x3c8
    float fWaistPitch;          // 0x3cc
    int entityEventSequence;    // 0x3D0
    int weapAnim;               // 0x3d4
    float aimSpreadScale;       // 0x3d8
    int shellshockIndex;        // 0x3dc
    int shellshockTime;         // 0x3e0
    int shellshockDuration;     // 0x3e4
    objective_t objective[MAX_OBJECTIVES];  // 0x3E8
    hudElemState_t hud;         // 0x5A8
    int deltaTime;              // 0x20C8
} playerState_t;

typedef struct
{
    sessionState_t sessionState;
    int forceSpectatorClient;
    int statusIcon;
    int archiveTime;
    int score;
    int deaths;
    byte gap[4];
    clientConnected_t connected;
    usercmd_t cmd;
    //...
} clientSession_t;

struct gclient_s
{
    playerState_t ps;
    clientSession_t sess;
    //...
};

struct gentity_s
{
    byte gap[348];
    struct gclient_s *client;
    byte gap2[444];
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
    netadr_t adr;
    int challenge;
    int time;
    int pingTime;
    int firstTime;
    int firstPing;
    qboolean connected;
    int guid;
    char PBguid[33];
} challenge_t;

typedef struct
{
    qboolean initialized;
    int time;
    int snapFlagServerBit;
    byte gap[2];
    client_t *clients;
    byte gap2[76];
    challenge_t challenges[MAX_CHALLENGES];
    //...
} serverStatic_t;

extern gentity_t *g_entities;

static const int svs_offset = 0x083ccd80;
static const int vmpub_offset = 0x0830acc0;
static const int gvm_offset = 0x080ee804;
static const int playerStateFields_offset = 0x080dc560;
static const int objectiveFields_offset = 0x080e9a80;

#define scrVmPub (*((scrVmPub_t*)(vmpub_offset)))
#define svs (*((serverStatic_t*)(svs_offset)))
#define gvm (*(vm_t**)(gvm_offset))
#define playerStateFields (*((netField_t*)(playerStateFields_offset)))
#define objectiveFields (*((netField_t*)(objectiveFields_offset)))

// Require structure sizes to match
#if __GNUC__ >= 6
static_assert((sizeof(netchan_t) == 32832), "ERROR: netchan_t size is invalid");
static_assert((sizeof(client_t) == 371124), "ERROR: client_t size is invalid");
static_assert((sizeof(playerState_t) == 8396), "ERROR: playerState_t size is invalid");
static_assert((sizeof(usercmd_t) == 24), "ERROR: usercmd_t size is invalid");
static_assert((sizeof(netadr_t) == 20), "ERROR: netadr_t size is invalid");
static_assert((sizeof(gentity_t) == 796), "ERROR: gentity_t size is invalid!");
static_assert((sizeof(hudElemState_t) == 6944), "ERROR: hudElemState_t size is invalid!");
static_assert((sizeof(objective_t) == 28), "ERROR: objective_t size is invalid!");
#endif


//// Custom
typedef struct callback_s
{
    int *pos;
    const char *name;
    bool custom;
} callback_t;

typedef struct customPlayerState_s
{
    //// Bot
    int botButtons;
    ////
    int protocolVersion;
} customPlayerState_t;
////