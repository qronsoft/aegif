#pragma once

// std
#include <memory>

// aegif
#include "locale.hpp"

namespace aegif
{
class UIOptionsDialogBase;

class OptionsDialog
{
public:
    OptionsDialog(void* parentWindowHandle);
    ~OptionsDialog();

    void SetLocale(Locale locale);

    void SetLoopEnable(bool enable);
    void SetFastEncodeEnable(bool enable);
    void SetImageQuality(int quality);

    bool ShowModal();

    bool GetLoopEnable();
    bool GetFastEncodeEnable();
    int GetImageQuality();

private:
    OptionsDialog(const OptionsDialog&) = delete;
    OptionsDialog& operator=(const OptionsDialog&) = delete;

    std::unique_ptr<UIOptionsDialogBase> uiOptionsDialog_;
};
}
