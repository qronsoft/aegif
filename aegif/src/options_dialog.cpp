#include "options_dialog.hpp"

// aegif
#include "ui/ui_options_dialog_base.hpp"

namespace aegif
{
OptionsDialog::OptionsDialog(void* parentWindowHandle)
    : uiOptionsDialog_(nullptr)
{
    uiOptionsDialog_.reset(UIOptionsDialogBase::Create(parentWindowHandle));
}
OptionsDialog::~OptionsDialog()
{
}
void OptionsDialog::SetLocale(Locale locale)
{
    uiOptionsDialog_->SetLocale(locale);
}
void OptionsDialog::SetLoopEnable(bool enable)
{
    uiOptionsDialog_->SetLoopEnable(enable);
}
void OptionsDialog::SetFastEncodeEnable(bool enable)
{
    uiOptionsDialog_->SetFastEncodeEnable(enable);
}
void OptionsDialog::SetImageQuality(int quality)
{
    uiOptionsDialog_->SetImageQuality(quality);
}
bool OptionsDialog::ShowModal()
{
    return uiOptionsDialog_->Show();
}
bool OptionsDialog::GetLoopEnable()
{
    return uiOptionsDialog_->GetLoopEnable();
}
bool OptionsDialog::GetFastEncodeEnable()
{
    return uiOptionsDialog_->GetFastEncodeEnable();
}
int OptionsDialog::GetImageQuality()
{
    return uiOptionsDialog_->GetImageQuality();
}
}
