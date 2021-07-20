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
		void SetHitboxScale(const sf::Vector2f& scale);

		sf::Vector2f GetHitboxScale();

		// Inherited via Drawable
		void draw(RenderTarget& target, RenderStates) const override;
	public:
		Collision();
		Collision(Collision& other);
		~Collision() override = default;

		sf::FloatRect GetCollisionBox() const;
		sf::FloatRect GetCollisionBoxOffset() const;
		const sf::RectangleShape& GetHitboxRectangle() const;
		const sf::Color& GetHitboxColor() const;
		const sf::Color& GetHitboxOutlineColor() const;
		float GetHitboxOutlineThickness() const;
		bool GetHitboxVisibility() const;
		const sf::Vector2f& GetHitboxPosition() const;

		void SetCollisionBoxOffset(const sf::FloatRect& rect);
		void SetHitboxColor(const sf::Color& color);
		void SetHitboxOutlineColor(const sf::Color& color);
		void SetHitboxOutlineThickness(float thickness);
		void SetHitboxVisibility(bool visible);
		void SetHitboxPosition(const sf::Vector2f& pos);
		void SetHitboxPosition(float x, float y);
	};
}

