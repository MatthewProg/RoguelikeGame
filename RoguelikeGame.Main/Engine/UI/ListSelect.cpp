#include "ListSelect.h"

void ListSelect::UpdateArrowsState()
{
    if (_selectedIndex == 0) _leftArrow.ForceState("click");
    else _leftArrow.ResetForcedState();

    if (_selectedIndex + 1 >= _values.size()) _rightArrow.ForceState("click");
    else _rightArrow.ResetForcedState();
}

ListSelect::ListSelect(SoundsManager* sounds, TexturesManager* textures)
{
    _soundsManager = sounds;
    _texturesManager = textures;
    _selectedIndex = 0;
    _values.clear();
    _leftArrow.SetSoundsManager(_soundsManager);
    _leftArrow.SetTexturesManager(_texturesManager);
    _rightArrow.SetSoundsManager(_soundsManager);
    _rightArrow.SetTexturesManager(_texturesManager);
    _currSelectionLabel.SetSoundsManager(_soundsManager);
    _currSelectionLabel.SetTexturesManager(_texturesManager);
    _leftArrow.SetMouseInput(true);
    _rightArrow.SetMouseInput(true);
    _leftArrow.SetKeyboardInput(true);
    _rightArrow.SetKeyboardInput(true);
}

ListSelect::ListSelect(ListSelect& other) : UIElement(other), _leftArrow(other._leftArrow), _rightArrow(other._rightArrow), _currSelectionLabel(other._currSelectionLabel)
{
    _selectedIndex = other._selectedIndex;
    _values = other._values;
}

ListSelect::~ListSelect()
{
}

void ListSelect::AddLeftArrowState(const std::string& name, const std::string& textureName, const sf::FloatRect& rect)
{
    _leftArrow.AddState(name, _dummyText, textureName, rect);
}

void ListSelect::RemoveLeftArrowState(const std::string& name)
{
    _leftArrow.RemoveState(name);
}

void ListSelect::SetLeftArrowSize(const sf::Vector2f& size)
{
    _leftArrow.Init(sf::Vector2u((uint32_t)ceilf(size.x), (uint32_t)ceilf(size.y)));
    _leftArrow.SetBackgroundSize(size);
}

sf::Transformable* ListSelect::TransformLeftArrow()
{
    _sthChanged = true;
    return (sf::Transformable*)&_leftArrow;
}

void ListSelect::AddRightArrowState(const std::string& name, const std::string& textureName, const sf::FloatRect& rect)
{
    _rightArrow.AddState(name, _dummyText, textureName, rect);
}

void ListSelect::RemoveRightArrowState(const std::string& name)
{
    _rightArrow.RemoveState(name);
}

void ListSelect::SetRightArrowSize(const sf::Vector2f& size)
{
    _rightArrow.Init(sf::Vector2u((uint32_t)ceilf(size.x), (uint32_t)ceilf(size.y)));
    _rightArrow.SetBackgroundSize(size);
}

sf::Transformable* ListSelect::TransformRightArrow()
{
    _sthChanged = true;
    return (sf::Transformable*)&_rightArrow;
}

void ListSelect::SetTextSize(const sf::Vector2f& size)
{
    _currSelectionLabel.Init(sf::Vector2u((uint32_t)ceilf(size.x), (uint32_t)ceilf(size.y)));
}

void ListSelect::SetFont(const sf::Font& font)
{
    _currSelectionLabel.SetFont(font);
}

void ListSelect::SetCharacterSize(uint32_t size)
{
    _currSelectionLabel.SetCharacterSize(size);
}

void ListSelect::SetLetterSpacing(float spacing)
{
    _currSelectionLabel.SetLetterSpacing(spacing);
}

void ListSelect::SetStyle(uint32_t style)
{
    _currSelectionLabel.SetStyle(style);
}

void ListSelect::SetFillColor(const sf::Color& color)
{
    _currSelectionLabel.SetFillColor(color);
}

void ListSelect::SetOutlineColor(const sf::Color& color)
{
    _currSelectionLabel.SetOutlineColor(color);
}

void ListSelect::SetOutlineThickness(float thickness)
{
    _currSelectionLabel.SetOutlineThickness(thickness);
}

void ListSelect::SetVerticalAlignment(UIElement::Align align)
{
    _currSelectionLabel.SetVerticalAlignment(align);
}

void ListSelect::SetHorizontalAlignment(UIElement::Align align)
{
    _currSelectionLabel.SetHorizontalAlignment(align);
}

sf::Transformable* ListSelect::TransformText()
{
    return &_currSelectionLabel;
}

void ListSelect::AddValue(const std::string& value)
{
    _values.push_back(value);

    if (_values.size() == 1)
        SetCurrentIndex(0);
    UpdateArrowsState();
    _sthChanged = true;
}

void ListSelect::RemoveValue(size_t index)
{
    if (_selectedIndex >= index)
    {
        if(_selectedIndex == index) _sthChanged = true;
        if (_selectedIndex > 0) _selectedIndex--;
    }
    
    if (index < _values.size())
        _values.erase(_values.begin() + index);
    UpdateArrowsState();
}

void ListSelect::ClearValues()
{
    _selectedIndex = 0;
    _values.clear();
    UpdateArrowsState();
    _sthChanged = true;
}

void ListSelect::SetCurrentIndex(size_t index)
{
    if (index < _values.size())
    {
        if (_selectedIndex != index)
        {
            _selectedIndex = index;
            UpdateArrowsState();
            _sthChanged = true;
        }
    }
}

void ListSelect::NextSelection()
{
    if (_selectedIndex < _values.size() - 1)
    {
        _selectedIndex++;
        UpdateArrowsState();
        _sthChanged = true;
    }
}

void ListSelect::PreviousSelection()
{
    if (_selectedIndex > 0)
    {
        _selectedIndex--;
        UpdateArrowsState();
        _sthChanged = true;
    }
}

const sf::Font* ListSelect::GetFont() const
{
    return _currSelectionLabel.GetFont();
}

uint32_t ListSelect::GetCharacterSize() const
{
    return _currSelectionLabel.GetCharacterSize();
}

float ListSelect::GetLetterSpacing() const
{
    return _currSelectionLabel.GetLetterSpacing();
}

uint32_t ListSelect::GetStyle() const
{
    return _currSelectionLabel.GetStyle();
}

const sf::Color& ListSelect::GetFillColor() const
{
    return _currSelectionLabel.GetFillColor();
}

const sf::Color& ListSelect::GetOutlineColor() const
{
    return _currSelectionLabel.GetOutlineColor();
}

float ListSelect::GetOutlineThickness() const
{
    return _currSelectionLabel.GetOutlineThickness();
}

UIElement::Align ListSelect::GetVerticalAlignment() const
{
    return _currSelectionLabel.GetVerticalAlignment();
}

UIElement::Align ListSelect::GetHorizontalAlignment() const
{
    return _currSelectionLabel.GetHorizontalAlignment();
}

std::string ListSelect::GetCurrentValue() const
{
    if (_values.size() == 0 || _selectedIndex >= _values.size())
        return std::string();
    return _values[_selectedIndex];
}

size_t ListSelect::GetCurrentIndex() const
{
    return _selectedIndex;
}

size_t ListSelect::GetValuesSize() const
{
    return _values.size();
}

const std::vector<std::string>& ListSelect::GetValues() const
{
    return _values;
}

UIElement* ListSelect::clone()
{
    return new ListSelect(*this);
}

void ListSelect::Update(bool tick, float delta)
{
    if (_values.size() == 0 || _selectedIndex >= _values.size())
        _currSelectionLabel.SetText("");
    else
        _currSelectionLabel.SetText(_values[_selectedIndex]);

    _leftArrow.Update(tick, delta);
    _rightArrow.Update(tick, delta);
    _currSelectionLabel.Update(tick, delta);

    Redraw();
}

void ListSelect::ForceRedraw()
{
    _leftArrow.ForceRedraw();
    _rightArrow.ForceRedraw();
    _currSelectionLabel.ForceRedraw();
    Redraw();
}

bool ListSelect::Redraw()
{
    if (_leftArrow.RedrawHappened() ||
        _rightArrow.RedrawHappened() ||
        _currSelectionLabel.RedrawHappened()) _sthChanged = true;

    if (_sthChanged == true)
    {
        _render.clear(sf::Color::Transparent);

        sf::VertexArray element(sf::PrimitiveType::Quads, 4);
        sf::RenderStates rs;

        for (unsigned short i = 0; i < 3; i++)
        {
            const sf::RenderTexture* tex = nullptr;
            sf::Vector2f pos = sf::Vector2f(0.f, 0.f);

            if (i == 0)
            {
                tex = _leftArrow.GetTexture();
                pos = _leftArrow.getPosition();
            }
            else if (i == 1)
            {
                tex = _rightArrow.GetTexture();
                pos = _rightArrow.getPosition();
            }
            else
            {
                tex = _currSelectionLabel.GetTexture();
                pos = _currSelectionLabel.getPosition();
            }
            auto sizeU = tex->getSize();
            auto size = sf::Vector2f(float(sizeU.x), float(sizeU.y));
            rs.texture = &tex->getTexture();

            element[0].position = sf::Vector2f(pos.x, pos.y);
            element[1].position = sf::Vector2f(pos.x + size.x, pos.y);
            element[2].position = sf::Vector2f(pos.x + size.x, pos.y + size.y);
            element[3].position = sf::Vector2f(pos.x, pos.y + size.y);
            element[0].texCoords = sf::Vector2f(0.f, 0.f);
            element[1].texCoords = sf::Vector2f(size.x, 0.f);
            element[2].texCoords = sf::Vector2f(size.x, size.y);
            element[3].texCoords = sf::Vector2f(0.f, size.y);

            _render.draw(element, rs);
        }

        _render.display();

        _sthChanged = false;
        _redrawHappened = true;
        return true;
    }
    return false;
}

void ListSelect::ProcessEvent(sf::Event* ev, const sf::Vector2f& mousePos)
{
    auto relMouse = getTransform().getInverse().transformPoint(mousePos);
    _leftArrow.ProcessEvent(ev, relMouse);
    _rightArrow.ProcessEvent(ev, relMouse);
    _currSelectionLabel.ProcessEvent(ev, relMouse);

    if (_mouseInput)
    {
        if (ev->type == sf::Event::MouseButtonPressed && _leftArrow.GetGlobalBounds().contains(relMouse) == false) _leftArrow.SetInFocus(false);
        if (ev->type == sf::Event::MouseButtonPressed && _rightArrow.GetGlobalBounds().contains(relMouse) == false)_rightArrow.SetInFocus(false);
    }
    if (_keyboardInput)
    {
        if (_inFocus)
        {
            if (ev->type == sf::Event::KeyPressed && ev->key.code == sf::Keyboard::Left)
            {
                sf::Event e;
                e.type = sf::Event::KeyPressed;
                e.key.code = sf::Keyboard::Enter;
                _leftArrow.SetInFocus(true);
                _leftArrow.ProcessEvent(&e, sf::Vector2f(0.f, 0.f));
            }
            else if (ev->type == sf::Event::KeyPressed && ev->key.code == sf::Keyboard::Right)
            {
                sf::Event e;
                e.type = sf::Event::KeyPressed;
                e.key.code = sf::Keyboard::Enter;
                _rightArrow.SetInFocus(true);
                _rightArrow.ProcessEvent(&e, sf::Vector2f(0.f, 0.f));
            }

            if (ev->type == sf::Event::KeyReleased && (ev->key.code == sf::Keyboard::Left || ev->key.code == sf::Keyboard::Right))
            {
                sf::Event e;
                e.type = sf::Event::KeyReleased;
                e.key.code = sf::Keyboard::Enter;
                _leftArrow.ProcessEvent(&e, sf::Vector2f(0.f, 0.f));
                _rightArrow.ProcessEvent(&e, sf::Vector2f(0.f, 0.f));
                _leftArrow.SetInFocus(false);
                _rightArrow.SetInFocus(false);
            }
        }
    }
      
    if (_leftArrow.Clicked()) PreviousSelection();
    if (_rightArrow.Clicked()) NextSelection();
}

std::vector<sf::Vector2f> ListSelect::GetAllBoundsPoints() const
{
    std::vector<sf::Vector2f> output = UIElement::GetAllBoundsPoints();
    auto la = _leftArrow.GetAllBoundsPoints();
    auto ra = _rightArrow.GetAllBoundsPoints();
    auto lab = _currSelectionLabel.GetAllBoundsPoints();
    output.insert(output.end(), std::make_move_iterator(la.begin()), std::make_move_iterator(la.end()));
    output.insert(output.end(), std::make_move_iterator(ra.begin()), std::make_move_iterator(ra.end()));
    output.insert(output.end(), std::make_move_iterator(lab.begin()), std::make_move_iterator(lab.end()));
    for (size_t i = 4; i < output.size(); i++)
        output[i] = getTransform().transformPoint(output[i]);
    return output;
}

std::vector<sf::Vector2f> ListSelect::GetDeepestInFocusBoundsPoints() const
{
    std::vector<sf::Vector2f> output;
    if (_leftArrow.GetInFocus() == true) output = _leftArrow.GetAllBoundsPoints();
    else if (_rightArrow.GetInFocus() == true) output = _rightArrow.GetAllBoundsPoints();

    if (output.size() == 0)
        output = UIElement::GetAllBoundsPoints();
    else
        for (auto& p : output)
            p = getTransform().transformPoint(p);
    return output;
}
