#pragma once

#include "../Managers/TexturesManager.h"
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

	sf::Texture* _noTexture;
	TexturesManager* _texturesManager;
public:
	UIElement();
	~UIElement();

	void Init(sf::Vector2u size);
	virtual void Update(bool tick, float delta) = 0;
	virtual void RedrawElement() = 0;
	virtual void ProcessEvent(sf::Event* ev, sf::Vector2f mousePos) = 0;

	void SetVisibility(bool visible);
	void SetMouseInput(bool active);
	void SetKeyboardInput(bool active);
	void SetTexturesManager(TexturesManager* manager);
	void SetFocusOnHover(bool hover);

	bool GetVisibility();
	bool GetMouseInput();
	bool GetKeyboardInput();
	bool GetFocusOnHover();
	sf::FloatRect GetGlobalBounds();
	sf::RenderTexture* GetTexture();
};

