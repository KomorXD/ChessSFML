#pragma once

#include "Widget.hpp"

class Textbox : public Widget {
	protected:
		Rectangle m_Box;
		Text	  m_Text;

	public:
		Textbox(const sf::Vector2f& size);
		Textbox(const sf::Vector2f& size, const std::string& text);

		void SetText(const std::string& text);
		void SetFontSize(unsigned int size);
		void SetBgColor(const sf::Color& color);

		void HandleEvents(const sf::RenderWindow& window, sf::Event& event) override;
		void Render(sf::RenderTarget& renderer)							    override;
		void SetPosition(const sf::Vector2f& pos)							override;
		sf::Vector2f GetSize() const										override;
};