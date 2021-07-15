#include "UIHelper.h"

ProgressBar* UIHelper::ExtractProgressBar(Scene* scene, const std::string& scrollViewName, const std::string& focusContainerName, const std::string& elementName)
{
    if(scene == nullptr) return nullptr;

    auto sv = scene->GetElement(scrollViewName);
    if (sv == nullptr) return nullptr;

    auto fc = ((ScrollView*)sv)->GetElement(focusContainerName);
    if (fc == nullptr) return nullptr;

    auto pb = ((FocusContainer*)fc)->GetElement(elementName);

    return ((ProgressBar*)pb);
}

CheckBox* UIHelper::ExtractCheckBox(Scene* scene, const std::string& scrollViewName, const std::string& focusContainerName, const std::string& elementName)
{
    if (scene == nullptr) return nullptr;

    auto sv = scene->GetElement(scrollViewName);
    if (sv == nullptr) return nullptr;

    auto fc = ((ScrollView*)sv)->GetElement(focusContainerName);
    if (fc == nullptr) return nullptr;

    auto cb = ((FocusContainer*)fc)->GetElement(elementName);

    return ((CheckBox*)cb);
}
