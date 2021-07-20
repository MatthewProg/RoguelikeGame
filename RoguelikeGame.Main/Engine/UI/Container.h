#pragma once

#include "../UI/UIElement.h"
#include "../Helpers/ViewHelper.h"

class Container : public UIElement
{
protected:
	std::map<std::string, UIElement*> _uiElements;
public:
	Container();
	Container(Container& other);
	~Container();

	void AddElement(const std::string& name, UIElement* element);
	void RemoveElement(UIElement* element);
	void RemoveElement(const std::string& name);
	void RenameElement(const std::string& oldName, const std::string& newName);
	UIElement* GetElement(const std::string& name);
	const std::map<std::string, UIElement*>& GetElements() const;

	void AutoAlignElementsHorizontally(Align align);
	void AutoAlignElementsVertically(Align align);

	sf::FloatRect GetElementsGlobalBounds() const;

	// Inherited via UIElement
	UIElement* clone() override;
	void Update(bool tick, float delta) override;
	void ForceRedraw() override;
	bool Redraw() override;
	void ProcessEvent(sf::Event* ev, const sf::Vector2f& mousePos) override;
	std::vector<sf::Vector2f> GetAllBoundsPoints() const override;
	std::vector<sf::Vector2f> GetDeepestInFocusBoundsPoints() const override;
};

