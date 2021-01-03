#pragma once

// AE
#include <SPBasic.h>

namespace aegif
{
enum class Locale
{
    EN_US,
    JA_JP
};

Locale GetAppLocale(const SPBasicSuite* pica_basicP);
}
