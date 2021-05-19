#pragma once

#include <string>

#include "../UI/FocusContainer.h"
#include "../UI/ProgressBar.h"
#include "../UI/CheckBox.h"
#include "../UI/Scene.h"


class UIHelper
{
public:
	static float ExtractProgressBarValue(Scene* scene, const std::string& focusContainerName, const std::string& elementName);
	static bool ExtractCheckBoxValue(Scene* scene, const std::string& focusContainerName, const std::string& elementName);
};

