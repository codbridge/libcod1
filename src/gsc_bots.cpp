#include "shared.hpp"

void gsc_bots_setuse(scr_entref_t ref)
{
    int id = ref.entnum;
    int use;

    if (!stackGetParams("i", &use))
    {
        stackError("gsc_bots_setuse() argument is undefined or has a wrong type");
        stackPushUndefined();
        return;
    }

    if (id >= MAX_CLIENTS)
    {
        stackError("gsc_bots_setuse() entity %i is not a player", id);
        stackPushUndefined();
        return;
    }

    client_t *client = &svs.clients[id];

    if (client->netchan.remoteAddress.type != NA_BOT)
    {
        stackError("gsc_bots_setuse() player %i is not a bot", id);
        stackPushUndefined();
        return;
    }

    if(!use)
        customPlayerState[id].botButtons &= ~BUTTON_USE;
    else
        customPlayerState[id].botButtons |= BUTTON_USE;

    stackPushBool(qtrue);
}

void gsc_bots_setbotstance(scr_entref_t ref)
{
    int id = ref.entnum;
    char *stance;

    if (!stackGetParams("s", &stance))
    {
        stackError("gsc_bots_setbotstance() argument is undefined or has a wrong type");
        stackPushUndefined();
        return;
    }

    if (id >= MAX_CLIENTS)
    {
        stackError("gsc_bots_setbotstance() entity %i is not a player", id);
        stackPushUndefined();
        return;
    }

    client_t *client = &svs.clients[id];

    if (client->netchan.remoteAddress.type != NA_BOT)
    {
        stackError("gsc_bots_setbotstance() player %i is not a bot", id);
        stackPushUndefined();
        return;
    }

    if (!strcmp(stance, "stand"))
    {
        customPlayerState[id].botUpMove &= ~(-KEY_JUMP | KEY_JUMP);
        customPlayerState[id].botWButtons &= ~(WBUTTON_CROUCH | WBUTTON_PRONE);
    }
    else if(!strcmp(stance, "crouch"))
        customPlayerState[id].botWButtons |= WBUTTON_CROUCH;
    else if(!strcmp(stance, "prone"))
        customPlayerState[id].botWButtons |= WBUTTON_PRONE;
    else if(!strcmp(stance, "jump"))
        customPlayerState[id].botUpMove |= KEY_JUMP;
    else
    {
        stackError("gsc_bots_setbotstance() invalid argument '%s'. Valid arguments are: 'stand' 'crouch' 'prone' 'jump'", stance);
        stackPushUndefined();
        return;
    }

    stackPushBool(qtrue);
}

void gsc_bots_meleeweapon(scr_entref_t ref)
{
    int id = ref.entnum;
    int melee;

    if (!stackGetParams("i", &melee))
    {
        stackError("gsc_bots_meleeweapon() argument is undefined or has a wrong type");
        stackPushUndefined();
        return;
    }

    if (id >= MAX_CLIENTS)
    {
        stackError("gsc_bots_meleeweapon() entity %i is not a player", id);
        stackPushUndefined();
        return;
    }

    client_t *client = &svs.clients[id];

    if (client->netchan.remoteAddress.type != NA_BOT)
    {
        stackError("gsc_bots_meleeweapon() player %i is not a bot", id);
        stackPushUndefined();
        return;
    }

    if(!melee)
        customPlayerState[id].botButtons &= ~BUTTON_MELEE;
    else
        customPlayerState[id].botButtons |= BUTTON_MELEE;

    stackPushBool(qtrue);
}

void gsc_bots_setaim(scr_entref_t ref)
{
    int id = ref.entnum;
    int ads;

    if (!stackGetParams("i", &ads))
    {
        stackError("gsc_bots_setaim() argument is undefined or has a wrong type");
        stackPushUndefined();
        return;
    }

    if (id >= MAX_CLIENTS)
    {
        stackError("gsc_bots_setaim() entity %i is not a player", id);
        stackPushUndefined();
        return;
    }

    client_t *client = &svs.clients[id];

    if (client->netchan.remoteAddress.type != NA_BOT)
    {
        stackError("gsc_bots_setaim() player %i is not a bot", id);
        stackPushUndefined();
        return;
    }

    if(!ads)
        customPlayerState[id].botButtons &= ~BUTTON_ZOOM;
    else
        customPlayerState[id].botButtons |= BUTTON_ZOOM;

    stackPushBool(qtrue);
}

void gsc_bots_fireweapon(scr_entref_t ref)
{
    int id = ref.entnum;
    int shoot;

    if (!stackGetParams("i", &shoot))
    {
        stackError("gsc_bots_fireweapon() argument is undefined or has a wrong type");
        stackPushUndefined();
        return;
    }

    if (id >= MAX_CLIENTS)
    {
        stackError("gsc_bots_fireweapon() entity %i is not a player", id);
        stackPushUndefined();
        return;
    }

    client_t *client = &svs.clients[id];

    if (client->netchan.remoteAddress.type != NA_BOT)
    {
        stackError("gsc_bots_fireweapon() player %i is not a bot", id);
        stackPushUndefined();
        return;
    }

    if(!shoot)
        customPlayerState[id].botButtons &= ~BUTTON_ATTACK;
    else
        customPlayerState[id].botButtons |= BUTTON_ATTACK;

    stackPushBool(qtrue);
}

void gsc_bots_setlean(scr_entref_t ref)
{
    int id = ref.entnum;
    char *lean;

    if (!stackGetParams("s", &lean))
    {
        stackError("gsc_bots_setlean() argument is undefined or has a wrong type");
        stackPushUndefined();
        return;
    }

    if (id >= MAX_CLIENTS)
    {
        stackError("gsc_bots_setlean() entity %i is not a player", id);
        stackPushUndefined();
        return;
    }

    client_t *client = &svs.clients[id];

    if (client->netchan.remoteAddress.type != NA_BOT)
    {
        stackError("gsc_bots_setlean() player %i is not a bot", id);
        stackPushUndefined();
        return;
    }

    if(!strcmp(lean, "none"))
        customPlayerState[id].botWButtons &= ~(WBUTTON_LEANLEFT | WBUTTON_LEANRIGHT);
    else if(!strcmp(lean, "left"))
        customPlayerState[id].botWButtons |= WBUTTON_LEANLEFT;
    else if(!strcmp(lean, "right"))
        customPlayerState[id].botWButtons |= WBUTTON_LEANRIGHT;
    else
    {
        stackError("gsc_bots_setlean() invalid argument '%s'. Valid arguments are: 'right' 'left'", lean);
        stackPushUndefined();
        return;
    }

    stackPushBool(qtrue);
}

void gsc_bots_reloadweapon(scr_entref_t ref)
{
    int id = ref.entnum;
    int reload;

    if (!stackGetParams("i", &reload))
    {
        stackError("gsc_bots_reloadweapon() argument is undefined or has a wrong type");
        stackPushUndefined();
        return;
    }

    if (id >= MAX_CLIENTS)
    {
        stackError("gsc_bots_reloadweapon() entity %i is not a player", id);
        stackPushUndefined();
        return;
    }

    client_t *client = &svs.clients[id];

    if (client->netchan.remoteAddress.type != NA_BOT)
    {
        stackError("gsc_bots_reloadweapon() player %i is not a bot", id);
        stackPushUndefined();
        return;
    }

    if(!reload)
        customPlayerState[id].botWButtons &= ~WBUTTON_RELOAD;
    else
        customPlayerState[id].botWButtons |= WBUTTON_RELOAD;

    stackPushBool(qtrue);
}

void gsc_bots_switchtoweaponid(scr_entref_t ref)
{
    int id = ref.entnum;
    int weaponid;

    if (!stackGetParams("i", &weaponid))
    {
        stackError("gsc_bots_switchtoweaponid() argument is undefined or has a wrong type");
        stackPushUndefined();
        return;
    }

    if (id >= MAX_CLIENTS)
    {
        stackError("gsc_bots_switchtoweaponid() entity %i is not a player", id);
        stackPushUndefined();
        return;
    }

    client_t *client = &svs.clients[id];

    if (client->netchan.remoteAddress.type != NA_BOT)
    {
        stackError("gsc_bots_switchtoweaponid() player %i is not a bot", id);
        stackPushUndefined();
        return;
    }

    customPlayerState[id].botWeapon = weaponid;

    stackPushBool(qtrue);
}

void gsc_bots_setwalkdir(scr_entref_t ref)
{
    int id = ref.entnum;
    char *dir;

    if (!stackGetParams("s", &dir))
    {
        stackError("gsc_bots_setwalkdir() argument is undefined or has a wrong type");
        stackPushUndefined();
        return;
    }

    if (id >= MAX_CLIENTS)
    {
        stackError("gsc_bots_setwalkdir() entity %i is not a player", id);
        stackPushUndefined();
        return;
    }

    client_t *client = &svs.clients[id];

    if (client->netchan.remoteAddress.type != NA_BOT)
    {
        stackError("gsc_bots_setwalkdir() player %i is not a bot", id);
        stackPushUndefined();
        return;
    }

    if (!strcmp(dir, "none"))
    {
        customPlayerState[id].botForwardMove = KEY_NONE;
        customPlayerState[id].botRightMove = KEY_NONE;
    }
    else if(!strcmp(dir, "forward"))
        customPlayerState[id].botForwardMove = KEY_FORWARD;
    else if(!strcmp(dir, "back"))
        customPlayerState[id].botForwardMove = KEY_BACK;
    else if(!strcmp(dir, "right"))
        customPlayerState[id].botRightMove = KEY_RIGHT;
    else if(!strcmp(dir, "left"))
        customPlayerState[id].botRightMove = KEY_LEFT;
    else
    {
        stackError("gsc_bots_setwalkdir() invalid argument '%s'. Valid arguments are: 'none' 'forward' 'back' 'right' 'left'", dir);
        stackPushUndefined();
        return;
    }

    stackPushBool(qtrue);
}

void gsc_bots_setwalkvalues(scr_entref_t ref)
{
    int id = ref.entnum;
    int fwcount;
    int rgcount;

    if (!stackGetParams("ii", &fwcount, &rgcount))
    {
        stackError("gsc_bots_setwalkvalues() arguments are undefined or have a wrong type");
        stackPushUndefined();
        return;
    }

    customPlayerState[id].botForwardMove = fwcount;
    customPlayerState[id].botRightMove = rgcount;

    stackPushBool(qtrue);
}