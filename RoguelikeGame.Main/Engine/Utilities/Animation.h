#pragma once

#include <vector>
#include "../Utilities/Utilities.h"

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

		bool _loop;
		bool _paused;
		bool _ended;

		std::vector<sf::IntRect> _rectFrames;

		Texture* _texture;
		Texture* _noTexture;

		sf::VertexArray _vertices;

		sf::Transformable _externalTransform;

		std::vector<sf::Color> _frameColor;

		bool _frameSwitched;
		void NextFrame();

		// Inherited via Drawable
		virtual void draw(RenderTarget& target, RenderStates states) const override;
	public:
		Animation();
		~Animation();

		//Animation
		void Start();
		void Pause();
		void Reset();

		void Tick(bool tick);
		void UpdateVertices();
		void AddNewFrame(sf::IntRect rect);
		void RemoveFrame(int index);
		void RemoveAllFrames();
		size_t GetNoOfFrames();
		void SetFrames(std::vector<sf::IntRect> frames);

		sf::Transformable* ExternalTransform();

		void FlipHorizontally();
		void FlipVertically();

		//Animation setters
		void SetChangeFrameEvery(unsigned int ticks);
		void SetAnimationSpeed(float speed);
		void SetTexture(sf::Texture* texture);
		void SetFrameColor(unsigned int frame, sf::Color color);
		void SetHorizontalFlip(bool flip);
		void SetVerticalFlip(bool flip);
		void SetLoop(bool loop);

		//Animation getters
		unsigned int GetChangeFrameEvery();
		float GetAnimationSpeed();
		sf::IntRect GetCurrentRect();
		const sf::Texture* GetTexture();
		sf::Color GetFrameColor(unsigned int frame);
		bool GetHorizontalFlip();
		bool GetVerticalFlip();
		bool GetLoop();
		bool IsPaused();
		bool IsEnded();
		bool IsFrameSwitched();

		//Texture getteres
		bool IsTextureRepeated();
		bool IsTextureSmooth();

		//Frame getters
		FloatRect GetGlobalBounds();
		FloatRect GetLocalBounds();
	};
}

