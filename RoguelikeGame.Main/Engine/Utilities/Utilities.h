#pragma once

#include <vector>

#include "SFML/Graphics/VertexArray.hpp"
#include "SFML/Graphics/Texture.hpp"
#include "SFML/Graphics/Image.hpp"
#include "SFML/Window/Event.hpp"

#include "../Helpers/MathHelper.h"

class Utilities
{
protected:
	Utilities() { Generate(16, 16); _noTxt16x16.setRepeated(true); }
	static Utilities* _utilities;
private:
	sf::Texture _noTxt16x16;
	void Generate(unsigned int width, unsigned int height);
public:
	sf::Texture* NoTexture16x16();
	static sf::VertexArray GenerateVertexCircle(const sf::Vector2f& center, float radius, unsigned short points, sf::Color color = sf::Color::White, bool fill = false);

	Utilities(Utilities& other) = delete;
	void operator=(const Utilities&) = delete;

	static Utilities* GetInstance();
};

struct EventKeyMapKey : public sf::Event::KeyEvent
{	
	bool operator== (const EventKeyMapKey& other) const
	{
		return (alt == other.alt &&
				code == other.code &&
				control == other.control &&
				shift == other.shift &&
				system == other.system);
	}
};

struct EventKeyMapKeyHasher
{
	std::size_t operator()(const EventKeyMapKey& k) const
	{
		using std::size_t;
		using std::hash;
		using std::string;

		std::size_t output = 17;
		output = output * 31 + hash<bool>()(k.alt);
		output = output * 31 + hash<bool>()(k.control);
		output = output * 31 + hash<bool>()(k.shift);
		output = output * 31 + hash<bool>()(k.system);
		output = output * 31 + hash<int>()(k.code);
		return output;
	}
};

template<typename T>
struct Vector2MapKey : public sf::Vector2<T>
{
	Vector2MapKey() : sf::Vector2<T>()
	{ 
		;
	}

	Vector2MapKey(const sf::Vector2<T>& other)
	{
		this->x = other.x;
		this->y = other.y;
	}

	Vector2MapKey(const Vector2MapKey<T>& other)
	{
		this->x = other.x;
		this->y = other.y;
	}
	
	bool operator== (const Vector2MapKey<T>& other) const
	{
		return (this->x == other.x && this->y == other.y);
	}
};

template<typename T>
struct Vector2MapKeyHasher
{
	std::size_t operator()(const Vector2MapKey<T>& k) const
	{
		using std::size_t;
		using std::hash;
		using std::string;

		std::size_t output = 17;
		output = output * 31 + hash<T>()(k.x);
		output = output * 31 + hash<T>()(k.y);
		return output;
	}
};
