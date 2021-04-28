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
