#include "ScrollBar.h"

UIElement* ScrollBar::clone()
{
    return new ScrollBar(*this);
}

void ScrollBar::Update(bool tick, float delta)
{
    _thumb.Update(tick, delta);

    Redraw();
}

void ScrollBar::ForceRedraw()
{
    _thumb.ForceRedraw();
    Redraw();
}

bool ScrollBar::Redraw()
{
    if (_thumb.RedrawHappened()) _sthChanged = true;

    if (_sthChanged)
    {
        sf::RenderStates rs;
        sf::VertexArray v(sf::PrimitiveType::Quads, 4);

        sf::FloatRect defSixteen(0.f, 0.f, 16.f, 16.f);
        sf::FloatRect bSize(defSixteen), bTexSize(defSixteen), eSize(defSixteen), eTexSize(defSixteen);
        sf::Texture* bTex = _noTexture, * eTex = _noTexture;

        auto foundB = _textures.find("beginning");
        auto foundE = _textures.find("ending");

        if (foundB != _textures.end())
        {
            bSize = std::get<1>(foundB->second);
            auto tmpTex = _texturesManager->GetTexture(std::get<0>(foundB->second));
            if (tmpTex != nullptr && tmpTex->getSize().x > 0)
            {
                bTex = tmpTex;
                bTexSize = bSize;
            }
        }
        if (foundE != _textures.end())
        {
            eSize = std::get<1>(foundE->second);
            auto tmpTex = _texturesManager->GetTexture(std::get<0>(foundE->second));
            if (tmpTex != nullptr && tmpTex->getSize().x > 0)
            {
                eTex = tmpTex;
                eTexSize = eSize;
            }
        }


        _render.clear(sf::Color::Transparent);

        //Beginning
        v[0].position = sf::Vector2f(0.f, 0.f) + _trackOffset;
        v[1].position = sf::Vector2f(bSize.width, 0.f) + _trackOffset;
        v[2].position = sf::Vector2f(bSize.width, bSize.height) + _trackOffset;
        v[3].position = sf::Vector2f(0.f, bSize.height) + _trackOffset;
        if (bTex == _noTexture) bTexSize = defSixteen;
        v[0].texCoords = sf::Vector2f(bTexSize.left, bTexSize.top);
        v[1].texCoords = sf::Vector2f(bTexSize.left + bTexSize.width, bTexSize.top);
        v[2].texCoords = sf::Vector2f(bTexSize.left + bTexSize.width, bTexSize.top + bTexSize.height);
        v[3].texCoords = sf::Vector2f(bTexSize.left, bTexSize.top + bTexSize.height);
        rs.texture = bTex;
        _render.draw(v, rs);

        //Track
        if (_trackTexture != nullptr && _trackTexture->getSize().x > 0)
        {
            auto tkSize = _trackTexture->getSize();
            auto trackSize = sf::Vector2f(float(tkSize.x), float(tkSize.y));
            float multi = 1.f;
            if (_horizontalSource)
                multi = roundf(_trackLength / trackSize.x);
            else
                multi = roundf(_trackLength / trackSize.y);

            sf::Vector2f begOffset(0.f, 0.f);
            if (_horizontalSource)
                begOffset.x = bSize.width;
            else
                begOffset.y = bSize.height;

            if (_horizontalSource)
            {
                v[0].position = sf::Vector2f(0.f, 0.f) + _trackOffset + begOffset;
                v[1].position = sf::Vector2f(_trackLength, 0.f) + _trackOffset + begOffset;
                v[2].position = sf::Vector2f(_trackLength, trackSize.y) + _trackOffset + begOffset;
                v[3].position = sf::Vector2f(0.f, trackSize.y) + _trackOffset + begOffset;
                v[0].texCoords = sf::Vector2f(0.f, 0.f);
                v[1].texCoords = sf::Vector2f(multi * trackSize.x, 0.f);
                v[2].texCoords = sf::Vector2f(multi * trackSize.x, trackSize.y);
                v[3].texCoords = sf::Vector2f(0.f, trackSize.y);
            }
            else
            {
                v[0].position = sf::Vector2f(0.f, 0.f) + _trackOffset + begOffset;
                v[1].position = sf::Vector2f(trackSize.x, 0.f) + _trackOffset + begOffset;
                v[2].position = sf::Vector2f(trackSize.x, _trackLength) + _trackOffset + begOffset;
                v[3].position = sf::Vector2f(0.f, _trackLength) + _trackOffset + begOffset;
                v[0].texCoords = sf::Vector2f(0.f, 0.f);
                v[1].texCoords = sf::Vector2f(trackSize.x, 0.f);
                v[2].texCoords = sf::Vector2f(trackSize.x, multi * trackSize.y);
                v[3].texCoords = sf::Vector2f(0.f, multi * trackSize.y);
            }
            rs.texture = _trackTexture.get();
            _render.draw(v, rs);
        }

        //Ending
        sf::Vector2f endOffset(0.f, 0.f);
        if (_horizontalSource)
            endOffset.x = bSize.width + _trackLength;
        else
            endOffset.y = bSize.height + _trackLength;
        v[0].position = sf::Vector2f(0.f, 0.f) + _trackOffset + endOffset;
        v[1].position = sf::Vector2f(eSize.width, 0.f) + _trackOffset + endOffset;
        v[2].position = sf::Vector2f(eSize.width, eSize.height) + _trackOffset + endOffset;
        v[3].position = sf::Vector2f(0.f, eSize.height) + _trackOffset + endOffset;
        if (eTex == _noTexture) eTexSize = defSixteen;
        v[0].texCoords = sf::Vector2f(eTexSize.left, eTexSize.top);
        v[1].texCoords = sf::Vector2f(eTexSize.left + eTexSize.width, eTexSize.top);
        v[2].texCoords = sf::Vector2f(eTexSize.left + eTexSize.width, eTexSize.top + eTexSize.height);
        v[3].texCoords = sf::Vector2f(eTexSize.left, eTexSize.top + eTexSize.height);
        rs.texture = eTex;
        _render.draw(v, rs);

        //Thumb
        auto thumbTex = _thumb.GetTexture();
        auto thumbSize = thumbTex->getSize();
        auto& thumbPos = _thumb.getPosition();
        v[0].position = sf::Vector2f(thumbPos.x, thumbPos.y) + _thumbOffset;
        v[1].position = sf::Vector2f(thumbPos.x + float(thumbSize.x), thumbPos.y) + _thumbOffset;
        v[2].position = sf::Vector2f(thumbPos.x + float(thumbSize.x), thumbPos.y + float(thumbSize.y)) + _thumbOffset;
        v[3].position = sf::Vector2f(thumbPos.x, thumbPos.y + float(thumbSize.y)) + _thumbOffset;
        v[0].texCoords = sf::Vector2f(0.f, 0.f);
        v[1].texCoords = sf::Vector2f(float(thumbSize.x), 0.f);
        v[2].texCoords = sf::Vector2f(float(thumbSize.x), float(thumbSize.y));
        v[3].texCoords = sf::Vector2f(0.f, float(thumbSize.y));
        rs.texture = &thumbTex->getTexture();
        _render.draw(v, rs);

        _render.display();

        _sthChanged = false;
        _redrawHappened = true;
        return true;
    }
    return false;
}

void ScrollBar::ProcessEvent(sf::Event* ev, const sf::Vector2f& mousePos)
{
    auto relMouse = getTransform().getInverse().transformPoint(mousePos - _thumbOffset);
    _thumb.ProcessEvent(ev, relMouse);
    
    if (_mouseInput)
    {
        if (ev->type == sf::Event::MouseButtonPressed && _thumb.GetGlobalBounds().contains(relMouse) == false) _thumb.SetInFocus(false);

        //Scrolling
        if (GetInFocus() == true)
        {
            if (ev->type == sf::Event::MouseWheelScrolled)
            {
                if (_horizontalSource)
                    SetScroll(GetScroll() + (ev->mouseWheelScroll.delta * _scrollAmount));
                else
                    SetScroll(GetScroll() + (ev->mouseWheelScroll.delta * _scrollAmount * -1.f));
            }
            if (_thumb.Holding() == true)
            {
                if (_horizontalSource)
                    SetScroll((relMouse.x - (_thumb.GetBackgroundSize().x / 2)) / _trackLength);
                else
                    SetScroll((relMouse.y - (_thumb.GetBackgroundSize().y / 2)) / _trackLength);
            }
        }
    }
    if (_keyboardInput == true && GetInFocus() == true)
    {
        float scrollBy = (_horizontalSource != _horizontalInput) ? _scrollAmount * -1.f : _scrollAmount;

        if (_horizontalInput)
        {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) SetScroll(GetScroll() + scrollBy);
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))SetScroll(GetScroll() - scrollBy);
        }
        else
        {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) SetScroll(GetScroll() + scrollBy);
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))SetScroll(GetScroll() - scrollBy);
        }
    }
}

std::vector<sf::Vector2f> ScrollBar::GetAllBoundsPoints() const
{
    std::vector<sf::Vector2f> output = UIElement::GetAllBoundsPoints();
    auto thumb = _thumb.GetAllBoundsPoints(); 
    output.insert(output.end(), std::make_move_iterator(thumb.begin()), std::make_move_iterator(thumb.end()));
    for (size_t i = 4; i < output.size(); i++)
        output[i] = getTransform().transformPoint(output[i] + _thumbOffset);
    return output;
}

std::vector<sf::Vector2f> ScrollBar::GetDeepestInFocusBoundsPoints() const
{
    if (_thumb.GetInFocus())
    {
        auto output = _thumb.GetDeepestInFocusBoundsPoints();
        for (auto& p : output)
            p = getTransform().transformPoint(p + _thumbOffset);
        return output;
    }
    return UIElement::GetAllBoundsPoints();
}

ScrollBar::ScrollBar(SoundsManager* sounds, TexturesManager* textures)
{
    _soundsManager = sounds;
    _texturesManager = textures;
    _textures.clear();
    _trackTexture = nullptr;
    _trackOffset = sf::Vector2f(0.f, 0.f);
    _trackLength = 20.f;
    _horizontalSource = true;
    _horizontalInput = true;
    _scrollAmount = 0.1f;
    _thumbOffset = sf::Vector2f(0.f, 0.f);
    _thumb.setPosition(0.f, 0.f);
    _thumb.SetSoundsManager(_soundsManager);
    _thumb.SetTexturesManager(_texturesManager);
}

ScrollBar::ScrollBar(ScrollBar& other) : UIElement(other), _thumb(other._thumb)
{
    _textures = other._textures;
    _trackTexture = other._trackTexture;
    _trackOffset = other._trackOffset;
    _thumbOffset = other._thumbOffset;
    _trackLength = other._trackLength;
    _horizontalSource = other._horizontalSource;
    _horizontalInput = other._horizontalInput;
    _empty = other._empty;
    _scrollAmount = other._scrollAmount;
}

ScrollBar::~ScrollBar()
{
}

void ScrollBar::SetTrackTexture(const std::string& tmpName, const std::string& textureName, const sf::FloatRect& rect)
{
    if (_texturesManager->TmpExists(tmpName))
        _trackTexture = _texturesManager->GetTmpTexture(tmpName);
    else
    {
        auto iRect = sf::IntRect(int(ceilf(rect.left)), int(ceilf(rect.top)), int(ceilf(rect.width)), int(ceilf(rect.height)));
        _trackTexture = _texturesManager->CreateTmpTexture(tmpName, textureName, iRect);
        if(_trackTexture != nullptr)
            _trackTexture->setRepeated(true);
    }
    _sthChanged = true;
}

void ScrollBar::SetBeginningTexture(const std::string& name, const sf::FloatRect& rect)
{
    _textures["beginning"] = std::tuple<std::string, sf::FloatRect>(name, rect);
    _sthChanged = true;
}

void ScrollBar::SetEndingTexture(const std::string& name, const sf::FloatRect& rect)
{
    _textures["ending"] = std::tuple<std::string, sf::FloatRect>(name, rect);
    _sthChanged = true;
}

void ScrollBar::SetTrackLength(float length)
{
    _trackLength = length;
    _sthChanged = true;
}

void ScrollBar::SetThumbOffset(const sf::Vector2f& offset)
{
    _thumbOffset = offset;
    _sthChanged = true;
}

void ScrollBar::SetTrackOffset(const sf::Vector2f& offset)
{
    _trackOffset = offset;
    _sthChanged = true;
}

void ScrollBar::SetScroll(float percent)
{
    percent = std::max(0.f, std::min(percent, 1.f));

    if (_horizontalSource)
        _thumb.setPosition(roundf(_thumbOffset.x + (_trackLength * percent)), _thumbOffset.y);
    else
        _thumb.setPosition(_thumbOffset.x, roundf(_thumbOffset.y + (_trackLength * percent)));

    _sthChanged = true;
}

void ScrollBar::SetScrollAmount(float percent)
{
    _scrollAmount = std::max(0.f, std::min(percent, 1.f));
}

void ScrollBar::SetHorizontalSource(bool horizontalMode)
{
    _horizontalSource = horizontalMode;
    _sthChanged = true;
}

void ScrollBar::SetHorizontalInput(bool horizontalMode)
{
    _horizontalInput = horizontalMode;
}

void ScrollBar::ThumbAddState(const std::string& name, const std::string& textureName, const sf::FloatRect& backgroundRect)
{
    _thumb.AddState(name, _empty, textureName, backgroundRect);
}

void ScrollBar::ThumbRemoveState(const std::string& name)
{
    _thumb.RemoveState(name);
}

std::tuple<std::string, sf::FloatRect>* ScrollBar::ThumbEditBackgroundState(const std::string& name)
{
    return _thumb.EditBackgroundState(name);
}

void ScrollBar::SetThumbBackgroundSize(const sf::Vector2f& size)
{
    _thumb.Init(sf::Vector2u(uint32_t(ceilf(size.x)), uint32_t(ceilf(size.y))));
    _thumb.SetBackgroundSize(size);
}

bool ScrollBar::Clicked()
{
    return _thumb.Clicked();
}

const sf::Vector2f& ScrollBar::GetThumbBackgroundSize() const
{
    return _thumb.GetBackgroundSize();
}

float ScrollBar::GetTrackLength()
{
    return _trackLength;
}

const sf::Vector2f& ScrollBar::GetThumbOffset()
{
    return _thumbOffset;
}

const sf::Vector2f& ScrollBar::GetTrackOffset()
{
    return _trackOffset;
}

float ScrollBar::GetScroll()
{
    if (_horizontalSource)
    {
        auto pos = _thumb.getPosition() - _thumbOffset;
        return (pos.x / _trackLength);
    }
    else
    {
        auto pos = _thumb.getPosition() - _thumbOffset;
        return (pos.y / _trackLength);
    }
}

float ScrollBar::GetScrollAmount()
{
    return _scrollAmount;
}

bool ScrollBar::IsHorizontalSource()
{
    return _horizontalSource;
}

bool ScrollBar::IsHorizontalInput()
{
    return _horizontalInput;
}
