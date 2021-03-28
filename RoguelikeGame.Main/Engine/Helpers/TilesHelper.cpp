#include "TilesHelper.h"


sf::Texture TilesHelper::GetStandardTile(const sf::Image& img, unsigned int width, unsigned int height, int id)
{
	sf::Texture output;

	auto size = img.getSize();
	if (size.x <= 0 || size.y <= 0 || height <= 0 || width <= 0)
	{
		Logger::GetInstance()->Log(Logger::LogType::ERROR, "GetStandardTile(), one of the parameters were equal to 0!");
		output = *Utilities::GetInstance()->NoTexture16x16();
		return output;
	}

	int cols = size.x / width;

	int row = id / cols;
	int col = id % cols;

	if (!output.loadFromImage(img, sf::IntRect(col * width, row * height, width, height)))
		Logger::GetInstance()->Log(Logger::LogType::ERROR, "Unable to get tile!");
	return output;
}

sf::IntRect TilesHelper::GetTileRect(const sf::Vector2u& imgSize, unsigned int width, unsigned int height, int id)
{
	if (imgSize.x <= 0 || imgSize.y <= 0 || height <= 0 || width <= 0)
		return sf::IntRect(0, 0, 16, 16);

	int cols = imgSize.x / width;

	int row = id / cols;
	int col = id % cols;
	if (imgSize.x < (col * width) + width || imgSize.y < (row * height) + height)
		return sf::IntRect(0, 0, 16, 16);

	return sf::IntRect(col * width, row * height, width, height);
}

sf::Texture TilesHelper::GetCustomTile(const sf::Image& img, unsigned int left, unsigned int top, unsigned int width, unsigned int height)
{
	sf::Texture output;

	auto size = img.getSize();
	if (size.x <= 0 || size.y <= 0 || height <= 0 || width <= 0 || top <= 0 || left <= 0)
	{
		Logger::GetInstance()->Log(Logger::LogType::ERROR, "GetCustomTile(), one of the parameters were equal to 0!");
		output = *Utilities::GetInstance()->NoTexture16x16();
		return output;
	}

	if (left + width > size.x || top + height > size.y)
		Logger::GetInstance()->Log(Logger::LogType::ERROR, "Passed offset and size parameters exceed img size!");
	if (!output.loadFromImage(img, sf::IntRect(left, top, width, height)))
		Logger::GetInstance()->Log(Logger::LogType::ERROR, "Unable to get tile!");
	
	return output;
}
