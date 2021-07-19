#include "UIHelper.h"

std::vector<sf::Vector2u> UIHelper::GetAllTypicalResolutions(uint32_t limitWidth, uint32_t limitHeight)
{
    std::array<sf::Vector2u, 15> resolutions
    {
        sf::Vector2u(640,360),
        sf::Vector2u(800,450),
        sf::Vector2u(854,480),
        sf::Vector2u(960,540),
        sf::Vector2u(1024,576),
        sf::Vector2u(1152,648),
        sf::Vector2u(1280,720),
        sf::Vector2u(1366,768),
        sf::Vector2u(1600,900),
        sf::Vector2u(1920,1080),
        sf::Vector2u(2560,1440),
        sf::Vector2u(3200,1800),
        sf::Vector2u(3840,2160),
        sf::Vector2u(5120,2880),
        sf::Vector2u(7680,4320)
    };
    std::vector<sf::Vector2u> output;
    for (auto& r : resolutions)
        if (r.x <= limitWidth && r.y <= limitHeight)
            output.push_back(r);
        else
            break;

    return output;
}

ProgressBar* UIHelper::ExtractProgressBar(Scene* scene, const std::string& scrollViewName, const std::string& focusContainerName, const std::string& elementName)
{
    if(scene == nullptr) return nullptr;

    auto sv = scene->GetElement(scrollViewName);
    if (sv == nullptr) return nullptr;

    auto fc = ((ScrollView*)sv)->GetElement(focusContainerName);
    if (fc == nullptr) return nullptr;

    auto pb = fc->GetElement(elementName);

    return ((ProgressBar*)pb);
}

CheckBox* UIHelper::ExtractCheckBox(Scene* scene, const std::string& scrollViewName, const std::string& focusContainerName, const std::string& elementName)
{
    if (scene == nullptr) return nullptr;

    auto sv = scene->GetElement(scrollViewName);
    if (sv == nullptr) return nullptr;

    auto fc = ((ScrollView*)sv)->GetElement(focusContainerName);
    if (fc == nullptr) return nullptr;

    auto cb = fc->GetElement(elementName);

    return ((CheckBox*)cb);
}

ListSelect* UIHelper::ExtractListSelect(Scene* scene, const std::string& scrollViewName, const std::string& focusContainerName, const std::string& elementName)
{
    if (scene == nullptr) return nullptr;

    auto sv = scene->GetElement(scrollViewName);
    if (sv == nullptr) return nullptr;

    auto fc = ((ScrollView*)sv)->GetElement(focusContainerName);
    if (fc == nullptr) return nullptr;

    auto ls = fc->GetElement(elementName);

    return ((ListSelect*)ls);
}

Button* UIHelper::ExtractButton(Scene* scene, const std::string& scrollViewName, const std::string& focusContainerName, const std::string& elementName)
{
    if (scene == nullptr) return nullptr;

    auto sv = scene->GetElement(scrollViewName);
    if (sv == nullptr) return nullptr;

    auto fc = ((ScrollView*)sv)->GetElement(focusContainerName);
    if (fc == nullptr) return nullptr;

    auto btn = fc->GetElement(elementName);

    return ((Button*)btn);
}
