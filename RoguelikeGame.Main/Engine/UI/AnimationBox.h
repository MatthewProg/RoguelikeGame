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
	~AnimationBox();

	//Setters
	void SetAnimation(sf::Animation anim);

	//Getters
	sf::Animation* GetAnimation();

	// Inherited via UIElement
	virtual UIElement* clone() override;
	virtual void Update(bool tick, float delta) override;
	virtual void RedrawElement() override;
	virtual void ProcessEvent(sf::Event* ev, sf::Vector2f mousePos) override;
};

