#include "global.h"
#include "RageFile.h"
#include "RageLog.h"
#include "ver.h"

extern char const * const product_version;

// Logic to load version from VERSION.md using RageFile for VFS compatibility
static RString LoadVersion()
{
    RageFile file;
    if (!file.Open("VERSION.md", RageFile::READ))
    {
        LOG->Warn("Could not open VERSION.md, using fallback version.");
        return "5.7.0-Unified-Fallback";
    }

    RString sVersion;
    if (file.GetLine(sVersion) <= 0)
    {
        return "5.7.0-Unified-Unknown";
    }

    sVersion.Trim();
    return sVersion;
}

// We use a pointer that we initialize at startup to avoid order-of-initialization issues
// with static RStrings if this is accessed very early.
static char g_szProductVersion[128] = {0};

void InitProductVersion()
{
    RString v = LoadVersion();
    strncpy(g_szProductVersion, v.c_str(), sizeof(g_szProductVersion)-1);
}

char const * const product_version = g_szProductVersion;
