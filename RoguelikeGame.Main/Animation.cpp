#include "Animation.h"


sf::Animation::Animation()
{
	_changeEveryTicks = 20;
	_animationSpeed = 1.0F;

	_currentTick = 0;
	_currentFrame = 0;

	_mainSprite.setTexture(_mainTexture);
}

sf::Animation::~Animation()
{
}

void sf::Animation::NextFrame()
{
	_currentFrame++;
	if (_currentFrame > _rectFrames.size() - 1)
		_currentFrame = 0;

	_mainSprite.setTextureRect(_rectFrames[_currentFrame]);
}

void sf::Animation::Tick(bool tick)
{
	if (tick == false) return;

	if (_currentTick > _changeEveryTicks * _animationSpeed)
	{
		_currentTick = 0;
		NextFrame();
	}
	else
	{
		_currentTick++;
		return;
	}
}

void sf::Animation::AddNewFrame(sf::IntRect rect)
{
	_rectFrames.push_back(rect);
	NextFrame();
}

void sf::Animation::RemoveFrame(int index)
{
	_rectFrames.erase(_rectFrames.begin() + index);
}

void sf::Animation::RemoveAllFrames()
{
	_rectFrames.clear();
}

void sf::Animation::GetNoOfFrames()
{
	_rectFrames.size();
}

void sf::Animation::SetFrames(std::vector<sf::IntRect> frames)
{
	_rectFrames.clear();
	_rectFrames = frames;
	NextFrame();
}

void sf::Animation::SetChangeFrameEvery(unsigned int ticks)
{
	_changeEveryTicks = ticks;
}

void sf::Animation::SetAnimationSpeed(float speed)
{
	_animationSpeed = speed;
}

unsigned int sf::Animation::GetChangeFrameEvery()
{
	return _changeEveryTicks;
}

float sf::Animation::GetAnimationSpeed()
{
	return _animationSpeed;
}

bool sf::Animation::LoadFromFile(const std::string& path)
{
	return _mainTexture.loadFromFile(path);
}

bool sf::Animation::LoadFromMemory(const void* data, size_t size)
{
	return _mainTexture.loadFromMemory(data, size);
}

bool sf::Animation::LoadFromStream(sf::InputStream& stream)
{
	return _mainTexture.loadFromStream(stream);
}

bool sf::Animation::LoadFromImage(const sf::Image& img, const sf::IntRect& area)
{
	return _mainTexture.loadFromImage(img, area);
}

void sf::Animation::SetRepeated(bool repeateTexture)
{
	_mainTexture.setRepeated(repeateTexture);
}

void sf::Animation::SetSmooth(bool smooth)
{
	_mainTexture.setSmooth(smooth);
}

bool sf::Animation::IsRepeated()
{
	return _mainTexture.isRepeated();
}

bool sf::Animation::IsSmooth()
{
	return _mainTexture.isSmooth();
}

void sf::Animation::SetColor(const Color& color)
{
	_mainSprite.setColor(color);
}

void sf::Animation::SetOrigin(const Vector2f& origin)
{
	_mainSprite.setOrigin(origin);
}

void sf::Animation::SetOrigin(float x, float y)
{
	_mainSprite.setOrigin(x, y);
}

void sf::Animation::SetPosition(const Vector2f& position)
{
	_mainSprite.setPosition(position);
}

void sf::Animation::SetPosition(float x, float y)
{
	_mainSprite.setPosition(x, y);
}

void sf::Animation::SetRotation(float angle)
{
	_mainSprite.setRotation(angle);
}

void sf::Animation::SetScale(const Vector2f& factors)
{
	_mainSprite.setScale(factors);
}

void sf::Animation::SetScale(float factorX, float factorY)
{
	_mainSprite.setScale(factorX, factorY);
}

sf::Color sf::Animation::GetColor()
{
	return _mainSprite.getColor();
}

sf::Vector2f sf::Animation::GetOrigin()
{
	return _mainSprite.getOrigin();
}

sf::Vector2f sf::Animation::GetPosition()
{
	return _mainSprite.getPosition();
}

float sf::Animation::GetRotation()
{
	return _mainSprite.getRotation();
}

sf::Vector2f sf::Animation::GetScale()
{
	return _mainSprite.getScale();
}

sf::FloatRect sf::Animation::GetGlobalBounds()
{
	return _mainSprite.getGlobalBounds();
}

sf::FloatRect sf::Animation::GetLocalBounds()
{
	return _mainSprite.getLocalBounds();
}

sf::Transform sf::Animation::GetTransform()
{
	return _mainSprite.getTransform();
}

void sf::Animation::draw(RenderTarget& target, RenderStates states) const
{
	target.draw(_mainSprite);
}
