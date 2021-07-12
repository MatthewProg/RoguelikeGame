#pragma once

#include "../UI/UIElement.h"
#include "../UI/Button.h"

class ScrollBar : public UIElement
{
private:
	std::map<std::string, std::tuple<std::string, sf::FloatRect>> _textures;
	std::shared_ptr<sf::Texture> _trackTexture;

	sf::Vector2f _trackOffset;
	sf::Vector2f _thumbOffset;

	Button _thumb;
	sf::Text _empty;

	float _trackLength;
	float _scrollAmount;
	bool _horizontalSource;
	bool _horizontalInput;
	bool _sthChanged;
public:
	ScrollBar(SoundsManager* sounds, TexturesManager* textures);
	ScrollBar(ScrollBar& other);
	~ScrollBar();

	void SetTrackTexture(const std::string& tmpName, const std::string& textureName, const sf::FloatRect& rect);
	void SetBeginningTexture(const std::string& name, const sf::FloatRect& rect);
	void SetEndingTexture(const std::string& name, const sf::FloatRect& rect);
	void SetTrackLength(float length);
	void SetThumbOffset(const sf::Vector2f& offset);
	void SetTrackOffset(const sf::Vector2f& offset);
	void SetScroll(float percent);
	void SetScrollAmount(float percent);
	void SetHorizontalSource(bool horizontalMode);
	void SetHorizontalInput(bool horizontalMode);

	void ThumbAddState(const std::string& name, const std::string& textureName, const sf::FloatRect& backgroundRect);
	void ThumbRemoveState(const std::string& name);
	std::tuple<std::string, sf::FloatRect>* ThumbEditBackgroundState(const std::string& name);
	void SetThumbBackgroundSize(const sf::Vector2f& size);

	bool Clicked();
	const sf::Vector2f& GetThumbBackgroundSize() const;

	float GetTrackLength();
	const sf::Vector2f& GetThumbOffset();
	const sf::Vector2f& GetTrackOffset();
	float GetScroll();
	float GetScrollAmount();
	bool IsHorizontalSource();
	bool IsHorizontalInput();

	// Inherited via UIElement
	virtual UIElement* clone() override;
	virtual void Update(bool tick, float delta) override;
	virtual void RedrawElement() override;
	virtual void ProcessEvent(sf::Event* ev, const sf::Vector2f& mousePos) override;
	virtual std::vector<sf::Vector2f> GetAllBoundsPoints() const override;
	virtual std::vector<sf::Vector2f> GetDeepestInFocusBoundsPoints() const override;
};

