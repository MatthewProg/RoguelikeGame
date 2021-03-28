#pragma once

#include <vector>
#include <tuple>

#include "SFML/Graphics/Transformable.hpp"

namespace sf
{
	class TransformAnimation
	{
	private:
		uint16_t _currentTransformIndex;
		float _elapsedTime;

		std::vector<std::tuple<sf::Transformable, uint16_t>> _transforms;
		sf::Transformable* _target;

		bool _paused;
		bool _loop;
		bool _isPlaying;
		bool _isEnded;

		sf::Transformable* _prevStart;
		sf::Vector2f _currentPosDiff;
		sf::Vector2f _currentScaleDiff;
		float _currentRotationDiff;
		uint16_t _currentTransformTicks;

		void SetNextTransformDiff();
		void UpdateTarget(float delta);
	public:
		TransformAnimation();
		~TransformAnimation();

		void SetTarget(sf::Transformable* target);
		void Update(float deltaTime);

		void AddTransform(const sf::Transformable& transform, uint16_t ticks);
		void AddTransform(const std::tuple<sf::Transformable, uint16_t>& tuple);
		void RemoveTransform(uint16_t transformIndex);
		void ClearTransforms();

		void SetTransformTuple(uint16_t transformIndex, const std::tuple<sf::Transformable, uint16_t>& tuple);
		void SetTransformTransform(uint16_t transformIndex, const sf::Transformable& transform);
		void SetTransformTime(uint16_t transformIndex, uint16_t ticks);
		void SetLoop(bool loop);

		const std::tuple<sf::Transformable, uint16_t>* GetTransformTuple(uint16_t transformIndex) const;
		sf::Transformable GetTransformTransform(uint16_t transformIndex) const;
		uint16_t GetTransformTicks(uint16_t transformIndex) const;
		bool GetLoop() const;

		uint16_t GetCurrentTransformIndex() const;
		size_t GetNoOfTransforms() const;

		void Start();
		void Pause();
		void Reset();

		bool IsPlaying() const;
		bool IsEnded() const;
	};
}

