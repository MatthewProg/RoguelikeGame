#pragma once

#include "SFML/Graphics/Rect.hpp"
#include "SFML/Graphics/RenderTarget.hpp"
#include "SFML/Graphics/RectangleShape.hpp"

namespace sf
{
	class Collision : public sf::Drawable
	{
	private:
		sf::FloatRect _hitboxOffset;
		sf::RectangleShape _hitboxRecangle;

		bool _showHitbox;
	protected:
		// Inherited via Drawable
		virtual void draw(RenderTarget& target, RenderStates states) const override;
	public:
		Collision();
		~Collision();

		sf::FloatRect GetCollisionBox();
		sf::FloatRect GetCollisionBoxOffset();
		sf::RectangleShape GetHitboxRectangle();
		sf::Color GetHitboxColor();
		sf::Color GetHitboxOutlineColor();
		float GetHitboxOutlineThickness();
		bool GetHitboxVisibility();
		sf::Vector2f GetHitboxPosition();

		void SetCollisionBoxOffset(sf::FloatRect rect);
		void SetHitboxColor(const sf::Color& color);
		void SetHitboxOutlineColor(const sf::Color& color);
		void SetHitboxOutlineThickness(float thickness);
		void SetHitboxVisibility(bool visible);
		void SetHitboxPosition(sf::Vector2f pos);
		void SetHitboxPosition(float x, float y);
	};
}

