#include "ui_options_dialog_win.hpp"

// std
#include <string>

// win32
#include <CommCtrl.h>

// aegif
#include "../../resource/options_dialog/resource.h"
#include "../dll_hinstance.hpp"
#include "../log.hpp"
#include "../scope_guard.hpp"

namespace
{
WPARAM BoolToCheck(bool b)
{
    return b ? BST_CHECKED : BST_UNCHECKED;
}
void CreateToolTip(HWND hWndParent, HWND hControlItem, LPWSTR pszText)
{
    const HWND hwndTip = CreateWindowExW(
        NULL,
        TOOLTIPS_CLASS,
        NULL,
        WS_POPUP | TTS_ALWAYSTIP,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        hWndParent,
        NULL,
        aegif::GetDllInstanceHandle(),
        NULL);

    if (hwndTip == nullptr)
    {
        AEGLOG_ERROR("failed to create tooltip");
        return;
    }

    TOOLINFOW toolInfo = { 0 };
    toolInfo.cbSize    = sizeof(toolInfo);
    toolInfo.hwnd      = hWndParent;
    toolInfo.uFlags    = TTF_IDISHWND | TTF_SUBCLASS;
    toolInfo.uId       = (UINT_PTR) hControlItem;
    toolInfo.lpszText  = pszText;
    if (!SendMessageW(hwndTip, TTM_ADDTOOL, 0, (LPARAM) &toolInfo))
    {
        AEGLOG_ERROR("failed to create tooltip");
    }
}
}

namespace aegif
{
UIOptionsDialogWin::UIOptionsDialogWin(HWND parentWindowHandle)
    : UIOptionsDialogBase()
    , parentWindowHandle_(parentWindowHandle)
    , langId_(MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US))
    , loop_(true)
    , fastEncode_(false)
    , imageQuality_(IMAGE_QUALITY_MAX)
{
}

UIOptionsDialogWin::~UIOptionsDialogWin()
{
}

bool UIOptionsDialogWin::Show()
{
    const auto prevLangId = GetThreadUILanguage();
    aegif::ScopeGuard restoreUILang([&]() { SetThreadUILanguage(prevLangId); });
    SetThreadUILanguage(langId_);

    const auto result = DialogBoxParamW(
        GetDllInstanceHandle(),
        MAKEINTRESOURCE(IDD_OPTIONS_DIALOG),
        parentWindowHandle_,
        s_DialogProc,
        reinterpret_cast<LPARAM>(this));
    switch (result)
    {
    case IDOK:
        AEGLOG_TRACE("dialog: ok selected");
        return true;
    case IDCANCEL:
        AEGLOG_TRACE("dialog: cancel selected");
        return false;
    default:
        AEGLOG_ERROR("dialog error occured: {}", GetLastError());
        return false;
    }
}

void UIOptionsDialogWin::SetLocale(Locale locale)
{
    switch (locale)
    {
    case Locale::JA_JP:
        langId_ = MAKELANGID(LANG_JAPANESE, SUBLANG_JAPANESE_JAPAN);
        break;
    default:
        langId_ = MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US);
        break;
    }
}

void UIOptionsDialogWin::SetImageQuality(int quality)
{
    imageQuality_ = std::clamp(quality, IMAGE_QUALITY_MIN, IMAGE_QUALITY_MAX);
}

int UIOptionsDialogWin::GetImageQuality()
{
    return std::clamp(imageQuality_, IMAGE_QUALITY_MIN, IMAGE_QUALITY_MAX);
}

INT_PTR CALLBACK UIOptionsDialogWin::s_DialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    aegif::UIOptionsDialogWin* pThis = nullptr;
    if (uMsg == WM_INITDIALOG)
    {
        pThis = reinterpret_cast<aegif::UIOptionsDialogWin*>(lParam);
        SetWindowLongPtr(hwndDlg, DWLP_USER, reinterpret_cast<LONG_PTR>(pThis));
    }
    else
    {
        pThis = reinterpret_cast<aegif::UIOptionsDialogWin*>(GetWindowLongPtr(hwndDlg, DWLP_USER));
    }

    if (pThis)
    {
        return pThis->DialogProc(hwndDlg, uMsg, wParam, lParam);
    }
    else
    {
        return FALSE;
    }
}

INT_PTR UIOptionsDialogWin::DialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_INITDIALOG:
        InitUI(hwndDlg);
        return TRUE;
    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case IDOK:
            AEGLOG_TRACE("ok button was clicked");
            EndDialog(hwndDlg, IDOK);
            return TRUE;
        case IDCANCEL:
            AEGLOG_TRACE("cancel button was clicked");
            EndDialog(hwndDlg, IDCANCEL);
            return TRUE;
        case IDC_CHECK_LOOP:
            loop_ = SendDlgItemMessageW(hwndDlg, IDC_CHECK_LOOP, BM_GETCHECK, 0, 0) == BST_CHECKED;
            AEGLOG_TRACE("loop checkbox was toggled: {}", loop_);
            return TRUE;
        case IDC_EDIT_IMAGE_QUALITY:
            switch (HIWORD(wParam))
            {
            case EN_UPDATE:
            {
                wchar_t buf[4] = {};
                GetWindowTextW(GetDlgItem(hwndDlg, IDC_EDIT_IMAGE_QUALITY), buf, 4);
                if (buf[0] != L'\0')
                {
                    imageQuality_ = _wtoi(buf);
                    AEGLOG_TRACE("image quality edit was updated: {}", imageQuality_);
                    if (imageQuality_ < IMAGE_QUALITY_MIN || imageQuality_ > IMAGE_QUALITY_MAX)
                    {
                        imageQuality_ = std::clamp(imageQuality_, IMAGE_QUALITY_MIN, IMAGE_QUALITY_MAX);
                        AEGLOG_TRACE("input value was clamped: {}", imageQuality_);
                        UpdateImageQualityEdit(hwndDlg);
                    }
                    UpdateImageQualitySlider(hwndDlg);
                    return TRUE;
                }
                else
                {
                    AEGLOG_TRACE("image quality edit was cleared");
                }
            }
            break;
            }
            return FALSE;
        case IDC_CHECK_FAST_ENCODE:
            fastEncode_
                = SendDlgItemMessageW(hwndDlg, IDC_CHECK_FAST_ENCODE, BM_GETCHECK, 0, 0) == BST_CHECKED;
            AEGLOG_TRACE("fast encode checkbox was toggled: {}", fastEncode_);
            return TRUE;
        }
        return FALSE;
    case WM_HSCROLL:
        if (HWND(lParam) == GetDlgItem(hwndDlg, IDC_SLIDER_IMAGE_QUALITY))
        {
            imageQuality_ = SendDlgItemMessageW(hwndDlg, IDC_SLIDER_IMAGE_QUALITY, TBM_GETPOS, 0, 0);
            AEGLOG_TRACE("image quality slider was tracked: {}", imageQuality_);
            UpdateImageQualityEdit(hwndDlg);
            return TRUE;
        }
        return FALSE;
    case WM_CLOSE:
        AEGLOG_TRACE("dialog was closed");
        EndDialog(hwndDlg, IDCANCEL);
        return TRUE;
    default:
        return FALSE;
    }
}
void UIOptionsDialogWin::InitUI(HWND hwndDlg)
{
    /* config */
    // image-quality-slider
    SendDlgItemMessageW(hwndDlg, IDC_SLIDER_IMAGE_QUALITY, TBM_SETRANGEMIN, FALSE, IMAGE_QUALITY_MIN);
    SendDlgItemMessageW(hwndDlg, IDC_SLIDER_IMAGE_QUALITY, TBM_SETRANGEMAX, TRUE, IMAGE_QUALITY_MAX);

    // image-quality-edit
    SendDlgItemMessageW(hwndDlg, IDC_EDIT_IMAGE_QUALITY, EM_SETLIMITTEXT, 3, 0);

    // fast-encode-checkbox
    CreateToolTip(
        hwndDlg,
        GetDlgItem(hwndDlg, IDC_CHECK_FAST_ENCODE),
        L"3 times faster encoding, but 10% lower quality and bigger file");

    /* update states */
    UpdateLoopCheckbox(hwndDlg);
    UpdateImageQualityEdit(hwndDlg);
    UpdateImageQualitySlider(hwndDlg);
    UpdateFastEncodeCheckbox(hwndDlg);
}
void UIOptionsDialogWin::UpdateLoopCheckbox(HWND hwndDlg)
{
    SendDlgItemMessageW(hwndDlg, IDC_CHECK_LOOP, BM_SETCHECK, BoolToCheck(loop_), 0);
}
void UIOptionsDialogWin::UpdateImageQualityEdit(HWND hwndDlg)
{
    SetWindowTextW(GetDlgItem(hwndDlg, IDC_EDIT_IMAGE_QUALITY), std::to_wstring(imageQuality_).c_str());
    // move cursor to end
    SendDlgItemMessageW(hwndDlg, IDC_EDIT_IMAGE_QUALITY, EM_SETSEL, 0, -1);
    SendDlgItemMessageW(hwndDlg, IDC_EDIT_IMAGE_QUALITY, EM_SETSEL, -1, 0);
}
void UIOptionsDialogWin::UpdateImageQualitySlider(HWND hwndDlg)
{
    SendDlgItemMessageW(hwndDlg, IDC_SLIDER_IMAGE_QUALITY, TBM_SETPOS, TRUE, imageQuality_);
}
void UIOptionsDialogWin::UpdateFastEncodeCheckbox(HWND hwndDlg)
{
    SendDlgItemMessageW(hwndDlg, IDC_CHECK_FAST_ENCODE, BM_SETCHECK, BoolToCheck(fastEncode_), 0);
}
}
