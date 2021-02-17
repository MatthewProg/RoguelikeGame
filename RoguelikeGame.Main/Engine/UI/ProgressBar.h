#pragma once

#include "../UI/UIElement.h"

class ProgressBar : public UIElement
{
private:
	std::vector<std::tuple<sf::FloatRect,std::string>> _progressBarSteps;
	std::vector<std::tuple<sf::FloatRect,std::string>> _backgroundLayers;

	sf::Vector2f _progressBarStepsPos;

	float _prevValue;
	float _prevMaxValue;

	float _currentValue;
	float _maxValue;

	bool _sthChanged;
public:
	ProgressBar();
	~ProgressBar();

	ProgressBar& operator=(ProgressBar &other);

	//Var setters
	void SetCurrentValue(float value);
	void SetMaxValue(float maxValue);

	//Var getters
	float GetCurrentValue();
	float GetMaxValue();

	//Graphics setters
	void AddProgressBarStep(sf::FloatRect step, std::string textureName);
	void AddBackgroundLayer(sf::FloatRect layer, std::string textureName);
	void ResetProgressBarSteps();
	void ResetBackgroundLayers();
	void ReplaceProgressBarStep(size_t index, std::tuple<sf::FloatRect, std::string> newTuple);
	void ReplaceBackgroundLayer(size_t index, std::tuple<sf::FloatRect, std::string> newTuple);
	void RemoveProgressBarStep(size_t index);
	void RemoveBackgroundLayer(size_t index);
	void SetProgressBarStepsPos(sf::Vector2f pos);

	//Graphics getters
	size_t GetNoOfProgressBarSteps();
	size_t GetNoOfBackgroundLayers();
	std::tuple<sf::FloatRect, std::string> GetProgressBarStep(size_t index);
	std::tuple<sf::FloatRect, std::string> GetBackgroundLayer(size_t index);
	sf::Vector2f GetProgressBarStepsPos();
	sf::FloatRect GetProgressBarStepsGlobalBounds();

	// Inherited via UIElement
	virtual void RedrawElement() override;
	virtual void Update(bool, float) override;
	virtual void ProcessEvent(sf::Event* ev, sf::Vector2f mousePos) override;
};