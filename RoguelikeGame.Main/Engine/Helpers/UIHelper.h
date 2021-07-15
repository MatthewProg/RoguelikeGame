#pragma once

#include <string>

#include "../UI/FocusContainer.h"
#include "../UI/ProgressBar.h"
#include "../UI/ScrollView.h"
#include "../UI/CheckBox.h"
#include "../UI/Scene.h"


class UIHelper
{
public:
	static ProgressBar* ExtractProgressBar(Scene* scene, const std::string& scrollViewName, const std::string& focusContainerName, const std::string& elementName);
	static CheckBox* ExtractCheckBox(Scene* scene, const std::string& scrollViewName, const std::string& focusContainerName, const std::string& elementName);
};

