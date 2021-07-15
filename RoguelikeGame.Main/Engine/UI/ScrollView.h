#pragma once

#include <vector>
#include <map>

#include "../UI/FocusContainer.h"
#include "../UI/ScrollBar.h"
#include "../UI/UIElement.h"

class ScrollView : public UIElement
{
private:
	typedef std::tuple<FocusContainer*, std::string> Element;
	sf::RenderTexture _elementsRender;
	std::vector<Element> _elements;
	size_t _currentIndex;

	ScrollBar* _scrollBar;
	float _scroll;

	std::string _backgroundTex;
	sf::FloatRect _backgroundRect;
	sf::Transformable _backgroundTransform;
	sf::Vector2f _backgroundSize;

	sf::FloatRect _elementsBounds;
	sf::Transformable _elementsTransform;
	sf::FloatRect _listRect;

	bool _scrollHorizontaly;

	void UpdateElementsBounds();
	void UpdateListRect();
	void AlignElements();
	void ScrollToCurrentElement();
	void RedrawElements();
public:
	ScrollView();
	ScrollView(ScrollView& other);
	~ScrollView();

	void AddElement(const std::string& name, FocusContainer* container);
	FocusContainer* GetElement(size_t index);
	FocusContainer* GetElement(const std::string& name);
	void ClearElements();
	void RemoveElement(size_t index);
	void RemoveElement(const std::string& name);
	void SwapElements(size_t firstIndex, size_t secondIndex);
	void SwapElements(const std::string& firstName, const std::string& secondName);
	size_t GetNoOfElements() const;
	bool ElementExists(const std::string& name) const;
	void NextElement();
	void PreviousElement();

	void SetCurrentIndex(size_t index);
	void SetScrollHorizontaly(bool horizontalMode);
	void SetScrollBar(ScrollBar* scrollBar);
	void SetBackground(const std::string& texName, const sf::FloatRect& rect);
	void SetBackgroundSize(const sf::Vector2f& size);
	void SetListSize(const sf::Vector2f& size);
	void SetScroll(float percent);

	size_t GetCurrentIndex() const;
	bool GetScrollHorizontaly() const;
	ScrollBar* GetScrollBar();
	std::tuple<std::string, sf::FloatRect> GetBackground() const;
	const sf::Vector2f& GetBackgroundSize() const;
	sf::Vector2f GetListSize() const;
	float GetScroll();
	sf::Transformable* GetBackgroundTransform();
	sf::Transformable* GetElementsTransform();
	const sf::FloatRect& GetElementsBounds() const;

	// Inherited via UIElement
	virtual UIElement* clone() override;
	virtual void Update(bool tick, float delta) override;
	virtual void ForceRedraw() override;
	virtual bool Redraw() override;
	virtual void ProcessEvent(sf::Event* ev, const sf::Vector2f& mousePos) override;
	virtual std::vector<sf::Vector2f> GetAllBoundsPoints() const override;
	virtual std::vector<sf::Vector2f> GetDeepestInFocusBoundsPoints() const;
};

