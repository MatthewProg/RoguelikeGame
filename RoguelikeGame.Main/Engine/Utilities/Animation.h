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
		void draw(RenderTarget& target, RenderStates states) const override;
	public:
		Animation();
		~Animation() override = default;

		//Animation
		void Start();
		void Pause();
		void Reset();

		void Tick(bool tick);
		void UpdateVertices();
		void AddNewFrame(const sf::IntRect& rect);
		void RemoveFrame(size_t index);
		void RemoveAllFrames();
		size_t GetNoOfFrames() const;
		void SetFrames(const std::vector<sf::IntRect>& frames);

		sf::Transformable* ExternalTransform();

		void FlipHorizontally();
		void FlipVertically();

		//Animation setters
		void SetChangeFrameEvery(unsigned int ticks);
		void SetAnimationSpeed(float speed);
		void SetTexture(sf::Texture* texture);
		void SetFrameColor(size_t frame, const sf::Color& color);
		void SetHorizontalFlip(bool flip);
		void SetVerticalFlip(bool flip);
		void SetLoop(bool loop);

		//Animation getters
		unsigned int GetChangeFrameEvery() const;
		float GetAnimationSpeed() const;
		const sf::IntRect& GetCurrentRect() const;
		const sf::Texture* GetTexture() const;
		sf::Color GetFrameColor(size_t frame) const;
		bool GetHorizontalFlip() const;
		bool GetVerticalFlip() const;
		bool GetLoop() const;
		bool IsPaused() const;
		bool IsEnded() const;
		bool IsFrameSwitched() const;

		//Texture getteres
		bool IsTextureRepeated() const;
		bool IsTextureSmooth() const;

		//Frame getters
		FloatRect GetGlobalBounds() const;
		FloatRect GetLocalBounds() const;
	};
}

