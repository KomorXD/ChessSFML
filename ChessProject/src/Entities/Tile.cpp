#include "Tile.hpp"

Tile::Tile(const sf::Vector2f& pos, const std::string& boardPos)
	: m_Body({ 80.0f, 80.0f }), m_pPiece(nullptr), m_BoardPos(boardPos), m_Highlighted(false) {

	m_Body.setPosition(pos);
}

std::unique_ptr<Piece>& Tile::GetPiecePtr() {
	return m_pPiece;
}

const std::unique_ptr<Piece>& Tile::GetPiecePtr() const {
	return m_pPiece;
}

void Tile::UpdatePiecePos() {
	if (m_pPiece)
		m_pPiece->SetPosition({ m_Body.getPosition().x + m_Body.getSize().x / 2.0f, m_Body.getPosition().y + m_Body.getSize().y / 2.0f });
}

void Tile::RenderTile(sf::RenderTarget& renderer) {
	renderer.draw(m_Body);
}

void Tile::RenderPiece(sf::RenderTarget& renderer) {
	if (m_pPiece)
		m_pPiece->Render(renderer);
}

void Tile::SetTexture(sf::Texture* texture) {
	m_Body.setTexture(texture, true);
}

void Tile::SetPiece(std::unique_ptr<Piece>&& piece) {
	m_pPiece = std::move(piece);
	m_pPiece->SetBoardPosition(m_BoardPos);

	UpdatePiecePos();
}

void Tile::SetHighlighted(bool highlighted) {
	m_Highlighted = highlighted;
}

void Tile::EraseTile() {
	Piece* p = m_pPiece.release();

	delete p;
}

bool Tile::IsHoveredOver(const sf::Vector2i& mousePos) const {
	return m_Body.getGlobalBounds().contains((float)mousePos.x, (float)mousePos.y);
}

bool Tile::IsHighlighted() const {
	return m_Highlighted;
}

std::string Tile::GetBoardPos() const {
	return m_BoardPos;
}
