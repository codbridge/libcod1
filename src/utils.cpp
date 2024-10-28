#include "shared.hpp"

int getProtocolFromShortVersion(const char *shortVersion)
{
    if(!strcmp(shortVersion, "1.1"))
        return 1;
    else if(!strcmp(shortVersion, "1.5"))
        return 6;
    else
        return -1;
}

const char * getShortVersionFromProtocol(int protocol)
{
    switch (protocol)
    {
        case 1: return "1.1";
        case 6: return "1.5";
        default: return nullptr;
    }
}