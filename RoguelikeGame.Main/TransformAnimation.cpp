#include "TransformAnimation.h"

namespace sf
{
	void TransformAnimation::SetNextTransformDiff()
	{
		if ((long)_currentTransformIndex + 1 > (long)_transforms.size() - 1)
		{
			if(_loop)
			{
				Reset();
				_paused = false;
				_isPlaying = true;
				SetNextTransformDiff();
				return;
			}
			_paused = true;
			_isPlaying = false;
			_isEnded = true;
			_currentPosDiff = sf::Vector2f(0, 0);
			_currentScaleDiff = sf::Vector2f(0, 0);
			_currentRotationDiff = 0;
			_currentTransformTicks = 0;
			return;
		}

		auto prev = &std::get<0>(_transforms[_currentTransformIndex]);
		auto next = &std::get<0>(_transforms[_currentTransformIndex + 1]);
		
		_currentPosDiff = next->getPosition() - prev->getPosition();
		_currentRotationDiff = next->getRotation() - prev->getRotation();
		_currentScaleDiff = next->getScale() - prev->getScale();

		_currentTransformTicks = std::get<1>(_transforms[_currentTransformIndex + 1]);

		_prevStart = prev;

		_currentTransformIndex++;
	}

	void TransformAnimation::UpdateTarget(float delta)
	{
		if (delta > _elapsedTime - (int)_elapsedTime) //correction
		{
			_target->setRotation(_prevStart->getRotation() + (_currentRotationDiff / _currentTransformTicks * (int)_elapsedTime));
			
			auto x = _prevStart->getPosition().x + (_currentPosDiff.x / _currentTransformTicks * (int)_elapsedTime);
			auto y = _prevStart->getPosition().y + (_currentPosDiff.y / _currentTransformTicks * (int)_elapsedTime);
			_target->setPosition(x, y);

			auto factorX = _prevStart->getScale().x + (_currentScaleDiff.x / _currentTransformTicks * (int)_elapsedTime);
			auto factorY = _prevStart->getScale().y + (_currentScaleDiff.y / _currentTransformTicks * (int)_elapsedTime);
			_target->setScale(factorX, factorY);

			delta = _elapsedTime - (int)_elapsedTime;
		}


		sf::Vector2f pos(_currentPosDiff);
		pos.x = (pos.x / (float)_currentTransformTicks) * delta;
		pos.y = (pos.y / (float)_currentTransformTicks) * delta;

		_target->move(pos);

		sf::Vector2f scale(_currentScaleDiff);
		scale.x = 1 + (scale.x / (float)_currentTransformTicks) * delta;
		scale.y = 1 + (scale.y / (float)_currentTransformTicks) * delta;

		_target->scale(scale);

		_target->rotate((_currentRotationDiff / (float)_currentTransformTicks) * delta);

		_target->setOrigin(_prevStart->getOrigin());
	}

	TransformAnimation::TransformAnimation()
	{
		_currentTransformIndex = 0;
		_elapsedTime = 0;

		_transforms.clear();
		_target = nullptr;

		_paused = true;
		_loop = true;
		_isPlaying = false;
		_isEnded = false;

		_currentTransformTicks = 0;
		_prevStart = nullptr;

		_currentPosDiff = sf::Vector2f(0, 0);
		_currentScaleDiff = sf::Vector2f(0, 0);
		_currentRotationDiff = 0;
	}

	TransformAnimation::~TransformAnimation()
	{
	}

	void TransformAnimation::SetTarget(sf::Transformable* target)
	{
		_target = target;
	}

	void TransformAnimation::Update(float deltaTime)
	{
		if (_paused == true)
			return;

		_elapsedTime += deltaTime;

		if (_elapsedTime > _currentTransformTicks)
		{
			SetNextTransformDiff();
			_elapsedTime = 0;
		}

		if (_paused == false)
			UpdateTarget(deltaTime);
	}

	void TransformAnimation::AddTransform(sf::Transformable transform, uint16_t ticks)
	{
		_transforms.push_back(std::make_tuple(transform, ticks));
	}

	void TransformAnimation::AddTransform(std::tuple<sf::Transformable, uint16_t> tuple)
	{
		_transforms.push_back(tuple);
	}

	void TransformAnimation::RemoveTransform(uint16_t transformIndex)
	{
		_transforms.erase(_transforms.begin() + transformIndex);
	}

	void TransformAnimation::ClearTransforms()
	{
		_transforms.clear();
		_currentTransformIndex = 0;
		_elapsedTime = 0;
		_currentTransformTicks = 0;
	}

	void TransformAnimation::SetTransformTuple(uint16_t transformIndex, std::tuple<sf::Transformable, uint16_t> tuple)
	{
		if (_transforms.size() < transformIndex)
			_transforms[transformIndex] = tuple;
	}

	void TransformAnimation::SetTransformTransform(uint16_t transformIndex, sf::Transformable transform)
	{
		if (_transforms.size() < transformIndex)
			std::get<0>(_transforms[transformIndex]) = transform;
	}

	void TransformAnimation::SetTransformTime(uint16_t transformIndex, uint16_t ticks)
	{
		if (_transforms.size() < transformIndex)
			std::get<1>(_transforms[transformIndex]) = ticks;
	}

	void TransformAnimation::SetLoop(bool loop)
	{
		_loop = loop;
	}

	std::tuple<sf::Transformable, uint16_t> TransformAnimation::GetTransformTuple(uint16_t transformIndex)
	{
		if (_transforms.size() < transformIndex)
			return _transforms[transformIndex];
		return std::tuple<sf::Transformable, uint16_t>();
	}

	sf::Transformable TransformAnimation::GetTransformTransform(uint16_t transformIndex)
	{
		if (_transforms.size() < transformIndex)
			return std::get<0>(_transforms[transformIndex]);
		return sf::Transformable();
	}

	uint16_t TransformAnimation::GetTransformTicks(uint16_t transformIndex)
	{
		if (_transforms.size() < transformIndex)
			return std::get<1>(_transforms[transformIndex]);
		return uint16_t();
	}

	bool TransformAnimation::GetLoop()
	{
		return _loop;
	}

	uint16_t TransformAnimation::GetCurrentTransformIndex()
	{
		return _currentTransformIndex;
	}

	size_t TransformAnimation::GetNoOfTransforms()
	{
		return _transforms.size();
	}

	void TransformAnimation::Start()
	{
		_paused = false;
		_isPlaying = true;
		_isEnded = false;
	}

	void TransformAnimation::Pause()
	{
		_paused = true;
		_isPlaying = false;
	}

	void TransformAnimation::Reset()
	{
		_elapsedTime = 0;
		_currentTransformIndex = 0;
		_currentTransformTicks = 0;
		_paused = true;
		_isPlaying = false;
		_isEnded = false;
	}
	bool TransformAnimation::IsPlaying()
	{
		return false;
	}
	bool TransformAnimation::IsEnded()
	{
		return _isEnded;
	}
}