#pragma once

#include "../Helpers/CollisionHelper.h"
#include "../Managers/TexturesManager.h"
#include "../Managers/SoundsManager.h"
#include "../Utilities/Utilities.h"

#include "SFML/Graphics/Transformable.hpp"
#include "SFML/Graphics/RenderTexture.hpp"

class UIElement : public sf::Transformable
{
protected:
	sf::RenderTexture _render;

	bool _isVisible;
	bool _keyboardInput;
	bool _mouseInput;
	bool _focusOnHover;
	bool _inFocus;
	bool _sthChanged;
	bool _redrawHappened;

	sf::Texture* _noTexture;
	TexturesManager* _texturesManager;
	SoundsManager* _soundsManager;
public:
	enum class Align { START, MIDDLE, END };

	UIElement();
	UIElement(UIElement& other);
	virtual UIElement* clone() = 0;
	virtual ~UIElement() { ; }

	void Init(const sf::Vector2u& size);
	virtual void Update(bool tick, float delta) = 0;
	virtual void ForceRedraw() = 0;
	virtual bool Redraw();
	bool RedrawHappened();
	virtual void ProcessEvent(sf::Event* ev, const sf::Vector2f& mousePos) = 0;

	void SetVisibility(bool visible);
	void SetMouseInput(bool active);
	void SetKeyboardInput(bool active);
	void SetTexturesManager(TexturesManager* manager);
	void SetSoundsManager(SoundsManager* manager);
	void SetFocusOnHover(bool hover);
	void SetInFocus(bool isFocused);

	bool GetVisibility() const;
	bool GetMouseInput() const;
	bool GetKeyboardInput() const;
	bool GetFocusOnHover() const;
	bool GetInFocus() const;
	virtual sf::FloatRect GetGlobalBounds() const;
	virtual std::vector<sf::Vector2f> GetAllBoundsPoints() const;
	virtual std::vector<sf::Vector2f> GetDeepestInFocusBoundsPoints() const;
	const sf::RenderTexture* GetTexture() const;
};

