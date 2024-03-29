#include "Animation.h"


sf::Animation::Animation()
{
	_changeEveryTicks = 20;
	_animationSpeed = 1.0F;

	_currentTick = 0;
	_currentFrame = 0;

	_noTexture = Utilities::GetInstance()->NoTexture16x16();
	_texture = nullptr;

	_switchLeftRight = false;
	_switchUpDown = false;

	_loop = true;
	_paused = false;
	_ended = false;
	_frameSwitched = true;

	_vertices.setPrimitiveType(sf::Quads);
	_vertices.resize(4);
}

void sf::Animation::Start()
{
	_paused = false;
}

void sf::Animation::Pause()
{
	_paused = true;
}

void sf::Animation::Reset()
{
	_ended = false;
	_paused = true;
	_currentFrame = 0;
	UpdateVertices();
}

void sf::Animation::SetTexture(sf::Texture* texture)
{
	_texture = texture;
}

void sf::Animation::SetFrameColor(size_t frame, const sf::Color& color)
{
	if (frame >= _frameColor.size())
		return;
	_frameColor[frame] = color;
	UpdateVertices();
}

void sf::Animation::SetHorizontalFlip(bool flip)
{
	_switchLeftRight = flip;
	UpdateVertices();
}

void sf::Animation::SetVerticalFlip(bool flip)
{
	_switchUpDown = flip;
	UpdateVertices();
}

void sf::Animation::SetLoop(bool loop)
{
	_loop = loop;
}

const sf::Texture* sf::Animation::GetTexture() const
{
	return _texture;
}

sf::Color sf::Animation::GetFrameColor(size_t frame) const
{
	if(frame >= _frameColor.size())
		return {};
	return _frameColor[frame];
}

bool sf::Animation::GetHorizontalFlip() const
{
	return _switchLeftRight;
}

bool sf::Animation::GetVerticalFlip() const
{
	return _switchUpDown;
}

bool sf::Animation::GetLoop() const
{
	return _loop;
}

bool sf::Animation::IsPaused() const
{
	return _paused;
}

bool sf::Animation::IsEnded() const
{
	return _ended;
}

bool sf::Animation::IsFrameSwitched() const
{
	return _frameSwitched;
}

bool sf::Animation::IsTextureRepeated() const
{
	return _texture->isRepeated();
}

bool sf::Animation::IsTextureSmooth() const
{
	return _texture->isSmooth();
}

const sf::IntRect& sf::Animation::GetCurrentRect() const
{
	return _rectFrames[_currentFrame];
}

void sf::Animation::UpdateVertices()
{
	_vertices[0].position = sf::Vector2f(0, 0);
	_vertices[1].position = sf::Vector2f((float)_rectFrames[_currentFrame].width, 0);
	_vertices[2].position = sf::Vector2f((float)_rectFrames[_currentFrame].width, (float)_rectFrames[_currentFrame].height);
	_vertices[3].position = sf::Vector2f(0, (float)_rectFrames[_currentFrame].height);

	auto texRect = _rectFrames[_currentFrame];
	if (_texture == nullptr || _texture->getSize().x == 0)
		texRect = sf::IntRect(0, 0, 16, 16);

	_vertices[0].texCoords = sf::Vector2f((float)texRect.left, (float)texRect.top);
	_vertices[1].texCoords = sf::Vector2f((float)texRect.left + (float)texRect.width, (float)texRect.top);
	_vertices[2].texCoords = sf::Vector2f((float)texRect.left + (float)texRect.width, (float)texRect.top + (float)texRect.height);
	_vertices[3].texCoords = sf::Vector2f((float)texRect.left, (float)texRect.top + (float)texRect.height);

	if (_switchLeftRight) //horizontal
	{
		auto tmp = _vertices[0].texCoords;
		_vertices[0].texCoords = _vertices[1].texCoords;
		_vertices[1].texCoords = tmp;

		tmp = _vertices[2].texCoords;
		_vertices[2].texCoords = _vertices[3].texCoords;
		_vertices[3].texCoords = tmp;
	}
	if (_switchUpDown) //vertical
	{
		auto tmp = _vertices[0].texCoords;
		_vertices[0].texCoords = _vertices[3].texCoords;
		_vertices[3].texCoords = tmp;

		tmp = _vertices[1].texCoords;
		_vertices[1].texCoords = _vertices[2].texCoords;
		_vertices[2].texCoords = tmp;
	}

	_vertices[0].color = _frameColor[_currentFrame];
	_vertices[1].color = _frameColor[_currentFrame];
	_vertices[2].color = _frameColor[_currentFrame];
	_vertices[3].color = _frameColor[_currentFrame];
}

void sf::Animation::NextFrame()
{
	auto prev_frame = _currentFrame;
	_currentFrame++;
	if (_currentFrame > _rectFrames.size() - 1)
	{
		if(_loop)
			_currentFrame = 0;
		else
		{
			_currentFrame--;
			_ended = true;
			_paused = true;
		}
	}
	if (_currentFrame != prev_frame)
		_frameSwitched = true;
}

void sf::Animation::Tick(bool tick)
{
	_frameSwitched = false;
	if (tick == false || _paused == true || (_ended && !_loop)) return;

	UpdateVertices();

	if ((float)_currentTick > (float)_changeEveryTicks * _animationSpeed)
	{
		_currentTick = 0U;
		NextFrame();
	}
	else
	{
		_currentTick++;
		return;
	}
}

void sf::Animation::AddNewFrame(const sf::IntRect& rect)
{
	_rectFrames.push_back(rect);
	_frameColor.emplace_back(255, 255, 255, 255);
	Reset();
}

void sf::Animation::RemoveFrame(size_t index)
{
	_rectFrames.erase(_rectFrames.begin() + index);
	_frameColor.erase(_frameColor.begin() + index);
}

void sf::Animation::RemoveAllFrames()
{
	_rectFrames.clear();
	_frameColor.clear();
}

size_t sf::Animation::GetNoOfFrames() const
{
	return _rectFrames.size();
}

void sf::Animation::SetFrames(const std::vector<sf::IntRect>& frames)
{
	_rectFrames.clear();
	_rectFrames = frames;
	_frameColor.clear();
	for (size_t i = 0; i < _rectFrames.size(); i++)
		_frameColor.emplace_back(255, 255, 255, 255);
	Reset();
}

sf::Transformable* sf::Animation::ExternalTransform()
{
	return &_externalTransform;
}

void sf::Animation::FlipHorizontally()
{
	_switchLeftRight = !_switchLeftRight;
	UpdateVertices();
}

void sf::Animation::FlipVertically()
{
	_switchUpDown = !_switchUpDown;
	UpdateVertices();
}

void sf::Animation::SetChangeFrameEvery(unsigned int ticks)
{
	_changeEveryTicks = ticks;
}

void sf::Animation::SetAnimationSpeed(float speed)
{
	_animationSpeed = speed;
}

unsigned int sf::Animation::GetChangeFrameEvery() const
{
	return _changeEveryTicks;
}

float sf::Animation::GetAnimationSpeed() const
{
	return _animationSpeed;
}

sf::FloatRect sf::Animation::GetGlobalBounds() const
{
	sf::FloatRect output;
	output.height = (float)_rectFrames[_currentFrame].height;
	output.width = (float)_rectFrames[_currentFrame].width;
	output.left = (float)_rectFrames[_currentFrame].left;
	output.top = (float)_rectFrames[_currentFrame].top;
	return getTransform().transformRect(output);
}

sf::FloatRect sf::Animation::GetLocalBounds() const
{
	return sf::FloatRect(0, 0, (float)_rectFrames[_currentFrame].width, (float)_rectFrames[_currentFrame].height);
}


void sf::Animation::draw(RenderTarget& target, RenderStates states) const
{
	if (_texture == nullptr || _texture->getSize() == sf::Vector2u())
		states.texture = _noTexture;
	else
		states.texture = _texture;

	states.transform *= getTransform();
	states.transform *= _externalTransform.getTransform();

	target.draw(_vertices, states);
}
