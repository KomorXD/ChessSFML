#pragma once

#include <SFML/Graphics.hpp>

class Widget {
	protected:
		class Rectangle : public sf::RectangleShape {
			public:
				Rectangle(const sf::Vector2f& size);

				bool IsRolledOn(const sf::RenderWindow& window) const;
				bool IsClickedOn(const sf::RenderWindow& window, sf::Event& ev) const;
		};

		class Text : public sf::Text {
			public:
				Text();

				void SetFont(const std::string& name);
				void SetFontSize(unsigned size);
				void UpdateText();
		};

	public:
		virtual ~Widget() = default;

		virtual void HandleEvents(const sf::RenderWindow& window, sf::Event& ev) = 0;
		virtual void Render(sf::RenderTarget& renderer)							 = 0;
		virtual void SetPosition(const sf::Vector2f& pos)						 = 0;
		virtual sf::Vector2f GetSize() const									 = 0;
};