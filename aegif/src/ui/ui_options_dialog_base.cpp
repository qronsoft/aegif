#include "ui_options_dialog_base.hpp"

// aegif
#ifdef WIN32
#include "ui_options_dialog_win.hpp"
#endif

namespace aegif
{
UIOptionsDialogBase::UIOptionsDialogBase()
{
}
UIOptionsDialogBase::~UIOptionsDialogBase()
{
}
UIOptionsDialogBase* UIOptionsDialogBase::Create(void* parentWindowHandle)
{
#ifdef WIN32
    return new UIOptionsDialogWin(HWND(parentWindowHandle));
#else
    static_assert(false, "unsupported platform");
#endif
}
}
