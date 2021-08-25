#include "Textbox.hpp"

Textbox::Textbox(const sf::Vector2f& size)
	: m_Box(size) {

	m_Box.setFillColor({ 66, 66, 66 });
	m_Box.setOrigin(m_Box.getSize() / 2.0f);

	m_Text.UpdateText();
}

Textbox::Textbox(const sf::Vector2f& size, const std::string& text)
	: m_Box(size) {

	m_Box.setFillColor({ 46, 46, 46 });
	m_Box.setOrigin(m_Box.getSize() / 2.0f);

	m_Text.setString(text);
	m_Text.UpdateText();
}

void Textbox::SetText(const std::string& text) {
	m_Text.setString(text);
	m_Text.UpdateText();
}

void Textbox::SetFontSize(unsigned int size) {
	m_Text.setCharacterSize(size);
	m_Text.UpdateText();
}

void Textbox::SetBgColor(const sf::Color& color) {
	m_Box.setFillColor(color);
}

void Textbox::HandleEvents(const sf::RenderWindow& window, sf::Event& event) {
	;
}

void Textbox::Render(sf::RenderTarget& renderer) {
	renderer.draw(m_Box);
	renderer.draw(m_Text);
}

void Textbox::SetPosition(const sf::Vector2f& pos) {
	m_Box.setPosition(pos);
	m_Text.setPosition(pos);

	m_Text.UpdateText();
}

sf::Vector2f Textbox::GetSize() const {
	return m_Box.getSize();
}