#include "TilesManager.h"


sf::Texture TilesManager::GetStandardTile(sf::Image img, int width, int height, int id)
{
	sf::Texture output;

	auto size = img.getSize();
	int cols = size.x / width;
	int rows = size.y / height;

	int row = id / cols;
	int col = id % cols;

	if (!output.loadFromImage(img, sf::IntRect(col * width, row * height, width, height)))
		Logger::GetInstance()->Log(Logger::LogType::ERROR, "Unable to get tile!");
	return output;
}

sf::IntRect TilesManager::GetTileRect(sf::Vector2u imgSize, int width, int height, int id)
{
	if (imgSize.x <= 0 || imgSize.y <= 0 || height <= 0 || width <= 0)
	{
		Logger::GetInstance()->Log(Logger::LogType::WARNING, "GetTileRect(), one of the parameters were equal to 0!");
		return sf::IntRect(0, 0, 16, 16);
	}
	int cols = imgSize.x / width;
	int rows = imgSize.y / height;

	int row = id / cols;
	int col = id % cols;
	return sf::IntRect(col * width, row * height, width, height);
}

sf::Texture TilesManager::GetCustomTile(sf::Image img, int left, int top, int width, int height)
{
	sf::Texture output;

	auto size = img.getSize();

	if (left + width > size.x || top + height > size.y)
		Logger::GetInstance()->Log(Logger::LogType::ERROR, "Passed offset and size parameters exceed img size!");
	if (!output.loadFromImage(img, sf::IntRect(left, top, width, height)))
		Logger::GetInstance()->Log(Logger::LogType::ERROR, "Unable to get tile!");
	
	return output;
}
