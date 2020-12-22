#pragma once

#include <map>
#include "Animation.h"

namespace sf
{
	class AnimationContainer : public sf::Drawable
	{
	private:
		std::map<std::string, sf::Animation> _animationStates;

		std::string _currentState;
		sf::Animation* _currentAnimation;

		sf::Texture* _noTexture;
		sf::VertexArray _noTextureVertex;
		sf::Transformable _noTextureTransform;

		bool HasStateAndAnimation();
		void Update();

		// Inherited via Drawable
		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	public:
		AnimationContainer();
		~AnimationContainer();

		void Tick(bool tick);

		//AnimationContainer setters
		void SetStateAnimation(std::string state, sf::Animation animation);
		void SetAnimationStates(std::map<std::string, sf::Animation> animationStates);
		void SetCurrentState(std::string state);
		void RenameState(std::string oldName, std::string newName);
		void RemoveState(std::string state);

		//AnimationContainer getters
		sf::Animation* GetStateAnimation(std::string state);
		std::map<std::string, sf::Animation>* GetAnimationStates();

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
	};
}