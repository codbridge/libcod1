#include "shared.hpp"

void gsc_utils_file_exists()
{
    char *filename;

    if (!stackGetParams("s", &filename))
    {
        stackError("gsc_utils_file_exists() argument is undefined or has a wrong type");
        stackPushUndefined();
        return;
    }

    int file_exists = access(filename, F_OK) != -1;
    stackPushInt(file_exists);
}

void gsc_utils_fopen()
{
    FILE *file;
    char *filename, *mode;

    if (!stackGetParams("ss", &filename, &mode))
    {
        stackError("gsc_utils_fopen() one or more arguments is undefined or has a wrong type");
        stackPushUndefined();
        return;
    }

    file = fopen(filename, mode);

    if (!file)
    {
        stackError("gsc_utils_fopen() returned an error");
        stackPushUndefined();
        return;
    }

    stackPushInt((int)file);
}

void gsc_utils_fread()
{
    FILE *file;

    if (!stackGetParams("i", &file))
    {
        stackError("gsc_utils_fread() argument is undefined or has a wrong type");
        stackPushUndefined();
        return;
    }

    if (!file)
    {
        stackError("gsc_utils_fread() returned an error");
        stackPushUndefined();
        return;
    }

    char buffer[256];
    int ret = fread(buffer, 1, 255, file);

    if (!ret)
    {
        stackPushUndefined();
        return;
    }

    buffer[ret] = '\0';
    stackPushString(buffer);
}

void gsc_utils_fwrite()
{
    FILE *file;
    char *buffer;

    if (!stackGetParams("is", &file, &buffer))
    {
        stackError("gsc_utils_fwrite() one or more arguments is undefined or has a wrong type");
        stackPushUndefined();
        return;
    }

    if (!file)
    {
        stackError("gsc_utils_fwrite() returned an error");
        stackPushUndefined();
        return;
    }

    stackPushInt(fwrite(buffer, 1, strlen(buffer), file));
}

void gsc_utils_fclose()
{
    FILE *file;

    if (!stackGetParams("i", &file))
    {
        stackError("gsc_utils_fclose() argument is undefined or has a wrong type");
        stackPushUndefined();
        return;
    }

    if (!file)
    {
        stackError("gsc_utils_fclose() returned an error");
        stackPushUndefined();
        return;
    }

    stackPushInt(fclose(file));
}