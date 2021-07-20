#pragma once

#include "../UI/UIElement.h"
#include "../Utilities/Animation.h"

class AnimationBox : public UIElement
{
private:
	sf::Animation _animation;

public:
	AnimationBox();
	AnimationBox(AnimationBox& other);
	~AnimationBox() override = default;

	//Setters
	void SetAnimation(const sf::Animation& anim);

	//Getters
	sf::Animation* GetAnimation();

	// Inherited via UIElement
	UIElement* clone() override;
	void Update(bool tick, float) override;
	void ForceRedraw() override;
	void ProcessEvent(sf::Event*, const sf::Vector2f&) override;
};

