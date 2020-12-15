#pragma once

#include <vector>

#include "SFML/Graphics.hpp"

namespace sf
{
	class Animation : public Drawable
	{
	private:
		unsigned int _changeEveryTicks;
		float _animationSpeed;

		unsigned int _currentTick;
		unsigned int _currentFrame;

		std::vector<sf::IntRect> _rectFrames;

		Texture _mainTexture;
		Sprite _mainSprite;

		void NextFrame();

		// Inherited via Drawable
		virtual void draw(RenderTarget& target, RenderStates states) const override;
	public:
		Animation();
		~Animation();

		//Animation
		void Tick(bool tick);
		void AddNewFrame(sf::IntRect rect);
		void RemoveFrame(int index);
		void RemoveAllFrames();
		void GetNoOfFrames();
		void SetFrames(std::vector<sf::IntRect> frames);

		//Animation setters
		void SetChangeFrameEvery(unsigned int ticks);
		void SetAnimationSpeed(float speed);

		//Animation getters
		unsigned int GetChangeFrameEvery();
		float GetAnimationSpeed();

		//Texture
		bool LoadFromFile(const std::string& path);
		bool LoadFromMemory(const void* data, size_t size);
		bool LoadFromStream(sf::InputStream& stream);
		bool LoadFromImage(const sf::Image& img, const sf::IntRect& area = sf::IntRect());

		//Texture setters
		void SetRepeated(bool repeateTexture);
		void SetSmooth(bool smooth);

		//Texture getteres
		bool IsRepeated();
		bool IsSmooth();

		//Sprite setters
		void SetColor(const Color& color);
		void SetOrigin(const Vector2f& origin);
		void SetOrigin(float x, float y);
		void SetPosition(const Vector2f& position);
		void SetPosition(float x, float y);
		void SetRotation(float angle);
		void SetScale(const Vector2f& factors);
		void SetScale(float factorX, float factorY);

		//Sprite getters
		Color GetColor();
		Vector2f GetOrigin();
		Vector2f GetPosition();
		float GetRotation();
		Vector2f GetScale();
		FloatRect GetGlobalBounds();
		FloatRect GetLocalBounds();
		Transform GetTransform();
	};
}

