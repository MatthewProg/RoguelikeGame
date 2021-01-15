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

		void AddTransform(sf::Transformable transform, uint16_t ticks);
		void AddTransform(std::tuple<sf::Transformable, uint16_t> tuple);
		void RemoveTransform(uint16_t transformIndex);
		void ClearTransforms();

		void SetTransformTuple(uint16_t transformIndex, std::tuple<sf::Transformable, uint16_t> tuple);
		void SetTransformTransform(uint16_t transformIndex, sf::Transformable transform);
		void SetTransformTime(uint16_t transformIndex, uint16_t ticks);
		void SetLoop(bool loop);

		std::tuple<sf::Transformable, uint16_t> GetTransformTuple(uint16_t transformIndex);
		sf::Transformable GetTransformTransform(uint16_t transformIndex);
		uint16_t GetTransformTicks(uint16_t transformIndex);
		bool GetLoop();

		uint16_t GetCurrentTransformIndex();
		size_t GetNoOfTransforms();

		void Start();
		void Pause();
		void Reset();

		bool IsPlaying();
		bool IsEnded();
	};
}

