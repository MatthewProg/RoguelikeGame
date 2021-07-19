#pragma once

#include <string>

#include "../UI/FocusContainer.h"
#include "../UI/ProgressBar.h"
#include "../UI/ListSelect.h"
#include "../UI/ScrollView.h"
#include "../UI/CheckBox.h"
#include "../UI/Button.h"
#include "../UI/Scene.h"

class UIHelper
{
public:
	static std::vector<sf::Vector2u> GetAllTypicalResolutions(uint32_t limitWidth, uint32_t limitHeight);
	static ProgressBar* ExtractProgressBar(Scene* scene, const std::string& scrollViewName, const std::string& focusContainerName, const std::string& elementName);
	static CheckBox* ExtractCheckBox(Scene* scene, const std::string& scrollViewName, const std::string& focusContainerName, const std::string& elementName);
	static ListSelect* ExtractListSelect(Scene* scene, const std::string& scrollViewName, const std::string& focusContainerName, const std::string& elementName);
	static Button* ExtractButton(Scene* scene, const std::string& scrollViewName, const std::string& focusContainerName, const std::string& elementName);
};

