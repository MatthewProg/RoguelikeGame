#include "ScrollView.h"

void ScrollView::UpdateElementsBounds()
{
	_elementsBounds.height = 0.f;
	_elementsBounds.width = 0.f;

	for(auto& b : _elements)
		if (std::get<0>(b) != nullptr)
		{
			auto bound = std::get<0>(b)->GetGlobalBounds();
			if (bound.top + bound.height > _elementsBounds.height)
				_elementsBounds.height = bound.top + bound.height;
			if(bound.left + bound.width > _elementsBounds.width)
				_elementsBounds.width = bound.left + bound.width;
		}

	RedrawElements();
	_sthChanged = true;
}

void ScrollView::UpdateListRect()
{
	_listRect.top = 0.f;
	_listRect.left = 0.f;

	if (_scrollHorizontaly)
		_listRect.left = ((_elementsBounds.left + _elementsBounds.width) - _listRect.width) * _scroll;
	else
		_listRect.top = ((_elementsBounds.top + _elementsBounds.height) - _listRect.height) * _scroll;

	if (_elementsBounds.left + _elementsBounds.width < _listRect.width) _listRect.left = 0.f;
	if (_elementsBounds.top + _elementsBounds.height < _listRect.height) _listRect.top = 0.f;
}

void ScrollView::AlignElements()
{
	for (size_t i = 0; i < _elements.size(); i++)
	{
		auto ptr = std::get<0>(_elements[i]);
		if (ptr == nullptr) continue;

		if (_scrollHorizontaly == true)
		{
			if (i == 0)
			{
				auto pos = ptr->getPosition();
				pos.x = 0.f;
				ptr->setPosition(pos);
			}
			else
			{
				auto prevB = std::get<0>(_elements[i - 1])->GetGlobalBounds();

				sf::Vector2f newPos(ptr->getPosition());
				newPos.x = prevB.left + prevB.width;

				ptr->setPosition(newPos);
			}
		}
		else
		{
			if (i == 0)
			{
				auto pos = ptr->getPosition();
				pos.y = 0.f;
				ptr->setPosition(pos);
			}
			else
			{
				auto prevB = std::get<0>(_elements[i - 1])->GetGlobalBounds();

				sf::Vector2f newPos(ptr->getPosition());
				newPos.y = prevB.top + prevB.height;

				ptr->setPosition(newPos);
			}
		}
	}
	UpdateElementsBounds();
}

void ScrollView::ScrollToCurrentElement()
{
	auto e = std::get<0>(_elements[_currentIndex]);
	if (e == nullptr) return;

	if (_scrollHorizontaly)
	{
		float showedLeftLine = _listRect.left;
		float showedRightLine = _listRect.left + _listRect.width;

		auto eBounds = e->GetGlobalBounds();

		if (eBounds.left >= showedLeftLine && eBounds.left + eBounds.width <= showedRightLine) //All good
		{
			if (_elements.size() > 1)
				SetScroll(float(_currentIndex) / float(_elements.size() - 1));
			else
				SetScroll(0.f);
		}
		else
		{
			if (eBounds.width <= _listRect.width) //Element fits in window
			{
				if (eBounds.left < showedLeftLine) //Scroll right
				{
					float diff = showedLeftLine - eBounds.left;
					float movePercent = diff / ((_elementsBounds.left + _elementsBounds.width) - _listRect.width);
					SetScroll(GetScroll() + movePercent);
				}
				else if (eBounds.left + eBounds.width > showedRightLine) //Scroll left
				{
					float diff = (eBounds.left + eBounds.width) - showedRightLine;
					float movePercent = diff / ((_elementsBounds.left + _elementsBounds.width) - _listRect.width);
					SetScroll(GetScroll() - movePercent);
				}
			}
			else //Just clip to left
			{
				float diff = showedLeftLine - eBounds.left;
				float movePercent = diff / ((_elementsBounds.left + _elementsBounds.width) - _listRect.width);
				SetScroll(GetScroll() + movePercent);
			}
		}
	}
	else
	{
		float showedTopLine = _listRect.top;
		float showedBottomLine = _listRect.top + _listRect.height;

		auto eBounds = e->GetGlobalBounds();

		if (eBounds.top >= showedTopLine && eBounds.top + eBounds.height <= showedBottomLine) //All good
		{
			if (_elements.size() > 1)
				SetScroll(float(_currentIndex) / float(_elements.size() - 1));
			else
				SetScroll(0.f);
		}
		else
		{
			if (eBounds.height <= _listRect.height) //Element fits in window
			{
				if (eBounds.top < showedTopLine) //Scroll up
				{
					float diff = showedTopLine - eBounds.top;
					float movePercent = diff / ((_elementsBounds.top + _elementsBounds.height) - _listRect.height);
					SetScroll(GetScroll() - movePercent);
				}
				else if(eBounds.top + eBounds.height > showedBottomLine) //Scroll down
				{
					float diff = (eBounds.top + eBounds.height) - showedBottomLine;
					float movePercent = diff / ((_elementsBounds.top + _elementsBounds.height) - _listRect.height);
					SetScroll(GetScroll() + movePercent);
				}
			}
			else //Just clip to top
			{
				float diff = showedTopLine - eBounds.top;
				float movePercent = diff / ((_elementsBounds.top + _elementsBounds.height) - _listRect.height);
				SetScroll(GetScroll() - movePercent);
			}
		}
	}
}

void ScrollView::RedrawElements()
{
	_elementsRender.create(uint32_t(ceilf(_elementsBounds.width)), uint32_t(ceilf(_elementsBounds.height)));
	_elementsRender.clear(sf::Color::Transparent);

	sf::VertexArray v(sf::PrimitiveType::Quads, 4);
	sf::RenderStates rs;
	for (auto& e : _elements)
	{
		auto ptr = std::get<0>(e);
		if (ptr == nullptr) continue;

		if (ptr->GetVisibility() == false) continue;
		auto texture = ptr->GetTexture();
		auto size = texture->getSize();
		rs.texture = &texture->getTexture();
		rs.transform = ptr->getTransform();
		v[0].position = sf::Vector2f(0, 0);
		v[1].position = sf::Vector2f((float)size.x, 0);
		v[2].position = sf::Vector2f((float)size.x, (float)size.y);
		v[3].position = sf::Vector2f(0, (float)size.y);
		v[0].texCoords = sf::Vector2f(0, 0);
		v[1].texCoords = sf::Vector2f((float)size.x, 0);
		v[2].texCoords = sf::Vector2f((float)size.x, (float)size.y);
		v[3].texCoords = sf::Vector2f(0, (float)size.y);
		_elementsRender.draw(v, rs);
	}

	_elementsRender.display();
}

ScrollView::ScrollView()
{
	_elements.clear();
	_currentIndex = 0;
	_scrollBar = nullptr;
	_scroll = 0.f;
	_elementsBounds = sf::FloatRect(0.f, 0.f, 0.f, 0.f);
	_listRect = sf::FloatRect(0.f, 0.f, 0.f, 0.f);
	_backgroundTex = "";
	_backgroundRect = sf::FloatRect(0.f, 0.f, 0.f, 0.f);
	_backgroundSize = sf::Vector2f(0.f, 0.f);
	_scrollHorizontaly = false;
}

ScrollView::ScrollView(ScrollView& other) : UIElement(other)
{
	for (auto& e : other._elements)
		_elements.push_back(Element((FocusContainer*)std::get<0>(e)->clone(), std::get<1>(e)));
	auto size = other._elementsRender.getSize();
	_elementsRender.create(size.x, size.y);
	_currentIndex = other._currentIndex;
	_scrollBar = (ScrollBar*)other._scrollBar->clone();
	_scroll = other._scroll;
	_elementsBounds = other._elementsBounds;
	_backgroundTex = other._backgroundTex;
	_backgroundRect = other._backgroundRect;
	_backgroundTransform = other._backgroundTransform;
	_backgroundSize = other._backgroundSize;
	_scrollHorizontaly = other._scrollHorizontaly;
	_elementsBounds = other._elementsBounds;
	_elementsTransform = other._elementsTransform;
	_listRect = other._listRect;
}

ScrollView::~ScrollView()
{
	for (auto& e : _elements)
		if (std::get<0>(e) != nullptr)
			delete std::get<0>(e);
	if (_scrollBar != nullptr)
		delete _scrollBar;
}

void ScrollView::AddElement(const std::string& name, FocusContainer* container)
{
	if (container != nullptr)
		_elements.push_back(Element(container, name));
	AlignElements();
	_sthChanged = true;
}

FocusContainer* ScrollView::GetElement(size_t index)
{
	if(index >= _elements.size())
		return nullptr;
	return std::get<0>(_elements[index]);
}

FocusContainer* ScrollView::GetElement(const std::string& name)
{
	for (auto it = _elements.begin(); it != _elements.end(); it++)
		if (std::get<1>(*it) == name)
			return std::get<0>(*it);
	return nullptr;
}

void ScrollView::ClearElements()
{
	for (auto& e : _elements)
		if (std::get<0>(e) != nullptr)
			delete std::get<0>(e);
	_elements.clear();
	_currentIndex = 0;
	AlignElements();
	_sthChanged = true;
}

void ScrollView::RemoveElement(size_t index)
{
	if (index >= _elements.size()) return;

	if(std::get<0>(_elements[index]) != nullptr)
		delete std::get<0>(_elements[index]);
	_elements.erase(_elements.begin() + index);

	if (index <= _currentIndex)
		PreviousElement();
	AlignElements();
	_sthChanged = true;
}

void ScrollView::RemoveElement(const std::string& name)
{
	for (auto it = _elements.begin(); it != _elements.end(); it++)
		if (std::get<1>(*it) == name)
		{
			_elements.erase(it);

			if (it - _elements.begin() <= _currentIndex)
				PreviousElement();
			AlignElements();
			_sthChanged = true;
			break;
		}
}

void ScrollView::SwapElements(size_t firstIndex, size_t secondIndex)
{
	if (firstIndex >= _elements.size() || secondIndex >= _elements.size()) return;

	std::iter_swap(_elements.begin() + firstIndex, _elements.begin() + secondIndex);
	AlignElements();
	_sthChanged = true;
}

void ScrollView::SwapElements(const std::string& firstName, const std::string& secondName)
{
	std::vector<Element>::iterator first, second;
	for (auto it = _elements.begin(); it != _elements.end(); it++)
	{
		if (std::get<1>(*it) == firstName) first = it;
		if (std::get<1>(*it) == secondName) second = it;
	}
	if (first != _elements.end() && second != _elements.end())
	{
		first->swap(*second);
		AlignElements();
		_sthChanged = true;
	}
}

size_t ScrollView::GetNoOfElements() const
{
	return _elements.size();
}

bool ScrollView::ElementExists(const std::string& name) const
{
	for (auto& e : _elements)
		if (std::get<1>(e) == name)
			return true;
	return false;
}

void ScrollView::NextElement()
{
	if (_elements.size() == 0)
	{
		_currentIndex = 0;
		return;
	}

	for (auto& e : _elements)
		if (std::get<0>(e) != nullptr) 
			std::get<0>(e)->SetInFocus(false);

	if (_currentIndex + 1 >= _elements.size())
		_currentIndex = _elements.size() - 1;
	else
		_currentIndex++;

	auto ptr = GetElement(_currentIndex);
	if (ptr != nullptr) ptr->SetInFocus(true);
	ScrollToCurrentElement();
	_sthChanged = true;
}

void ScrollView::PreviousElement()
{
	if (_currentIndex == 0) return;

	for (auto& e : _elements)
		if (std::get<0>(e) != nullptr) 
			std::get<0>(e)->SetInFocus(false);

	_currentIndex--;

	auto ptr = GetElement(_currentIndex);
	if (ptr != nullptr) ptr->SetInFocus(true);

	ScrollToCurrentElement();
	_sthChanged = true;
}

void ScrollView::SetCurrentIndex(size_t index)
{
	if (_elements.size() == 0)
		_currentIndex = 0;
	else if (index < _elements.size())
		_currentIndex = index;
	ScrollToCurrentElement();
	_sthChanged = true;
}

void ScrollView::SetScrollHorizontaly(bool horizontalMode)
{
	_scrollHorizontaly = horizontalMode;
}

void ScrollView::SetScrollBar(ScrollBar* scrollBar)
{
	if (_scrollBar != nullptr)
		delete _scrollBar;

	_scrollBar = scrollBar;
	_sthChanged = true;
}

void ScrollView::SetBackground(const std::string& texName, const sf::FloatRect& rect)
{
	_backgroundTex = texName;
	_backgroundRect = rect;
	_sthChanged = true;
}

void ScrollView::SetBackgroundSize(const sf::Vector2f& size)
{
	_backgroundSize = size;
	_sthChanged = true;
}

void ScrollView::SetListSize(const sf::Vector2f& size)
{
	if (_listRect.width != size.x || _listRect.height != size.y)
	{
		_listRect.width = size.x;
		_listRect.height = size.y;
		_sthChanged = true;
	}
}

void ScrollView::SetScroll(float percent)
{
	_scroll = std::max(0.f, std::min(percent, 1.f));

	if (_scrollBar != nullptr)
	{
		float s = _scroll;
		if (_scrollHorizontaly == false) s = 1.f - s;
		_scrollBar->SetScroll(s);
	}
	UpdateListRect();
	_sthChanged = true;
}

size_t ScrollView::GetCurrentIndex() const
{
	return _currentIndex;
}

bool ScrollView::GetScrollHorizontaly() const
{
	return _scrollHorizontaly;
}

ScrollBar* ScrollView::GetScrollBar()
{
	_sthChanged = true;
	return _scrollBar;
}

std::tuple<std::string, sf::FloatRect> ScrollView::GetBackground() const
{
	return std::tuple<std::string, sf::FloatRect>(_backgroundTex, _backgroundRect);
}

const sf::Vector2f& ScrollView::GetBackgroundSize() const
{
	return _backgroundSize;
}

sf::Vector2f ScrollView::GetListSize() const
{
	return sf::Vector2f(_listRect.width, _listRect.height);
}

float ScrollView::GetScroll()
{
	if (_scrollBar != nullptr)
	{
		float s = _scrollBar->GetScroll();
		if (_scrollHorizontaly == false) s = 1.f - s;

		if (_scroll != s)
		{
			_scroll = s;
			UpdateListRect();
			_sthChanged = true;
		}
	}

	return _scroll;
}

sf::Transformable* ScrollView::GetBackgroundTransform()
{
	_sthChanged = true;
	return &_backgroundTransform;
}

sf::Transformable* ScrollView::GetElementsTransform()
{
	_sthChanged = true;
	return &_elementsTransform;
}

const sf::FloatRect& ScrollView::GetElementsBounds() const
{
	return _elementsBounds;
}

UIElement* ScrollView::clone()
{
	return new ScrollView(*this);
}

void ScrollView::Update(bool tick, float delta)
{
	for (auto& e : _elements)
		if (std::get<0>(e) != nullptr)
			std::get<0>(e)->Update(tick, delta);

	if (_scrollBar != nullptr)
	{
		_scrollBar->Update(tick, delta);
		GetScroll();
	}

	Redraw();
}

void ScrollView::ForceRedraw()
{
	for (auto& e : _elements)
	{
		auto ptr = std::get<0>(e);
		if (ptr == nullptr) continue;
		ptr->ForceRedraw();
	}

	if (_scrollBar != nullptr)
		_scrollBar->ForceRedraw();
	Redraw();
}

bool ScrollView::Redraw()
{
	bool refreshElements = false;
	for (auto& e : _elements)
	{
		auto ptr = std::get<0>(e);
		if (ptr == nullptr) continue;
		if (ptr->RedrawHappened())
		{ 
			_sthChanged = true; 
			refreshElements = true; 
			break; 
		}
	}
	if (_scrollBar != nullptr)
		if (_scrollBar->RedrawHappened())
			_sthChanged = true;

	if(refreshElements) RedrawElements();
	if (_sthChanged)
	{
		sf::VertexArray v(sf::PrimitiveType::Quads, 4);
		sf::RenderStates rs;
		_render.clear(sf::Color::Transparent);

		//Background
		if (_backgroundSize.x != 0 && _backgroundSize.y != 0)
		{
			auto bgRect = _backgroundRect;
			auto bgTex = _texturesManager->GetTexture(_backgroundTex);
			if (bgTex == nullptr)
			{
				bgTex = _noTexture;
				bgRect = sf::FloatRect(0.f, 0.f, 16.f, 16.f);
			}
			v[0].position = sf::Vector2f(0.f, 0.f);
			v[1].position = sf::Vector2f(_backgroundSize.x, 0.f);
			v[2].position = _backgroundSize;
			v[3].position = sf::Vector2f(0.f, _backgroundSize.y);
			v[0].texCoords = sf::Vector2f(bgRect.left, bgRect.top);
			v[1].texCoords = sf::Vector2f(bgRect.left + bgRect.width, bgRect.top);
			v[2].texCoords = sf::Vector2f(bgRect.left + bgRect.width, bgRect.top + bgRect.height);
			v[3].texCoords = sf::Vector2f(bgRect.left, bgRect.top + bgRect.height);
			rs.transform = _backgroundTransform.getTransform();
			rs.texture = bgTex;
			_render.draw(v, rs);
		}

		//Elements
		sf::FloatRect renderRect(_listRect);
		if (_listRect.height > _elementsBounds.top + _elementsBounds.height ||
			_listRect.width > _elementsBounds.left + _elementsBounds.width) renderRect = _elementsBounds;
		v[0].position = sf::Vector2f(0.f, 0.f);
		v[1].position = sf::Vector2f(renderRect.width, 0.f);
		v[2].position = sf::Vector2f(renderRect.width, renderRect.height);
		v[3].position = sf::Vector2f(0.f, renderRect.height);
		v[0].texCoords = sf::Vector2f(renderRect.left, renderRect.top);
		v[1].texCoords = sf::Vector2f(renderRect.left + renderRect.width, renderRect.top);
		v[2].texCoords = sf::Vector2f(renderRect.left + renderRect.width, renderRect.top + renderRect.height);
		v[3].texCoords = sf::Vector2f(renderRect.left, renderRect.top + renderRect.height);
		rs.transform = _elementsTransform.getTransform();
		rs.texture = &_elementsRender.getTexture();
		_render.draw(v, rs);

		//ScrollBar
		if (_scrollBar != nullptr)
		{
			auto rt = _scrollBar->GetTexture();
			auto sbSize = rt->getSize();
			auto fSize = sf::Vector2f(float(sbSize.x), float(sbSize.y));

			v[0].position = sf::Vector2f(0.f, 0.f);
			v[1].position = sf::Vector2f(fSize.x, 0.f);
			v[2].position = sf::Vector2f(fSize.x, fSize.y);
			v[3].position = sf::Vector2f(0.f, fSize.y);
			v[0].texCoords = sf::Vector2f(0.f, 0.f);
			v[1].texCoords = sf::Vector2f(fSize.x, 0.f);
			v[2].texCoords = sf::Vector2f(fSize.x, fSize.y);
			v[3].texCoords = sf::Vector2f(0.f, fSize.y);
			rs.transform = _scrollBar->getTransform();
			rs.texture = &rt->getTexture();
			_render.draw(v, rs);
		}

		_render.display();

		_sthChanged = false;
		_redrawHappened = true;
		return true;
	}
	return false;
}

void ScrollView::ProcessEvent(sf::Event* ev, const sf::Vector2f& mousePos)
{
	auto relMouse = getTransform().getInverse().transformPoint(mousePos);
	if (_scrollBar != nullptr)
	{
		_scrollBar->SetInFocus(GetInFocus());
		_scrollBar->ProcessEvent(ev, relMouse);
	}

	if (GetInFocus())
	{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) NextElement();
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) PreviousElement();
	}
	//Pass mouse to containers moved by scroll amount
	relMouse = _elementsTransform.getInverseTransform().transformPoint(relMouse);
	if (_scrollHorizontaly) relMouse.x += _listRect.left;
	else relMouse.y += _listRect.top;

	//Clamp to listRect
	relMouse.x = std::max(_listRect.left, std::min(relMouse.x, _listRect.left + _listRect.width));
	relMouse.y = std::max(_listRect.top, std::min(relMouse.y, _listRect.top + _listRect.height));

	for (auto& e : _elements)
		if (std::get<0>(e) != nullptr)
		{
			if (std::get<0>(e)->GetGlobalBounds().intersects(_listRect) == false)
				continue;
			std::get<0>(e)->ProcessEvent(ev, relMouse);
		}

}

std::vector<sf::Vector2f> ScrollView::GetAllBoundsPoints() const
{
	std::vector<sf::Vector2f> output = UIElement::GetAllBoundsPoints();

	if (_scrollBar != nullptr)
	{
		auto sb = _scrollBar->GetAllBoundsPoints();
		for (auto& p : sb)
			p = getTransform().transformPoint(p);
		output.insert(output.end(), std::make_move_iterator(sb.begin()), std::make_move_iterator(sb.end()));
	}
	for(auto&e : _elements)
		if (std::get<0>(e) != nullptr)
		{
			if (std::get<0>(e)->GetGlobalBounds().intersects(_listRect) == false) 
				continue;

			auto fill = std::get<0>(e)->GetAllBoundsPoints();
			for (auto& p : fill)
			{
				//Clip to listRect
				if (p.x < _listRect.left) p.x = _listRect.left;
				else if (p.x > _listRect.left + _listRect.width) p.x = _listRect.left + _listRect.width;
				if (p.y < _listRect.top) p.y = _listRect.top;
				else if (p.y > _listRect.top + _listRect.height) p.y = _listRect.top + _listRect.height;

				p -= sf::Vector2f(_listRect.left, _listRect.top);
				p = getTransform().transformPoint(p);
				p = _elementsTransform.getTransform().transformPoint(p);
			}
			output.insert(output.end(), std::make_move_iterator(fill.begin()), std::make_move_iterator(fill.end()));
		}

	auto posZeroList = sf::FloatRect(0.f,0.f,_listRect.width, _listRect.height);
	auto rect = getTransform().transformRect(posZeroList);
	auto ls = _elementsTransform.getTransform().transformRect(rect);
	auto view = CollisionHelper::GetRectPoints(ls);
	output.insert(output.end(), std::make_move_iterator(view.begin()), std::make_move_iterator(view.end()));
	return output;
}

std::vector<sf::Vector2f> ScrollView::GetDeepestInFocusBoundsPoints() const
{
	for(auto& e : _elements)
		if (std::get<0>(e) != nullptr)
		{
			auto ptr = std::get<0>(e);
			if (ptr->GetInFocus() == true && ptr->GetGlobalBounds().intersects(_listRect))
			{
				auto v = ptr->GetDeepestInFocusBoundsPoints();
				for (auto& p : v)
				{
					if (p.x < _listRect.left) p.x = _listRect.left;
					else if (p.x > _listRect.left + _listRect.width) p.x = _listRect.left + _listRect.width;
					if (p.y < _listRect.top) p.y = _listRect.top;
					else if (p.y > _listRect.top + _listRect.height) p.y = _listRect.top + _listRect.height;

					p -= sf::Vector2f(_listRect.left, _listRect.top);
					p = getTransform().transformPoint(p);
					p = _elementsTransform.getTransform().transformPoint(p);
				}
				return v;
			}
		}

	if (_scrollBar != nullptr && _scrollBar->GetInFocus() == true)
	{
		auto v = _scrollBar->GetDeepestInFocusBoundsPoints();
		for (auto& p : v)
			p = getTransform().transformPoint(p);
		return v;
	}
	return UIElement::GetAllBoundsPoints();
}
