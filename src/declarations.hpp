#define qboolean int
#define qtrue   1
#define qfalse  0

// 3D vectors
#define DotProduct(a,b)         ((a)[0]*(b)[0]+(a)[1]*(b)[1]+(a)[2]*(b)[2])
#define VectorCopy(a,b)         ((b)[0]=(a)[0],(b)[1]=(a)[1],(b)[2]=(a)[2])
#define	VectorScale(v, s, o)    ((o)[0]=(v)[0]*(s),(o)[1]=(v)[1]*(s),(o)[2]=(v)[2]*(s))

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

#define OVERCLIP        1.001

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
    //...
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

typedef enum
{
    EV_NONE,
    EV_FOOTSTEP_RUN_DEFAULT,
    EV_FOOTSTEP_RUN_BARK,
    EV_FOOTSTEP_RUN_BRICK,
    EV_FOOTSTEP_RUN_CARPET,
    EV_FOOTSTEP_RUN_CLOTH,
    EV_FOOTSTEP_RUN_CONCRETE,
    EV_FOOTSTEP_RUN_DIRT,
    EV_FOOTSTEP_RUN_FLESH,
    EV_FOOTSTEP_RUN_FOLIAGE,
    EV_FOOTSTEP_RUN_GLASS,
    EV_FOOTSTEP_RUN_GRASS,
    EV_FOOTSTEP_RUN_GRAVEL,
    EV_FOOTSTEP_RUN_ICE,
    EV_FOOTSTEP_RUN_METAL,
    EV_FOOTSTEP_RUN_MUD,
    EV_FOOTSTEP_RUN_PAPER,
    EV_FOOTSTEP_RUN_PLASTER,
    EV_FOOTSTEP_RUN_ROCK,
    EV_FOOTSTEP_RUN_SAND,
    EV_FOOTSTEP_RUN_SNOW,
    EV_FOOTSTEP_RUN_WATER,
    EV_FOOTSTEP_RUN_WOOD,
    EV_FOOTSTEP_RUN_ASPHALT,
    EV_FOOTSTEP_WALK_DEFAULT,
    EV_FOOTSTEP_WALK_BARK,
    EV_FOOTSTEP_WALK_BRICK,
    EV_FOOTSTEP_WALK_CARPET,
    EV_FOOTSTEP_WALK_CLOTH,
    EV_FOOTSTEP_WALK_CONCRETE,
    EV_FOOTSTEP_WALK_DIRT,
    EV_FOOTSTEP_WALK_FLESH,
    EV_FOOTSTEP_WALK_FOLIAGE,
    EV_FOOTSTEP_WALK_GLASS,
    EV_FOOTSTEP_WALK_GRASS,
    EV_FOOTSTEP_WALK_GRAVEL,
    EV_FOOTSTEP_WALK_ICE,
    EV_FOOTSTEP_WALK_METAL,
    EV_FOOTSTEP_WALK_MUD,
    EV_FOOTSTEP_WALK_PAPER,
    EV_FOOTSTEP_WALK_PLASTER,
    EV_FOOTSTEP_WALK_ROCK,
    EV_FOOTSTEP_WALK_SAND,
    EV_FOOTSTEP_WALK_SNOW,
    EV_FOOTSTEP_WALK_WATER,
    EV_FOOTSTEP_WALK_WOOD,
    EV_FOOTSTEP_WALK_ASPHALT,
    EV_FOOTSTEP_PRONE_DEFAULT,
    EV_FOOTSTEP_PRONE_BARK,
    EV_FOOTSTEP_PRONE_BRICK,
    EV_FOOTSTEP_PRONE_CARPET,
    EV_FOOTSTEP_PRONE_CLOTH,
    EV_FOOTSTEP_PRONE_CONCRETE,
    EV_FOOTSTEP_PRONE_DIRT,
    EV_FOOTSTEP_PRONE_FLESH,
    EV_FOOTSTEP_PRONE_FOLIAGE,
    EV_FOOTSTEP_PRONE_GLASS,
    EV_FOOTSTEP_PRONE_GRASS,
    EV_FOOTSTEP_PRONE_GRAVEL,
    EV_FOOTSTEP_PRONE_ICE,
    EV_FOOTSTEP_PRONE_METAL,
    EV_FOOTSTEP_PRONE_MUD,
    EV_FOOTSTEP_PRONE_PAPER,
    EV_FOOTSTEP_PRONE_PLASTER,
    EV_FOOTSTEP_PRONE_ROCK,
    EV_FOOTSTEP_PRONE_SAND,
    EV_FOOTSTEP_PRONE_SNOW,
    EV_FOOTSTEP_PRONE_WATER,
    EV_FOOTSTEP_PRONE_WOOD,
    EV_FOOTSTEP_PRONE_ASPHALT,
    EV_JUMP_DEFAULT,
    EV_JUMP_BARK,
    EV_JUMP_BRICK,
    EV_JUMP_CARPET,
    EV_JUMP_CLOTH,
    EV_JUMP_CONCRETE,
    EV_JUMP_DIRT,
    EV_JUMP_FLESH,
    EV_JUMP_FOLIAGE,
    EV_JUMP_GLASS,
    EV_JUMP_GRASS,
    EV_JUMP_GRAVEL,
    EV_JUMP_ICE,
    EV_JUMP_METAL,
    EV_JUMP_MUD,
    EV_JUMP_PAPER,
    EV_JUMP_PLASTER,
    EV_JUMP_ROCK,
    EV_JUMP_SAND,
    EV_JUMP_SNOW,
    EV_JUMP_WATER,
    EV_JUMP_WOOD,
    EV_JUMP_ASPHALT,
    EV_LANDING_DEFAULT,
    EV_LANDING_BARK,
    EV_LANDING_BRICK,
    EV_LANDING_CARPET,
    EV_LANDING_CLOTH,
    EV_LANDING_CONCRETE,
    EV_LANDING_DIRT,
    EV_LANDING_FLESH,
    EV_LANDING_FOLIAGE,
    EV_LANDING_GLASS,
    EV_LANDING_GRASS,
    EV_LANDING_GRAVEL,
    EV_LANDING_ICE,
    EV_LANDING_METAL,
    EV_LANDING_MUD,
    EV_LANDING_PAPER,
    EV_LANDING_PLASTER,
    EV_LANDING_ROCK,
    EV_LANDING_SAND,
    EV_LANDING_SNOW,
    EV_LANDING_WATER,
    EV_LANDING_WOOD,
    EV_LANDING_ASPHALT,
    EV_LANDING_PAIN_DEFAULT,
    EV_LANDING_PAIN_BARK,
    EV_LANDING_PAIN_BRICK,
    EV_LANDING_PAIN_CARPET,
    EV_LANDING_PAIN_CLOTH,
    EV_LANDING_PAIN_CONCRETE,
    EV_LANDING_PAIN_DIRT,
    EV_LANDING_PAIN_FLESH,
    EV_LANDING_PAIN_FOLIAGE,
    EV_LANDING_PAIN_GLASS,
    EV_LANDING_PAIN_GRASS,
    EV_LANDING_PAIN_GRAVEL,
    EV_LANDING_PAIN_ICE,
    EV_LANDING_PAIN_METAL,
    EV_LANDING_PAIN_MUD,
    EV_LANDING_PAIN_PAPER,
    EV_LANDING_PAIN_PLASTER,
    EV_LANDING_PAIN_ROCK,
    EV_LANDING_PAIN_SAND,
    EV_LANDING_PAIN_SNOW,
    EV_LANDING_PAIN_WATER,
    EV_LANDING_PAIN_WOOD,
    EV_LANDING_PAIN_ASPHALT,
    EV_FOLIAGE_SOUND,
    EV_STANCE_FORCE_STAND,
    EV_STANCE_FORCE_CROUCH,
    EV_STANCE_FORCE_PRONE,
    EV_STEP_VIEW,
    EV_WATER_TOUCH,
    EV_WATER_LEAVE,
    EV_ITEM_PICKUP,
    EV_ITEM_PICKUP_QUIET,
    EV_AMMO_PICKUP,
    EV_NOAMMO,
    EV_EMPTYCLIP,
    EV_RELOAD,
    EV_RELOAD_FROM_EMPTY,
    EV_RELOAD_START,
    EV_RELOAD_END,
    EV_RAISE_WEAPON,
    EV_PUTAWAY_WEAPON,
    EV_WEAPON_ALT,
    EV_PULLBACK_WEAPON,
    EV_FIRE_WEAPON,
    EV_FIRE_WEAPONB,
    EV_FIRE_WEAPON_LASTSHOT,
    EV_RECHAMBER_WEAPON,
    EV_EJECT_BRASS,
    EV_MELEE_SWIPE,
    EV_FIRE_MELEE,
    EV_MELEE_HIT,
    EV_MELEE_MISS,
    EV_FIRE_WEAPON_MG42,
    EV_FIRE_QUADBARREL_1,
    EV_FIRE_QUADBARREL_2,
    EV_BULLET_TRACER,
    EV_SOUND_ALIAS,
    EV_BULLET_HIT_SMALL,
    EV_BULLET_HIT_LARGE,
    EV_BULLET_HIT_CLIENT_SMALL,
    EV_BULLET_HIT_CLIENT_LARGE,
    EV_GRENADE_BOUNCE,
    EV_GRENADE_EXPLODE,
    EV_ROCKET_EXPLODE,
    EV_ROCKET_EXPLODE_NOMARKS,
    EV_CUSTOM_EXPLODE,
    EV_CUSTOM_EXPLODE_NOMARKS,
    EV_RAILTRAIL,
    EV_BULLET,
    EV_PAIN,
    EV_CROUCH_PAIN,
    EV_DEATH,
    EV_DEBUG_LINE,
    EV_PLAY_FX,
    EV_PLAY_FX_DIR,
    EV_PLAY_FX_ON_TAG,
    EV_FLAMEBARREL_BOUNCE,
    EV_EARTHQUAKE,
    EV_DROPWEAPON,
    EV_ITEM_RESPAWN,
    EV_ITEM_POP,
    EV_PLAYER_TELEPORT_IN,
    EV_PLAYER_TELEPORT_OUT,
    EV_OBITUARY
} entity_event_t;

typedef struct netField_s
{
    char *name;
    int offset;
    int bits;
} netField_t;

typedef struct trace_s
{
    byte gap[0x10];     // pml + 0x38
    vec3_t normal;      // pml + 0x48
    int surfaceFlags;   // pml + 0x54
    byte gap_[16];      // pml + 0x58
} trace_t;

typedef struct usercmd_s
{
    int serverTime;
    byte buttons;   // console, chat, ads, attack, use
    byte wbuttons;  // lean left, lean right, reload
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
    int number;
    entityType_t eType;
    int eFlags;
    trajectory_t pos;
    trajectory_t apos;
    int time;
    int time2;
    vec3_t origin2;
    vec3_t angles2;
    int otherEntityNum;
    int attackerEntityNum;
    int groundEntityNum;
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
    unsigned int weapons[MAX_WEAPONS / (sizeof(int) * 8)]; // 0x30c
    byte weaponslots[8];        // 0x314
    unsigned int weaponrechamber[MAX_WEAPONS / (sizeof(int) * 8)]; // 0x31c
    vec3_t mins;                // 0x324
    vec3_t maxs;                // 0x330
    byte gap_0x33C[0x2C];       // 0x33C
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
    objective_t objective[MAX_OBJECTIVES]; // 0x3E8
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
    char PBguid[33];
} challenge_t;

typedef struct
{
    qboolean initialized;
    int time;
    int time2;
    int snapFlagServerBit;
    client_t *clients;
    byte gap_83CCD94[0x4C];
    challenge_t challenges[MAX_CHALLENGES];
    //...
} serverStatic_t;

typedef enum
{
    SS_DEAD,
    SS_LOADING,
    SS_GAME
} serverState_t;

typedef struct svEntity_s
{
    byte gap[8];
    archivedEntity_t baseline;
    int numClusters; // 0x118
    int clusternums[MAX_ENT_CLUSTERS];
    int lastCluster;
    byte gap2[24];
} svEntity_t;

typedef struct
{
    serverState_t state;
    qboolean restarting;
    int start_frameTime;
    int	checksumFeed;
    int timeResidual;
    byte gap_836B834[0x404];
    char *configstrings[MAX_CONFIGSTRINGS];
    svEntity_t svEntities[MAX_GENTITIES];
    char *entityParsePoint;
    gentity_t *gentities;
    int gentitySize;
    int	num_entities;
    playerState_t *gameClients;
    int gameClientSize;
    byte gap_83CCC50[0xBC];
    char gametype[MAX_QPATH];
} server_t;

typedef struct
{
    playerState_t *ps;  // 0x0
    usercmd_t cmd;      // 0x4
    usercmd_t oldcmd;   // 0x1C
    int tracemask;      // 0x34
    byte gap_38[4];
    int numtouch;       // 0x3C
    int touchents[32];  // 0x40
    vec3_t mins;        // 0xC0
    vec3_t maxs;        // 0xCC
    byte gap_D8[4];
    float xyspeed;      // 0xDC
    byte gap_E0[8];
    int proneChange;    // 0xE8
    byte gap_EC[16];
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

extern gentity_t *g_entities;

static const int sv_offset = 0x0836b820;
static const int svs_offset = 0x083ccd80;
static const int vmpub_offset = 0x0830acc0;
static const int gvm_offset = 0x080ee804;
static const int playerStateFields_offset = 0x080dc560;
static const int objectiveFields_offset = 0x080e9a80;

#define scrVmPub (*((scrVmPub_t*)(vmpub_offset)))
#define sv (*((server_t*)(sv_offset)))
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
static_assert((sizeof(trace_t) == 48), "ERROR: trace_t size is invalid!");
static_assert((sizeof(server_t) == 398636), "ERROR: server_t size is invalid");
static_assert((sizeof(entityState_t) == 240), "ERROR: entityState_t size is invalid!");
static_assert((sizeof(svEntity_t) == 380), "ERROR: svEntity_t size is invalid!");
static_assert((sizeof(archivedEntity_t) == 276), "ERROR: archivedEntity_t size is invalid!");
static_assert((sizeof(pmove_t) == 252), "ERROR: pmove_t size is invalid!");
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