#pragma once

#include <vector>
#include "Utilities.h"

#include "SFML/Graphics/Transformable.hpp"
#include "SFML/Graphics/RenderTarget.hpp"
#include "SFML/Graphics/VertexArray.hpp"
#include "SFML/Graphics/Drawable.hpp"
#include "SFML/Graphics/Texture.hpp"

namespace sf
{
	class Animation : public Drawable, public Transformable
	{
	private:
		unsigned int _changeEveryTicks;
		float _animationSpeed;

		unsigned int _currentTick;
		unsigned int _currentFrame;

		bool _switchLeftRight;
		bool _switchUpDown;

		std::vector<sf::IntRect> _rectFrames;

		Texture* _texture;
		Texture* _noTexture;

		sf::VertexArray _vertices;
		sf::RenderStates _renderStates;

		std::vector<sf::Color> _frameColor;

		void NextFrame();

		// Inherited via Drawable
		virtual void draw(RenderTarget& target, RenderStates states) const override;
	public:
		Animation();
		~Animation();

		//Animation
		void Tick(bool tick);
		void UpdateVertices();
		void AddNewFrame(sf::IntRect rect);
		void RemoveFrame(int index);
		void RemoveAllFrames();
		size_t GetNoOfFrames();
		void SetFrames(std::vector<sf::IntRect> frames);

		void FlipHorizontally();
		void FlipVertically();

		//Animation setters
		void SetChangeFrameEvery(unsigned int ticks);
		void SetAnimationSpeed(float speed);
		void SetTexture(sf::Texture* texture);
		void SetFrameColor(unsigned int frame, sf::Color color);
		void SetHorizontalFlip(bool flip);
		void SetVerticalFlip(bool flip);

		//Animation getters
		unsigned int GetChangeFrameEvery();
		float GetAnimationSpeed();
		sf::IntRect GetCurrentRect();
		const sf::Texture* GetTexture();
		sf::Color GetFrameColor(unsigned int frame);
		bool GetHorizontalFlip();
		bool GetVerticalFlip();

		//Texture getteres
		bool IsRepeated();
		bool IsSmooth();

		//Frame getters
		FloatRect GetGlobalBounds();
		FloatRect GetLocalBounds();
	};
}

