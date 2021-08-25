#include "Piece.hpp"

Piece::Piece(const std::string& boardPos, Color color, char id)
	: m_Body({ 60.0f, 60.0f }), m_BoardPos(boardPos), m_Color(color), m_ID(id), m_HasMoved(false) {

	m_Body.setOrigin(m_Body.getSize() / 2.0f);
}

void Piece::Render(sf::RenderTarget& renderer) {
	renderer.draw(m_Body);
}

void Piece::SetPosition(const sf::Vector2f& pos) {
	m_Body.setPosition(pos);
}

void Piece::SetBoardPosition(const std::string& boardPos) {
	m_BoardPos = boardPos;
}

void Piece::SetMoved() {
	m_HasMoved = true;
}

Color Piece::GetColor() const {
	return m_Color;
}

std::string Piece::GetBoardPos() const {
	return m_BoardPos;
}

char Piece::GetID() const {
	return m_ID;
}

bool Piece::HasMoved() const {
	return m_HasMoved;
}

bool Piece::operator ==(char id) {
	return m_ID == id;
}

bool Piece::operator !=(char id) {
	return m_ID != id;
}
