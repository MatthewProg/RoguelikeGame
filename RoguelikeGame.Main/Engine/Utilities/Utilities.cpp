#include "Utilities.h"

void Utilities::Generate(unsigned int width, unsigned int height)
{
	std::vector<sf::Uint8> pixels;
	for (unsigned int i = 0; i < width * height; i++)
	{
		if (!(i / width >= height / 2) != !(i % width >= width / 2))
		{
			pixels.push_back(sf::Uint8(0));
			pixels.push_back(sf::Uint8(0));
			pixels.push_back(sf::Uint8(0));
			pixels.push_back(sf::Uint8(255));
		}
		else
		{
			pixels.push_back(sf::Uint8(255));
			pixels.push_back(sf::Uint8(0));
			pixels.push_back(sf::Uint8(192));
			pixels.push_back(sf::Uint8(255));
		}
	}

	sf::Image img;
	img.create(width, height, pixels.data());
	_noTxt16x16.loadFromImage(img);
}

sf::Texture* Utilities::NoTexture16x16()
{
	return &_noTxt16x16;
}

sf::VertexArray Utilities::GenerateVertexCircle(const sf::Vector2f& center, float radius, unsigned short points, sf::Color color, bool fill)
{
	sf::VertexArray circle;
	circle.resize(points + 1);
	circle.setPrimitiveType((fill) ? sf::PrimitiveType::TriangleFan : sf::PrimitiveType::LineStrip);

	for (size_t i = 0; i < points + (unsigned int)1; i++)
	{
		circle[i].position = MathHelper::GetPointFromAngle(center, i * (360.F / (float)points), radius);
		circle[i].color = color;
	}
	circle[points].position = circle[0].position;
	circle[points].color = circle[0].color;
	return circle;
}

Utilities* Utilities::GetInstance()
{
	if (_utilities == nullptr) {
		_utilities = new Utilities();
	}
	return _utilities;
}

Utilities* Utilities::_utilities = nullptr;
