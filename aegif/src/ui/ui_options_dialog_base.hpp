#pragma once

// aegif
#include "../locale.hpp"

namespace aegif
{
class UIOptionsDialogBase
{
public:
    UIOptionsDialogBase();
    virtual ~UIOptionsDialogBase();

    virtual bool Show() = 0; // ok: true, cancel: false

    virtual void SetLocale(Locale locale) = 0;

    virtual void SetLoopEnable(bool enable)       = 0;
    virtual void SetFastEncodeEnable(bool enable) = 0;
    virtual void SetImageQuality(int quality)     = 0;

    virtual bool GetLoopEnable()       = 0;
    virtual bool GetFastEncodeEnable() = 0;
    virtual int GetImageQuality()      = 0;

    static UIOptionsDialogBase* Create(void* parentWindowHandle);

    static constexpr int IMAGE_QUALITY_MIN = 50;
    static constexpr int IMAGE_QUALITY_MAX = 100;
};
}
