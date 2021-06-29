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
	float _step;
	float _maxValue;

	bool _sthChanged;
public:
	ProgressBar();
	ProgressBar(ProgressBar& other);
	~ProgressBar();

	//Var setters
	void SetCurrentValue(float value);
	void SetMaxValue(float maxValue);
	void SetForceStep(float step);

	//Var getters
	float GetCurrentValue() const;
	float GetMaxValue() const;
	float GetStep() const;

	//Graphics setters
	void AddProgressBarStep(const sf::FloatRect& step, const std::string& textureName);
	void AddBackgroundLayer(const sf::FloatRect& layer, const std::string& textureName);
	void ResetProgressBarSteps();
	void ResetBackgroundLayers();
	void ReplaceProgressBarStep(size_t index, const std::tuple<sf::FloatRect, std::string>& newTuple);
	void ReplaceBackgroundLayer(size_t index, const std::tuple<sf::FloatRect, std::string>& newTuple);
	void RemoveProgressBarStep(size_t index);
	void RemoveBackgroundLayer(size_t index);
	void SetProgressBarStepsPos(const sf::Vector2f& pos);

	//Graphics getters
	size_t GetNoOfProgressBarSteps() const;
	size_t GetNoOfBackgroundLayers() const;
	std::tuple<sf::FloatRect, std::string> GetProgressBarStep(size_t index) const;
	std::tuple<sf::FloatRect, std::string> GetBackgroundLayer(size_t index) const;
	const sf::Vector2f& GetProgressBarStepsPos() const;
	sf::FloatRect GetProgressBarStepsGlobalBounds() const;

	// Inherited via UIElement
	virtual UIElement* clone() override;
	virtual void RedrawElement() override;
	virtual void Update(bool, float) override;
	virtual void ProcessEvent(sf::Event* ev, const sf::Vector2f& mousePos) override;
	virtual std::vector<sf::Vector2f> GetAllBoundsPoints() const override;
};