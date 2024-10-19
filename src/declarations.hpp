#define qboolean int
#define qtrue   1
#define qfalse  0

// 3D vectors
#define DotProduct(a, b)        ((a)[0] * (b)[0] + (a)[1] * (b)[1] + (a)[2] * (b)[2])
#define VectorCopy(a, b)        ((b)[0] = (a)[0], (b)[1] = (a)[1], (b)[2] = (a)[2])
#define VectorMA(v, s, b, o)    ((o)[0] = (v)[0] + (b)[0] * (s), (o)[1] = (v)[1] + (b)[1] * (s), (o)[2] = (v)[2] + (b)[2] * (s))
#define	VectorScale(v, s, o)    ((o)[0] = (v)[0] * (s), (o)[1] = (v)[1] * (s), (o)[2] = (v)[2] * (s))
#define VectorSet(v, x, y, z)   ((v)[0] = (x), (v)[1] = (y), (v)[2] = (z))

#define FLOAT_INT_BITS  13
#define FLOAT_INT_BIAS  (1 << (FLOAT_INT_BITS - 1)) // 0x1000
#define GENTITYNUM_BITS 10
#define PACKET_BACKUP   32
#define PACKET_MASK     (PACKET_BACKUP - 1)

#define MAX_CHALLENGES              1024
#define MAX_CLIENTS                 64
#define MAX_CONFIGSTRINGS           2048
#define MAX_DOWNLOAD_WINDOW         8
#define MAX_ENT_CLUSTERS            16
#define MAX_GENTITIES               (1 << GENTITYNUM_BITS) // 1024
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
#define MAX_CVAR_VALUE_STRING       256

#define CVAR_NOFLAG         0           // 0x0000
#define CVAR_ARCHIVE        (1 << 0)    // 0x0001
#define CVAR_SERVERINFO     (1 << 2)    // 0x0004
#define CVAR_SYSTEMINFO     (1 << 3)    // 0x0008
#define CVAR_CHEAT          (1 << 9)    // 0x0200

#define BUTTON_ATTACK       0x1
#define BUTTON_TALK         0x2
#define BUTTON_ZOOM         0x10
#define BUTTON_MELEE        0x20
#define BUTTON_USE          0x40

#define WBUTTON_RELOAD      0x8
#define WBUTTON_LEANLEFT    0x10
#define WBUTTON_LEANRIGHT   0x20
#define WBUTTON_PRONE       0x40
#define WBUTTON_CROUCH      0x80

#define MOVE_AXIS_MAX   127
#define KEY_FORWARD     MOVE_AXIS_MAX
#define KEY_BACK        -KEY_FORWARD
#define KEY_RIGHT       MOVE_AXIS_MAX
#define KEY_LEFT        -KEY_RIGHT
#define KEY_JUMP        MOVE_AXIS_MAX

#define EF_CROUCHING    0x20
#define EF_PRONE        0x40
#define EF_FIRING       0x400
#define EF_MG42_ACTIVE  0xC000
#define EF_TALK         0x40000

#define PMF_PRONE           0x1
#define PMF_CROUCH          0x2
#define PMF_JUMP_HELD       0x8
#define PMF_LADDER          0x10
#define PMF_ZOOMING         0x20
#define PMF_BACKWARDS_RUN   0x40
#define PMF_SLIDING         0x100
#define PMF_RESPAWNED       0x800 // until BUTTON_ATTACK released
#define PMF_JUMPING         0x2000
#define PMF_FROZEN          0x4000
//#define PMF_SPECTATOR       0x40000
//#define PMF_FOLLOW          0x80000
#define PMF_DISABLEWEAPON   0x100000

#define JUMP_LAND_SLOWDOWN_TIME 1800
#define OVERCLIP 1.001

#define STANCE_EFFECTIVE_STAND  0
#define STANCE_EFFECTIVE_PRONE  1
#define STANCE_EFFECTIVE_CROUCH 2

typedef float vec_t;
typedef vec_t vec2_t[2];
typedef vec_t vec3_t[3];

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
    ET_INVISIBLE = 7,
} entityType_t;

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
    TEAM_FREE = 0,
    TEAM_AXIS = 1,
    TEAM_ALLIES = 2,
    TEAM_NUM_TEAMS = 4,
} team_t;

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
    qboolean overflowed;    // 0x0
    byte *data;             // 0x4
    int maxsize;            // 0x8
    int cursize;            // 0xC
    int readcount;          // 0x10
    int bit;                // 0x14
} msg_t;

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

typedef int cvarHandle_t;

typedef struct
{
    cvarHandle_t handle;
    int modificationCount;
    float value;
    int integer;
    char string[MAX_CVAR_VALUE_STRING];
} vmCvar_t;

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
} scrVmPub_t; // TODO

typedef int fileHandle_t;
typedef void *unzFile;
typedef void (*xfunction_t)();
typedef void (*xmethod_t)(scr_entref_t);

typedef struct scr_function_s
{
    const char *name;
    xfunction_t call;
    qboolean developer;
} scr_function_t;

typedef struct scr_method_s
{
    const char *name;
    xmethod_t call;
    qboolean developer;
} scr_method_t;

typedef enum
{
    EV_STANCE_FORCE_STAND = 140,
    EV_STANCE_FORCE_CROUCH = 141,
    EV_STANCE_FORCE_PRONE = 142
    //...
} entity_event_t;

typedef struct netField_s
{
    char *name;
    int offset;
    int bits;
} netField_t;

typedef struct trace_s
{
    float fraction;     // 0x0
    vec3_t endpos;      // 0x4
    vec3_t normal;      // 0x10
    int surfaceFlags;   // 0x1C
    byte gap0x20[8];
    uint16_t entityNum; // 0x28
    uint16_t partName;  // 0x2A
    byte gap0x2C[2];
    byte allsolid;      // 0x2E
    byte startsolid;    // 0x2F
} trace_t;

typedef struct usercmd_s
{
    int serverTime; // 0x0  // pm + 4
    byte buttons;   // 0x4  // pm + 8   // console, chat, ads, attack, use
    byte wbuttons;  // 0x5  // pm + 9   // lean left, lean right, reload
    byte weapon;    // 0x6  // pm + 10
    byte gap_0x7;           // pm + 11
    int angles[3];  // 0x8  // pm + 12  [0] = 0x8, [1] = 0xC, [2] = 0x10
    signed char forwardmove;    // 0x14     // pm + 24
    signed char rightmove;      // 0x15     // pm + 25
    signed char upmove;         // 0x16     // pm + 26
    byte gap_0x17;                          // pm + 27
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
    //...
} trType_t;

typedef struct
{
    trType_t trType;
    int trTime;
    int trDuration;
    vec3_t trBase;
    vec3_t trDelta;
} trajectory_t;

typedef struct entityState_s
{
    int number;         // 0x0
    entityType_t eType; // 0x4
    int eFlags;         // 0x8
    trajectory_t pos;   // 0xC
    trajectory_t apos;  // 0x30
    int time;           // 0x54
    int time2;          // 0x58
    vec3_t origin2;     // 0x5c
    vec3_t angles2;     // 0x68
    int otherEntityNum; // 0x74
    int attackerEntityNum;  // 0x78
    int groundEntityNum;    // 0x7c
    int constantLight;
    int loopSound;
    int surfType;
    int index; // modelIndex
    int clientNum;
    int iHeadIcon;
    int iHeadIconTeam;
    int solid;
    int eventParm;
    int eventSequence;
    int events[4];
    unsigned int eventParms[4];
    int weapon;
    int legsAnim;
    int torsoAnim;
    int leanf;
    int scale; // used as loopfxid, hintstring, ... and doesn't actually scale a player's model size
    int dmgFlags;
    int animMovetype;
    float fTorsoHeight;
    float fTorsoPitch;
    float fWaistPitch;
} entityState_t;

typedef struct
{
    byte linked;        // 0x0  (ent + 240)
    byte gap_0x1[3];
    byte svFlags;       // 0x4  (... + 244)
    byte gap_0x5[0xF];  //      (... + 245)
    vec3_t mins;        // 0x14 (... + 260)
    vec3_t maxs;        // 0x20 (... + 272)
    byte gap_0x2C[0xC];
    int contents;       // 0x38 (... + 284)
    byte gap_0x3C[0x30];
} entityShared_t;

typedef struct objective_s
{
    int state;
    vec3_t origin;
    int entNum;
    int teamNum;
    int icon;
} objective_t;

enum hudelem_update_t
{
    HUDELEM_UPDATE_CURRENT = 0x2,
};

typedef struct hudelem_s
{
    byte gap[112];
} hudelem_t;

typedef struct hudElemState_s
{
    hudelem_t current[MAX_HUDELEMS_CURRENT];
    hudelem_t archival[MAX_HUDELEMS_ARCHIVAL];
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
    WEAPON_READY = 0x0,
    WEAPON_RAISING = 0x1,
    WEAPON_DROPPING = 0x2,
    WEAPON_FIRING = 0x3,
    WEAPON_RECHAMBERING = 0x4,
    WEAPON_RELOADING = 0x5,
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
    int weaponTime;         // 0x2c
    int weaponDelay;        // 0x30
    int grenadeTimeLeft;    // 0x34
    int iFoliageSoundTime;  // 0x38
    int gravity;            // 0x3C
    float leanf;            // 0x40
    int speed;              // 0x44
    vec3_t delta_angles;    // [0] = 0x48, [1] = 0x4C, [2] = 0x50
    int groundEntityNum;    // 0x54
    vec3_t vLadderVec;      // [0] = 0x58, [1] = 0x5C, [2] = 0x60
    int jumpTime;           // 0x64
    float fJumpOriginZ;     // 0x68
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
    int viewHeightLerpPosAdj;   // 0xE0
    int damageEvent;            // 0xe4
    int damageYaw;              // 0xe8
    int damagePitch;            // 0xec
    int damageCount;            // 0xf0
    int stats[6];               // 0xf4
    int ammo[MAX_WEAPONS];      // 0x10c
    int ammoclip[MAX_WEAPONS];  // 0x20c
    unsigned int weapons[2];    // 0x30c
    byte weaponslots[8];        // 0x314
    unsigned int weaponrechamber[2]; // 0x31c
    vec3_t mins;                // 0x324
    vec3_t maxs;                // 0x330
    int proneViewHeight;        // 0x33C
    int crouchViewHeight;       // 0x340
    int standViewHeight;        // 0x344
    int deadViewHeight;         // 0x348
    float walkSpeedScale;       // 0x34C // ADS
    float runSpeedScale;        // 0x350
    float proneSpeedScale;      // 0x354
    float crouchSpeedScale;     // 0x358
    float strafeSpeedScale;     // 0x35C
    float backSpeedScale;       // 0x360
    float leanSpeedScale;       // 0x364
    float proneDirection;       // 0x368
    float proneDirectionPitch;  // 0x36c
    float proneTorsoPitch;      // 0x370
    int viewlocked;             // 0x374
    int viewlocked_entNum;      // 0x378
    float friction;             // 0x37C
    int gunfx;                  // 0x380
    int serverCursorHint;       // 0x384
    int serverCursorHintVal;    // 0x388
    trace_t serverCursorHintTrace; // 0x38C
    byte gap_0x3BC[4];
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
    objective_t objective[MAX_OBJECTIVES]; // 0x3E8
    hudElemState_t hud;         // 0x5A8
    int deltaTime;              // 0x20C8
} playerState_t;

typedef struct
{
    int clientIndex;
    int team;
    byte gap_0x8[0x54];
} clientState_t;

typedef struct
{
    sessionState_t sessionState;    // 0x0
    int forceSpectatorClient;       // 0x4
    int statusIcon;                 // 0x8
    int archiveTime;                // 0xC
    int score;                      // 0x10
    int deaths;                     // 0x14
    byte gap_0x18[4];
    clientConnected_t connected;    // 0x1C
    usercmd_t cmd;                  // 0x20
    usercmd_t oldcmd;               // 0x38
    int localClient;                // 0x50
    int predictItemPickup;          // 0x54
    byte gap_0x58[0x28];
    int maxHealth;                  // 0x80
    byte gap_0x84[0x24];
    int noSpectate;                 // 0xA8
    byte gap_0xAC[0x8];
    clientState_t state;            // 0xB4
} clientSession_t;

struct gclient_s
{
    playerState_t ps;       // 0x0 // ent->client + 0x15C (ent[0x57])
    clientSession_t sess;   // 0x20CC
    int spectatorClient;    // 0x21DC
    qboolean noclip;        // 0x21E0
    qboolean ufo;           // 0x21E4
    qboolean bFrozen;       // 0x21E8
    byte gap_0x21F4[0xC];
    int latched_buttons;    // 0x21F8
    byte gap_0x21FC[0x8];
    int latched_wbuttons;   // 0x2204
    byte gap_0x2208[0xC];
    float fGunPitch;        // 0x2214
    float fGunYaw;          // 0x2218
    byte gap_0x221C[0x20];
    int inactivityTime;     // 0x223C
    byte gap_0x2240[0x8C];
};

struct gentity_s
{
    entityState_t s;        // 0x0
    entityShared_t r;       // 0xF0
    struct gclient_s *client;   // 0x15C
    byte gap_0x160[0x13];
    byte watertype;         // 0x173
    byte waterlevel;        // 0x174
    byte takedamage;        // 0x175
    byte gap_0x176[0x6];
    uint16_t classname;     // 0x17c
    byte gap_0x17E[0x6];
    int flags;              // 0x184
    byte gap_0x188[0x10];
    int clipmask;           // 0x198
    byte gap_0x19C[0x84];
    void (*die)(gentity_s *self, gentity_s *inflictor, gentity_s *attacker, int damage, int meansOfDeath, int iWeapon, const float *vDir, int hitLoc); // 0x220
    byte gap_0x224[0xF8];
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
    char PBGuid[33];
} client_t;

typedef struct
{
    int svFlags;
    int clientMask[2];
    vec3_t absmin;
    vec3_t absmax;
} archivedEntityShared_t;

typedef struct archivedEntity_s
{
    entityState_t s;
    archivedEntityShared_t r;
} archivedEntity_t;

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
    char PBGuid[33];
} challenge_t;

typedef struct
{
    qboolean initialized;
    int time;
    int time2;
    int snapFlagServerBit;
    client_t *clients;
    byte gap_0x83CCD94[0x4C];
    challenge_t challenges[MAX_CHALLENGES];
    //...
} serverStatic_t;

typedef struct
{
    gclient_t *clients;
    gentity_t *gentities;
    byte gap_0x8[0x1E0];
    int time;
    //...
} level_locals_t;

typedef enum
{
    SS_DEAD,
    SS_LOADING,
    SS_GAME
} serverState_t;

typedef struct svEntity_s
{
    byte gap_0x0[0x8];
    archivedEntity_t baseline;
    int numClusters; // 0x118
    int clusternums[MAX_ENT_CLUSTERS];
    int lastCluster;
    byte gap[0x18];
} svEntity_t;

typedef struct
{
    serverState_t state;
    qboolean restarting;
    int start_frameTime;
    int	checksumFeed;
    int timeResidual;
    byte gap_0x836B834[0x404];
    char *configstrings[MAX_CONFIGSTRINGS];
    svEntity_t svEntities[MAX_GENTITIES];
    char *entityParsePoint;
    gentity_t *gentities;
    int gentitySize;
    int	num_entities;
    playerState_t *gameClients;
    int gameClientSize;
    byte gap_0x83CCC50[0xBC];
    char gametype[MAX_QPATH];
} server_t;

typedef enum
{
    ANIM_BP_TORSO = 2,
    //...
} animBodyPart_t;

typedef struct
{
    playerState_t *ps;  // 0x0
    usercmd_t cmd;      // 0x4
    usercmd_t oldcmd;   // 0x1C
    int tracemask;      // 0x34
    int debugLevel;     // 0x38
    int numtouch;       // 0x3C
    int touchents[32];  // 0x40
    vec3_t mins;        // 0xC0
    vec3_t maxs;        // 0xCC
    byte watertype;     // 0xd8
    byte waterlevel;    // 0xd9
    byte gap_0xDA[2];
    float xyspeed;      // 0xDC
    int pmove_fixed;    // 0xE0
    int pmove_msec;     // 0xE4
    int proneChange;    // 0xE8
    void (*trace)(trace_t *, const vec3_t, const vec3_t, const vec3_t, const vec3_t, int, int);     // 0xec
    void (*trace2)(trace_t *, const vec3_t, const vec3_t, const vec3_t, const vec3_t, int, int);    // 0xF0
    void (*trace3)(trace_t *, const vec3_t, const vec3_t, const vec3_t, const vec3_t, int, int);    // 0xF4
    byte gap_0xF8[4];
} pmove_t;

struct pml_t
{
    vec3_t forward;     // 0x0
    vec3_t right;       // 0xC
    vec3_t up;          // 0x18
    float frametime;    // 0x24
    int msec;           // 0x28
    int walking;        // 0x2C
    int groundPlane;    // 0x30
    int almostGroundPlane;  // 0x34
    trace_t groundTrace;    // 0x38
    float impactSpeed;      // 0x68
    //...
};

typedef struct
{
    byte gap_0x0[0x30];
    unsigned short player;
    //...
} scr_const_t;

extern gentity_t *g_entities;

static const int sv_offset = 0x0836b820;
static const int svs_offset = 0x083ccd80;
static const int vmpub_offset = 0x0830acc0;
static const int gvm_offset = 0x080ee804;
static const int playerStateFields_offset = 0x080dc560;
static const int entityStateFields_offset = 0x080db860;
static const int objectiveFields_offset = 0x080e9a80;
static const int clientStateFields_offset = 0x080dbfe0;
static const int archivedEntityFields_offset = 0x080dbb80;

#define scrVmPub (*((scrVmPub_t*)(vmpub_offset)))
#define sv (*((server_t*)(sv_offset)))
#define svs (*((serverStatic_t*)(svs_offset)))
#define gvm (*(vm_t**)(gvm_offset))
#define playerStateFields (*((netField_t*)(playerStateFields_offset)))
#define entityStateFields (*((netField_t*)(entityStateFields_offset)))
#define objectiveFields (*((netField_t*)(objectiveFields_offset)))
#define clientStateFields (*((netField_t*)(clientStateFields_offset)))
#define archivedEntityFields (*((netField_t*)(archivedEntityFields_offset)))

// Require structure sizes to match
#if __GNUC__ >= 6
static_assert((sizeof(netchan_t) == 32832), "ERROR: netchan_t size invalid");
static_assert((sizeof(client_t) == 371124), "ERROR: client_t size invalid");
static_assert((sizeof(playerState_t) == 8396), "ERROR: playerState_t size invalid");
static_assert((sizeof(usercmd_t) == 24), "ERROR: usercmd_t size invalid");
static_assert((sizeof(netadr_t) == 20), "ERROR: netadr_t size invalid");
static_assert((sizeof(gentity_t) == 796), "ERROR: gentity_t size invalid");
static_assert((sizeof(hudElemState_t) == 6944), "ERROR: hudElemState_t size invalid");
static_assert((sizeof(objective_t) == 28), "ERROR: objective_t size invalid");
static_assert((sizeof(trace_t) == 48), "ERROR: trace_t size invalid");
static_assert((sizeof(server_t) == 398636), "ERROR: server_t size invalid");
static_assert((sizeof(entityState_t) == 240), "ERROR: entityState_t size invalid");
static_assert((sizeof(svEntity_t) == 380), "ERROR: svEntity_t size invalid");
static_assert((sizeof(archivedEntity_t) == 276), "ERROR: archivedEntity_t size invalid");
static_assert((sizeof(pmove_t) == 252), "ERROR: pmove_t size invalid");
static_assert((sizeof(gclient_t) == 8908), "ERROR: gclient_t size invalid");
static_assert((sizeof(clientSession_t) == 272), "ERROR: clientSession_t size invalid");
static_assert((sizeof(entityShared_t) == 108), "ERROR: entityShared_t size invalid");
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
    int protocol;
} customPlayerState_t;
////