#include "Widget.hpp"
#include "../ResManagers/ResourceHolder.hpp"

Widget::Rectangle::Rectangle(const sf::Vector2f& size)
	: sf::RectangleShape(size) { }

bool Widget::Rectangle::IsRolledOn(const sf::RenderWindow& window) const {
	sf::Vector2i mousePos = sf::Mouse::getPosition(window);

	return getGlobalBounds().contains(sf::Vector2f(mousePos));
}

bool Widget::Rectangle::IsClickedOn(const sf::RenderWindow& window, sf::Event& ev) const {
	if (IsRolledOn(window) && ev.type == sf::Event::MouseButtonPressed)
		return ev.mouseButton.button == sf::Mouse::Left;

	return false;
}

Widget::Text::Text() {
	setFillColor(sf::Color::White);
	setOutlineColor(sf::Color::Black);
	setOutlineThickness(1.0f);
	setFont(ResourceHolder::Get().GetFont("OpenSans"));
}

void Widget::Text::SetFont(const std::string& name) {
	setFont(ResourceHolder::Get().GetFont(name));
}

void Widget::Text::SetFontSize(unsigned size) {
	setCharacterSize(size);
}

void Widget::Text::UpdateText() {
	sf::FloatRect textRect = getLocalBounds();

	setOrigin(textRect.left + textRect.width / 2.0f,
			  textRect.top + textRect.height / 2.0f);
}
