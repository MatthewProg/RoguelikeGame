#include "AnimationContainer.h"

namespace sf
{
	bool AnimationContainer::HasStateAndAnimation()
	{
		if (_animationStates.size() == 0 && _currentState == "")
			return false;
		if (_animationStates.count(_currentState) == 0)
			return false;
		return true;
	}

	AnimationContainer::AnimationContainer()
	{
		_currentAnimation = nullptr;
		_noTexture.loadFromImage(Utilities::GetInstance()->NoTexture16x16());
		_noSprite.setTexture(_noTexture);
	}

	AnimationContainer::~AnimationContainer()
	{
	}

	void AnimationContainer::Tick(bool tick)
	{
		if (HasStateAndAnimation())
			_animationStates[_currentState].Tick(tick);

		Update();
	}

	void AnimationContainer::Update()
	{
		auto it = _animationStates.find(_currentState);
		if (it == _animationStates.end())
			_currentAnimation = nullptr;
		else
			_currentAnimation = &((*it).second);
	}

	void AnimationContainer::SetStateAnimation(std::string state, sf::Animation animation)
	{
		_animationStates[state] = animation;
		_animationStates[state].RefreshTexture();
	}

	void AnimationContainer::SetAnimationStates(std::map<std::string, sf::Animation> animationStates)
	{
		_animationStates = animationStates;
		for (auto iter = _animationStates.begin(); iter != _animationStates.end(); ++iter)
			iter->second.RefreshTexture();
	}

	void AnimationContainer::SetCurrentState(std::string state)
	{
		_currentState = state;
	}

	void AnimationContainer::RenameState(std::string oldName, std::string newName)
	{
		auto it = _animationStates.find(oldName);
		if (it != _animationStates.end()) {
			std::swap(_animationStates[newName], it->second);
			_animationStates.erase(oldName);

			if (_currentState == oldName)
				_currentState = newName;
		}
	}

	void AnimationContainer::RemoveState(std::string state)
	{
		_animationStates.erase(state);
		if (_currentState == state)
		{
			if (_animationStates.size() > 0)
				_currentState = _animationStates.begin()->first;
			else
				_currentState = "";
		}
	}

	void AnimationContainer::ApplySetChangeFrameEvery(unsigned int ticks)
	{
		for (auto iter = _animationStates.begin(); iter != _animationStates.end(); ++iter)
			iter->second.SetChangeFrameEvery(ticks);
	}

	void AnimationContainer::ApplySetAnimationSpeed(float speed)
	{
		for (auto iter = _animationStates.begin(); iter != _animationStates.end(); ++iter)
			iter->second.SetAnimationSpeed(speed);
	}

	void AnimationContainer::ApplySetRepeated(bool repeateTexture)
	{
		for (auto iter = _animationStates.begin(); iter != _animationStates.end(); ++iter)
			iter->second.SetRepeated(repeateTexture);
	}

	void AnimationContainer::ApplySetSmooth(bool smooth)
	{
		for (auto iter = _animationStates.begin(); iter != _animationStates.end(); ++iter)
			iter->second.SetSmooth(smooth);
	}

	void AnimationContainer::ApplySetColor(const Color& color)
	{
		for (auto iter = _animationStates.begin(); iter != _animationStates.end(); ++iter)
			iter->second.SetColor(color);
	}

	void AnimationContainer::ApplySetOrigin(const Vector2f& origin)
	{
		for (auto iter = _animationStates.begin(); iter != _animationStates.end(); ++iter)
			iter->second.SetOrigin(origin);
	}

	void AnimationContainer::ApplySetOrigin(float x, float y)
	{
		for (auto iter = _animationStates.begin(); iter != _animationStates.end(); ++iter)
			iter->second.SetOrigin(x, y);
	}

	void AnimationContainer::ApplySetPosition(const Vector2f& position)
	{
		for (auto iter = _animationStates.begin(); iter != _animationStates.end(); ++iter)
			iter->second.SetPosition(position);
	}

	void AnimationContainer::ApplySetPosition(float x, float y)
	{
		for (auto iter = _animationStates.begin(); iter != _animationStates.end(); ++iter)
			iter->second.SetPosition(x, y);
	}

	void AnimationContainer::ApplySetRotation(float angle)
	{
		for (auto iter = _animationStates.begin(); iter != _animationStates.end(); ++iter)
			iter->second.SetRotation(angle);
	}

	void AnimationContainer::ApplySetScale(const Vector2f& factors)
	{
		for (auto iter = _animationStates.begin(); iter != _animationStates.end(); ++iter)
			iter->second.SetScale(factors);
	}

	void AnimationContainer::ApplySetScale(float factorX, float factorY)
	{
		for (auto iter = _animationStates.begin(); iter != _animationStates.end(); ++iter)
			iter->second.SetScale(factorX, factorY);
	}

	sf::Animation* AnimationContainer::GetStateAnimation(std::string state)
	{
		if (_animationStates.size() > 0 && _animationStates.count(state))
			return &_animationStates[state];
		else
			return nullptr;
	}

	std::map<std::string, sf::Animation>* AnimationContainer::GetAnimationStates()
	{
		return &_animationStates;
	}

	void AnimationContainer::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		if (_currentAnimation == nullptr)
			target.draw(_noSprite);
		else
			target.draw(*_currentAnimation);
	}
}
