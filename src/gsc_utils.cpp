#include "shared.hpp"

void gsc_utils_strtok() // From cod2rev
{
    signed int j;
    char c;
    int dest;
    int i;
    signed int len;
    const char *tok;
    const char *delim;
    char tempString[1024];

    delim = Scr_GetString(0);
    tok = Scr_GetString(1u);
    len = strlen(tok);
    dest = 0;

    Scr_MakeArray();

    for (i = 0; ; ++i)
    {
        c = delim[i];

        if (!c)
            break;

        for (j = 0; j < len; ++j)
        {
            if (c == tok[j])
            {
                if (dest)
                {
                    tempString[dest] = 0;
                    Scr_AddString(tempString);
                    Scr_AddArray();
                    dest = 0;
                }

                goto skip;
            }
        }

        tempString[dest] = c;

        if (++dest > 1023)
            stackError("gsc_utils_strtok() string too long");
skip:
        ;
    }

    if (dest)
    {
        tempString[dest] = 0;
        Scr_AddString(tempString);
        Scr_AddArray();
    }
}