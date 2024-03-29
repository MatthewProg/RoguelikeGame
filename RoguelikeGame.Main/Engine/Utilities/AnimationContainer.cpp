#include "AnimationContainer.h"

namespace sf
{
	bool AnimationContainer::HasStateAndAnimation()
	{
		if (_animationStates.size() == 0 || _currentState == "")
			return false;
		if (_animationStates.count(_currentState) == 0)
			return false;
		return true;
	}

	AnimationContainer::AnimationContainer()
	{
		_noTexture = Utilities::GetInstance()->NoTexture16x16();

		_currentState = "";
		_currentAnimation = nullptr;

		_smoothChangeState = "";
		_smoothChangePrevLoop = true;

		_noTextureVertex.resize(4);
		_noTextureVertex.setPrimitiveType(sf::Quads);
		_noTextureVertex[0].position = sf::Vector2f(0, 0);
		_noTextureVertex[1].position = sf::Vector2f(0, 16);
		_noTextureVertex[2].position = sf::Vector2f(16, 16);
		_noTextureVertex[3].position = sf::Vector2f(16, 0);
		_noTextureVertex[0].texCoords = sf::Vector2f(0, 0);
		_noTextureVertex[1].texCoords = sf::Vector2f(0, 16);
		_noTextureVertex[2].texCoords = sf::Vector2f(16, 16);
		_noTextureVertex[3].texCoords = sf::Vector2f(16, 0);
	}

	void AnimationContainer::Tick(bool tick)
	{
		if (HasStateAndAnimation())
		{
			_currentAnimation->Tick(tick);

			if (_smoothChangeState != "" && _currentAnimation->IsEnded())
			{
				_currentAnimation->SetLoop(_smoothChangePrevLoop);
				SetCurrentState(_smoothChangeState);		
			}
		}

	}

	void AnimationContainer::UpdateCurrentAnimationPtr()
	{
		auto it = _animationStates.find(_currentState);
		if (it == _animationStates.end())
			_currentAnimation = nullptr;
		else
			_currentAnimation = &((*it).second);
	}

	void AnimationContainer::SetStateAnimation(const std::string& state, const sf::Animation& animation)
	{
		_animationStates[state] = animation;
	}

	void AnimationContainer::SetAnimationStates(const std::map<std::string, sf::Animation>& animationStates)
	{
		_animationStates = animationStates;
	}

	void AnimationContainer::SetCurrentState(const std::string& state)
	{
		if (_currentState == state) return;

		if (_smoothChangeState != "")
		{
			if(_currentAnimation != nullptr)
				_currentAnimation->SetLoop(_smoothChangePrevLoop);
			_smoothChangeState = "";
		}

		_currentState = state;

		if (HasStateAndAnimation())
		{
			UpdateCurrentAnimationPtr();
			if (_currentAnimation != nullptr)
			{
				_currentAnimation->Reset();
				_currentAnimation->Start();
			}
		}
	}

	void AnimationContainer::SmoothStateChange(const std::string& state)
	{
		if(_smoothChangeState == "") //Not overriding real one
			_smoothChangePrevLoop = _currentAnimation->GetLoop();

		_smoothChangeState = state;
		_currentAnimation->SetLoop(false);
	}

	void AnimationContainer::RenameState(const std::string& oldName, const std::string& newName)
	{
		auto it = _animationStates.find(oldName);
		if (it != _animationStates.end()) {
			std::swap(_animationStates[newName], it->second);
			_animationStates.erase(oldName);

			if (_currentState == oldName)
				_currentState = newName;
		}
	}

	void AnimationContainer::RemoveState(const std::string& state)
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

	void AnimationContainer::ApplySetColor(const Color& color)
	{
		for (auto iter = _animationStates.begin(); iter != _animationStates.end(); ++iter)
			for (size_t i = 0; i < iter->second.GetNoOfFrames(); i++)
				iter->second.SetFrameColor(i, color);

		_noTextureVertex[0].color = color;
		_noTextureVertex[1].color = color;
		_noTextureVertex[2].color = color;
		_noTextureVertex[3].color = color;
	}

	void AnimationContainer::ApplySetOrigin(const Vector2f& origin)
	{
		for (auto iter = _animationStates.begin(); iter != _animationStates.end(); ++iter)
			iter->second.setOrigin(origin);

		_noTextureTransform.setOrigin(origin);
	}

	void AnimationContainer::ApplySetOrigin(float x, float y)
	{
		for (auto iter = _animationStates.begin(); iter != _animationStates.end(); ++iter)
			iter->second.setOrigin(x, y);

		_noTextureTransform.setOrigin(x, y);
	}

	void AnimationContainer::ApplySetPosition(const Vector2f& position)
	{
		for (auto iter = _animationStates.begin(); iter != _animationStates.end(); ++iter)
			iter->second.setPosition(position);

		_noTextureTransform.setPosition(position);
	}

	void AnimationContainer::ApplySetPosition(float x, float y)
	{
		for (auto iter = _animationStates.begin(); iter != _animationStates.end(); ++iter)
			iter->second.setPosition(x, y);

		_noTextureTransform.setPosition(x, y);
	}

	void AnimationContainer::ApplySetRotation(float angle)
	{
		for (auto iter = _animationStates.begin(); iter != _animationStates.end(); ++iter)
			iter->second.setRotation(angle);

		_noTextureTransform.setRotation(angle);
	}

	void AnimationContainer::ApplySetScale(const Vector2f& factors)
	{
		for (auto iter = _animationStates.begin(); iter != _animationStates.end(); ++iter)
			iter->second.setScale(factors);

		_noTextureTransform.setScale(factors);
	}

	void AnimationContainer::ApplySetScale(float factorX, float factorY)
	{
		for (auto iter = _animationStates.begin(); iter != _animationStates.end(); ++iter)
			iter->second.setScale(factorX, factorY);

		_noTextureTransform.setScale(factorX, factorY);
	}

	void AnimationContainer::ApplyFlipHorizontally()
	{
		for (auto iter = _animationStates.begin(); iter != _animationStates.end(); ++iter)
			iter->second.FlipHorizontally();
	}

	void AnimationContainer::ApplyFlipVertically()
	{
		for (auto iter = _animationStates.begin(); iter != _animationStates.end(); ++iter)
			iter->second.FlipVertically();
	}

	void AnimationContainer::ApplySetHorizontalFlip(bool flip)
	{
		for (auto iter = _animationStates.begin(); iter != _animationStates.end(); ++iter)
			iter->second.SetHorizontalFlip(flip);
	}

	void AnimationContainer::ApplySetVerticalFlip(bool flip)
	{
		for (auto iter = _animationStates.begin(); iter != _animationStates.end(); ++iter)
			iter->second.SetVerticalFlip(flip);
	}

	sf::Animation* AnimationContainer::GetStateAnimation(const std::string& state)
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

	const std::string& AnimationContainer::GetCurrentState() const
	{
		return _currentState;
	}

	void AnimationContainer::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		if (_currentAnimation == nullptr)
		{
			states.texture = _noTexture;
			states.transform *= _noTextureTransform.getTransform();
			target.draw(_noTextureVertex);
		}
		else
			target.draw(*_currentAnimation);
	}
}
