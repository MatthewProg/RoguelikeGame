#include "TilesHelper.h"


sf::Texture TilesHelper::GetStandardTile(sf::Image img, int width, int height, int id)
{
	sf::Texture output;

	auto size = img.getSize();
	if (size.x <= 0 || size.y <= 0 || height <= 0 || width <= 0)
	{
		Logger::GetInstance()->Log(Logger::LogType::ERROR, "GetStandardTile(), one of the parameters were equal to 0!");
		//output = sf::Texture(*Utilities::GetInstance()->NoTexture16x16());
		return sf::Texture();
	}

	int cols = size.x / width;
	int rows = size.y / height;

	int row = id / cols;
	int col = id % cols;

	if (!output.loadFromImage(img, sf::IntRect(col * width, row * height, width, height)))
		Logger::GetInstance()->Log(Logger::LogType::ERROR, "Unable to get tile!");
	return output;
}

sf::IntRect TilesHelper::GetTileRect(sf::Vector2u imgSize, int width, int height, int id)
{
	if (imgSize.x <= 0 || imgSize.y <= 0 || height <= 0 || width <= 0)
	{
		//Logger::GetInstance()->Log(Logger::LogType::WARNING, "GetTileRect(), one of the parameters were equal to 0!", true);
		return sf::IntRect(0, 0, 16, 16);
	}
	int cols = imgSize.x / width;
	int rows = imgSize.y / height;

	int row = id / cols;
	int col = id % cols;
	if (imgSize.x < (col * width) + width || imgSize.y < (row * height) + height)
	{
		//Logger::GetInstance()->Log(Logger::LogType::WARNING, "GetTileRect(), tile exceed img size!", true);
		return sf::IntRect(0, 0, 16, 16);
	}
	return sf::IntRect(col * width, row * height, width, height);
}

sf::Texture TilesHelper::GetCustomTile(sf::Image img, int left, int top, int width, int height)
{
	sf::Texture output;

	auto size = img.getSize();
	if (size.x <= 0 || size.y <= 0 || height <= 0 || width <= 0 || top <= 0 || left <= 0)
	{
		Logger::GetInstance()->Log(Logger::LogType::ERROR, "GetCustomTile(), one of the parameters were equal to 0!");
		//output = sf::Texture(*Utilities::GetInstance()->NoTexture16x16());
		return sf::Texture();
	}

	if (left + width > size.x || top + height > size.y)
		Logger::GetInstance()->Log(Logger::LogType::ERROR, "Passed offset and size parameters exceed img size!");
	if (!output.loadFromImage(img, sf::IntRect(left, top, width, height)))
		Logger::GetInstance()->Log(Logger::LogType::ERROR, "Unable to get tile!");
	
	return output;
}
