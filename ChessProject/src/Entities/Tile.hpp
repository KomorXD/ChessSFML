#pragma once

#include <SFML/Graphics.hpp>
#include <memory>

#include "Pieces/Piece.hpp"

class Tile {
	private:
		sf::RectangleShape	   m_Body;
		std::unique_ptr<Piece> m_pPiece;
		std::string			   m_BoardPos;

		bool m_Highlighted;

	public:
		Tile(const sf::Vector2f& pos, const std::string& boardPos);

		std::unique_ptr<Piece>& GetPiecePtr();
		const std::unique_ptr<Piece>& GetPiecePtr() const;

		void UpdatePiecePos();
		void RenderTile(sf::RenderTarget& renderer);
		void RenderPiece(sf::RenderTarget& renderer);

		void SetTexture(sf::Texture* texture);
		void SetPiece(std::unique_ptr<Piece>&& piece);
		void SetHighlighted(bool highlighted);
		void EraseTile();

		bool IsHoveredOver(const sf::Vector2i& mousePos) const;
		bool IsHighlighted() const;
		std::string GetBoardPos() const;
};