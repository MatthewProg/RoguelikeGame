#include "UIHelper.h"

float UIHelper::ExtractProgressBarValue(Scene* scene, const std::string& focusContainerName, const std::string& elementName)
{
    if(scene == nullptr) return 0.f;

    auto fc = scene->GetElement(focusContainerName);
    if (fc == nullptr) return 0.f;

    auto pb = ((FocusContainer*)fc)->GetElement(elementName);
    if (pb == nullptr) return 0.f;

    return ((ProgressBar*)pb)->GetCurrentValue();
}

bool UIHelper::ExtractCheckBoxValue(Scene* scene, const std::string& focusContainerName, const std::string& elementName)
{
    if (scene == nullptr) return false;

    auto fc = scene->GetElement(focusContainerName);
    if (fc == nullptr) return false;

    auto cb = ((FocusContainer*)fc)->GetElement(elementName);
    if (cb == nullptr) return false;

    return ((CheckBox*)cb)->IsChecked();
}
