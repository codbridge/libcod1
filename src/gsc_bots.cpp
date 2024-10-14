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