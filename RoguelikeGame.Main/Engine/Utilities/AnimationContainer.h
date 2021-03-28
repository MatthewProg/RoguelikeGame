#pragma once

#include <map>
#include "../Utilities/Animation.h"

namespace sf
{
	class AnimationContainer : public sf::Drawable
	{
	private:
		std::map<std::string, sf::Animation> _animationStates;

		std::string _currentState;
		sf::Animation* _currentAnimation;

		std::string _smoothChangeState;
		bool _smoothChangePrevLoop;

		sf::Texture* _noTexture;
		sf::VertexArray _noTextureVertex;
		sf::Transformable _noTextureTransform;

		bool HasStateAndAnimation();

		// Inherited via Drawable
		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	public:
		AnimationContainer();
		~AnimationContainer();

		void Tick(bool tick);
		void UpdateCurrentAnimationPtr();

		//AnimationContainer setters
		void SetStateAnimation(const std::string& state, const sf::Animation& animation);
		void SetAnimationStates(const std::map<std::string, sf::Animation>& animationStates);
		void SetCurrentState(const std::string& state);
		void SmoothStateChange(const std::string& state);
		void RenameState(const std::string& oldName, const std::string& newName);
		void RemoveState(const std::string& state);

		//AnimationContainer getters
		sf::Animation* GetStateAnimation(const std::string& state);
		std::map<std::string, sf::Animation>* GetAnimationStates();
		const std::string& GetCurrentState() const;

		//Animation setters
		void ApplySetChangeFrameEvery(unsigned int ticks);
		void ApplySetAnimationSpeed(float speed);
		void ApplySetColor(const Color& color);
		void ApplySetOrigin(const Vector2f& origin);
		void ApplySetOrigin(float x, float y);
		void ApplySetPosition(const Vector2f& position);
		void ApplySetPosition(float x, float y);
		void ApplySetRotation(float angle);
		void ApplySetScale(const Vector2f& factors);
		void ApplySetScale(float factorX, float factorY);
		void ApplyFlipHorizontally();
		void ApplyFlipVertically();
		void ApplySetHorizontalFlip(bool flip);
		void ApplySetVerticalFlip(bool flip);
	};
}