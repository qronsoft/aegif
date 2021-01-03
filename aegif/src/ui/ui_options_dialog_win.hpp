#pragma once

// win32
#include <Windows.h>

// aegif
#include "ui_options_dialog_base.hpp"

namespace aegif
{
class UIOptionsDialogWin : public aegif::UIOptionsDialogBase
{
public:
    UIOptionsDialogWin(HWND parentWindowHandle);
    virtual ~UIOptionsDialogWin();
    bool Show() override;

    void SetLocale(Locale locale) override;

    void SetLoopEnable(bool enable) override { loop_ = enable; }
    void SetFastEncodeEnable(bool enable) override { fastEncode_ = enable; }
    void SetImageQuality(int quality) override;

    bool GetLoopEnable() override { return loop_; }
    bool GetFastEncodeEnable() override { return fastEncode_; }
    int GetImageQuality() override;

private:
    static INT_PTR CALLBACK s_DialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
    INT_PTR CALLBACK DialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);

    void InitUI(HWND hwndDlg);
    void UpdateLoopCheckbox(HWND hwndDlg);
    void UpdateImageQualityEdit(HWND hwndDlg);
    void UpdateImageQualitySlider(HWND hwndDlg);
    void UpdateFastEncodeCheckbox(HWND hwndDlg);

    HWND parentWindowHandle_;
    LANGID langId_;

    bool loop_;
    bool fastEncode_;
    int imageQuality_;
};
}
