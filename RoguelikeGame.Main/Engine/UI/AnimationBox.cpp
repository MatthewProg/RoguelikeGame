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
    if (_animation.IsFrameSwitched())
        RedrawElement();
}

void AnimationBox::RedrawElement()
{
    _render.clear(sf::Color::Transparent);
    _render.draw(_animation);
    _render.display();
}

void AnimationBox::ProcessEvent(sf::Event*, const sf::Vector2f&)
{
}
