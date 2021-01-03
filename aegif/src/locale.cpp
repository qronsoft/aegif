#include "locale.hpp"

// std
#include <cstring>

// win32
#ifdef WIN32
#include <Windows.h>
#endif

// AE
#include <AEGP_SuiteHandler.h>

namespace aegif
{
Locale GetAppLocale(const SPBasicSuite* pica_basicP) try
{
    AEGP_SuiteHandler suites(pica_basicP);
    A_char lang[PF_APP_LANG_TAG_SIZE] = {};
    suites.AppSuite5()->PF_AppGetLanguage(lang); // since CC
    if (strcmp(lang, "ja_JP") == 0)
    {
        return Locale::JA_JP;
    }
    else
    {
        return Locale::EN_US;
    }
}
catch (...)
{
    /* fallback */
#ifdef WIN32
    switch (::GetUserDefaultLangID())
    {
    case MAKELANGID(LANG_JAPANESE, SUBLANG_JAPANESE_JAPAN):
        return Locale::JA_JP;
    }
#endif

    return Locale::EN_US;
}
}
