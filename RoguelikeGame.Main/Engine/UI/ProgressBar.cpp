#include "ProgressBar.h"

ProgressBar::ProgressBar()
{
	_progressBarSteps.clear();
	_backgroundLayers.clear();

	_currentValue = 3.F;
	_step = 0.f;
	_maxValue = 3.F;

	_prevValue = _currentValue;
	_prevMaxValue = _maxValue;

	_keyboardInput = true;
	_mouseInput = true;
}

ProgressBar::ProgressBar(ProgressBar& other) : UIElement(other)
{
	_currentValue = other._currentValue;
	_maxValue = other._maxValue;
	_step = other._step;
	_prevValue = other._prevValue;
	_prevMaxValue = other._prevMaxValue;
	_progressBarSteps = other._progressBarSteps;
	_backgroundLayers = other._backgroundLayers;
	_progressBarStepsPos = other._progressBarStepsPos;
}

void ProgressBar::SetCurrentValue(float value)
{
	_currentValue = value;
	if (_currentValue != _prevValue)
	{
		_prevValue = _currentValue;
		_sthChanged = true;
	}
}

void ProgressBar::SetMaxValue(float maxValue)
{
	_maxValue = maxValue;
	if(_maxValue != _prevMaxValue)
	{
		_prevMaxValue = _maxValue;
		_sthChanged = true;
	}
}

void ProgressBar::SetForceStep(float step)
{
	_step = step;
}

float ProgressBar::GetCurrentValue() const
{
	return _currentValue;
}

float ProgressBar::GetMaxValue() const
{
	return _maxValue;
}

float ProgressBar::GetStep() const
{
	if (_step != 0.f)
		return _step;
	else
		return (1.f / (float(_progressBarSteps.size())-1.f));
}

void ProgressBar::AddProgressBarStep(const sf::FloatRect& step, const std::string& textureName)
{
	_progressBarSteps.emplace_back(step, textureName);
	_sthChanged = true;
}

void ProgressBar::AddBackgroundLayer(const sf::FloatRect& layer, const std::string& textureName)
{
	_backgroundLayers.emplace_back(layer, textureName);
	_sthChanged = true;
}

void ProgressBar::ResetProgressBarSteps()
{
	_progressBarSteps.clear();
	_sthChanged = true;
}

void ProgressBar::ResetBackgroundLayers()
{
	_backgroundLayers.clear();
	_sthChanged = true;
}

void ProgressBar::ReplaceProgressBarStep(size_t index, const std::tuple<sf::FloatRect, std::string>& newTuple)
{
	if (_progressBarSteps.size() - 1 <= index)
	{
		_progressBarSteps[index] = newTuple;
		_sthChanged = true;
	}
}

void ProgressBar::ReplaceBackgroundLayer(size_t index, const std::tuple<sf::FloatRect, std::string>& newTuple)
{
	if (_backgroundLayers.size() - 1 <= index)
	{
		_backgroundLayers[index] = newTuple;
		_sthChanged = true;
	}
}

void ProgressBar::RemoveProgressBarStep(size_t index)
{
	if (_progressBarSteps.size() - 1 <= index)
	{
		_progressBarSteps.erase(_progressBarSteps.begin() + index);
		_sthChanged = true;
	}
}

void ProgressBar::RemoveBackgroundLayer(size_t index)
{
	if (_backgroundLayers.size() - 1 <= index)
	{
		_backgroundLayers.erase(_backgroundLayers.begin() + index);
		_sthChanged = true;
	}
}

void ProgressBar::SetProgressBarStepsPos(const sf::Vector2f& pos)
{
	_progressBarStepsPos = pos;
	_sthChanged = true;
}

size_t ProgressBar::GetNoOfProgressBarSteps() const
{
	return _progressBarSteps.size();
}

size_t ProgressBar::GetNoOfBackgroundLayers() const
{
	return _backgroundLayers.size();
}

std::tuple<sf::FloatRect, std::string> ProgressBar::GetProgressBarStep(size_t index) const
{
	if (_progressBarSteps.size() - 1 <= index)
		return _progressBarSteps[index];
	return std::tuple<sf::FloatRect, std::string>();
}

std::tuple<sf::FloatRect, std::string> ProgressBar::GetBackgroundLayer(size_t index) const
{
	if (_backgroundLayers.size() - 1 <= index)
		return _backgroundLayers[index];
	return std::tuple<sf::FloatRect, std::string>();
}

const sf::Vector2f& ProgressBar::GetProgressBarStepsPos() const
{
	return _progressBarStepsPos;
}

sf::FloatRect ProgressBar::GetProgressBarStepsGlobalBounds() const
{
	if (_progressBarSteps.size() == 0)
		return GetGlobalBounds();

	float width = 0.f;
	if(_step == 0.f)
		width = ceilf(_maxValue) * std::get<0>(_progressBarSteps[_progressBarSteps.size() - 1]).width;
	else
		width = ceilf((_maxValue / GetStep()) / float(_progressBarSteps.size()-1)) * std::get<0>(_progressBarSteps[_progressBarSteps.size() - 1]).width;
	auto endPos = _progressBarStepsPos + sf::Vector2f(width, std::get<0>(_progressBarSteps[0]).height);
	return getTransform().transformRect(sf::FloatRect(_progressBarStepsPos, endPos - _progressBarStepsPos));
}

void ProgressBar::ForceRedraw()
{
	sf::RenderStates rs;

	_render.clear(sf::Color::Transparent);

	//Draw background
	for (auto& layer : _backgroundLayers)
	{
		sf::Texture* texture = _texturesManager->GetTexture(std::get<1>(layer));
		if (texture == nullptr || texture->getSize().x == 0 || texture->getSize().y == 0)
			texture = _noTexture;

		rs.texture = texture;

		auto& rect = std::get<0>(layer);
		sf::VertexArray arr(sf::Quads, 4);
		arr[0].position = sf::Vector2f(0, 0);
		arr[1].position = sf::Vector2f(rect.width, 0);
		arr[2].position = sf::Vector2f(rect.width, rect.height);
		arr[3].position = sf::Vector2f(0, rect.height);

		if (rs.texture == _noTexture)
			rect = sf::FloatRect(0.f, 0.f, 16.f, 16.f);
		arr[0].texCoords = sf::Vector2f(rect.left, rect.top);
		arr[1].texCoords = sf::Vector2f(rect.left + rect.width, rect.top);
		arr[2].texCoords = sf::Vector2f(rect.left + rect.width, rect.top + rect.height);
		arr[3].texCoords = sf::Vector2f(rect.left, rect.top + rect.height);
		_render.draw(arr, rs);
	}

	//Draw full steps
	auto& fullStep = _progressBarSteps[_progressBarSteps.size() - 1];

	sf::Texture* tex = _texturesManager->GetTexture(std::get<1>(fullStep));
	if (tex == nullptr || tex->getSize().x == 0 || tex->getSize().y == 0)
		tex = _noTexture;

	auto& size = std::get<0>(fullStep);
	sf::Transformable t;
	sf::VertexArray step(sf::Quads, 4);
	step[0].position = sf::Vector2f(0, 0);
	step[1].position = sf::Vector2f(size.width, 0);
	step[2].position = sf::Vector2f(size.width, size.height);
	step[3].position = sf::Vector2f(0, size.height);

	auto texSize = size;
	if (tex == _noTexture)
		texSize = sf::FloatRect(0.f, 0.f, 16.f, 16.f);
	step[0].texCoords = sf::Vector2f(texSize.left, texSize.top);
	step[1].texCoords = sf::Vector2f(texSize.left + texSize.width, texSize.top);
	step[2].texCoords = sf::Vector2f(texSize.left + texSize.width, texSize.top + texSize.height);
	step[3].texCoords = sf::Vector2f(texSize.left, texSize.top + texSize.height);

	int fullSteps = -1;
	float valueStep = (_step != 0.f) ? _step : 1.f;
	for (float i = 0; i <= _currentValue - valueStep; i+=valueStep)
	{
		fullSteps++;

		t.setPosition(_progressBarStepsPos.x + (size.width * (float)fullSteps), _progressBarStepsPos.y);
		rs.transform = t.getTransform();
		rs.texture = tex;
		_render.draw(step, rs);
	}

	//Draw partialy filled step
	float rest = _currentValue - (valueStep * (float)(fullSteps + 1));
	if (rest > 0.f && _maxValue > 0.f && _currentValue > 0.f)
	{
		fullSteps++;
		size_t id = (size_t)roundf((float)(_progressBarSteps.size() - 1) * (rest / valueStep));

		tex = _texturesManager->GetTexture(std::get<1>(_progressBarSteps[id]));
		if (tex == nullptr || tex->getSize().x == 0 || tex->getSize().y == 0)
			tex = _noTexture;

		auto& newSize = std::get<0>(_progressBarSteps[id]);
		step[0].position = sf::Vector2f(0, 0);
		step[1].position = sf::Vector2f(newSize.width, 0);
		step[2].position = sf::Vector2f(newSize.width, newSize.height);
		step[3].position = sf::Vector2f(0, newSize.height);

		auto newTexSize = newSize;
		if (tex == _noTexture)
			newTexSize = sf::FloatRect(0.f, 0.f, 16.f, 16.f);
		step[0].texCoords = sf::Vector2f(newTexSize.left, newTexSize.top);
		step[1].texCoords = sf::Vector2f(newTexSize.left + newTexSize.width, newTexSize.top);
		step[2].texCoords = sf::Vector2f(newTexSize.left + newTexSize.width, newTexSize.top + newTexSize.height);
		step[3].texCoords = sf::Vector2f(newTexSize.left, newTexSize.top + newTexSize.height);

		t.setPosition(_progressBarStepsPos.x + (size.width * (float)fullSteps), _progressBarStepsPos.y);
		rs.texture = tex;
		rs.transform = t.getTransform();
		_render.draw(step, rs);
	}


	//Draw empty steps
	fullSteps++;

	tex = _texturesManager->GetTexture(std::get<1>(_progressBarSteps[0]));
	if (tex == nullptr || tex->getSize().x == 0 || tex->getSize().y == 0)
		tex = _noTexture;

	auto& emptyStep = std::get<0>(_progressBarSteps[0]);
	step[0].position = sf::Vector2f(0, 0);
	step[1].position = sf::Vector2f(emptyStep.width, 0);
	step[2].position = sf::Vector2f(emptyStep.width, emptyStep.height);
	step[3].position = sf::Vector2f(0, emptyStep.height);

	auto emptyStepTex = emptyStep;
	if (tex == _noTexture)
		emptyStepTex = sf::FloatRect(0.f, 0.f, 16.f, 16.f);
	step[0].texCoords = sf::Vector2f(emptyStepTex.left, emptyStepTex.top);
	step[1].texCoords = sf::Vector2f(emptyStepTex.left + emptyStepTex.width, emptyStepTex.top);
	step[2].texCoords = sf::Vector2f(emptyStepTex.left + emptyStepTex.width, emptyStepTex.top + emptyStepTex.height);
	step[3].texCoords = sf::Vector2f(emptyStepTex.left, emptyStepTex.top + emptyStepTex.height);

	for (float j = (float)fullSteps * valueStep; j < _maxValue; j+=valueStep)
	{
		t.setPosition(_progressBarStepsPos.x + (size.width * (float)fullSteps), _progressBarStepsPos.y);
		rs.transform = t.getTransform();
		rs.texture = tex;
		_render.draw(step, rs);
		fullSteps++;
	}

	_render.display();

	_sthChanged = false;
	_redrawHappened = true;
}

void ProgressBar::Update(bool, float)
{
	Redraw();
}

void ProgressBar::ProcessEvent(sf::Event* ev, const sf::Vector2f& mousePos)
{
	if (_inFocus == true)
	{
		if (_mouseInput == true)
		{
			if (ev->type == sf::Event::MouseButtonPressed && ev->mouseButton.button == sf::Mouse::Left)
			{
				auto barBounds = GetProgressBarStepsGlobalBounds();
				if (barBounds.contains(mousePos))
				{
					float percent = (mousePos.x - barBounds.left) / barBounds.width;
					SetCurrentValue(_maxValue * percent);
				}
			}
		}
		if (_keyboardInput == true)
		{
			if (ev->type == sf::Event::KeyReleased)
			{
				float step = GetStep();
				if (ev->key.code == sf::Keyboard::Left)
				{
					if (_currentValue - step < 0) SetCurrentValue(0.F);
					else SetCurrentValue(_currentValue - step);
				}
				else if (ev->key.code == sf::Keyboard::Right)
				{
					if (_currentValue + step > _maxValue) SetCurrentValue(_maxValue);
					else SetCurrentValue(_currentValue + step);
				}
			}
		}
	}
}

std::vector<sf::Vector2f> ProgressBar::GetAllBoundsPoints() const
{
	std::vector<sf::Vector2f> output = UIElement::GetAllBoundsPoints();
	auto steps = CollisionHelper::GetRectPoints(GetProgressBarStepsGlobalBounds());
	output.insert(output.end(), std::make_move_iterator(steps.begin()), std::make_move_iterator(steps.end()));
	return output;
}

UIElement* ProgressBar::clone()
{
	return new ProgressBar(*this);
}

