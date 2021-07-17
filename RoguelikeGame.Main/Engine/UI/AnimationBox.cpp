#include "AnimationBox.h"

AnimationBox::AnimationBox()
{
    _animation.SetLoop(true);
    _animation.setPosition(0.f, 0.f);
    _keyboardInput = false;
    _mouseInput = false;
}

AnimationBox::AnimationBox(AnimationBox& other) : UIElement(other)
{
    _animation = other._animation;
}

AnimationBox::~AnimationBox()
{
}

void AnimationBox::SetAnimation(const sf::Animation& anim)
{
    _animation = anim;
    _sthChanged = true;
}

sf::Animation* AnimationBox::GetAnimation()
{
    return &_animation;
}

UIElement* AnimationBox::clone()
{
    return new AnimationBox(*this);
}

void AnimationBox::Update(bool tick, float)
{
    _animation.Tick(tick);
    if (_animation.IsFrameSwitched()) _sthChanged = true;

    Redraw();
}

void AnimationBox::ForceRedraw()
{
    _render.clear(sf::Color::Transparent);
    if(_isVisible)
        _render.draw(_animation);
    _render.display();

    _redrawHappened = true;
    _sthChanged = false;
}

void AnimationBox::ProcessEvent(sf::Event*, const sf::Vector2f&)
{
}
