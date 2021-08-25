#pragma once

#include <vector>

#include "Tile.hpp"
#include "Pieces/Piece.hpp"
#include "../GUI/Textbox.hpp"

class ChessState;

class Board {
	private:
		std::vector<std::vector<Tile>> m_Board;
		std::vector<Textbox>		   m_Indexes;

		void BoardInit(const sf::Vector2f& windSize, const std::string& fen, ChessState* cs);
		bool BoardInitPositions(const sf::Vector2f& windSize, const std::string& fen);

	public:
		Board(const sf::Vector2f& windSize, const std::string& fileName, ChessState* cs);
		~Board();

		void Render(sf::RenderTarget& renderer);
		void HighlightTiles(std::vector<std::pair<int, int>>& moves);
		void UnhighlightTiles();

		std::unique_ptr<Piece>&& GetCurrentlyHoveredPiece(const sf::RenderWindow& window);
		Tile* GetCurrentlyHoveredTile(const sf::RenderWindow& window);
		std::vector<std::vector<Tile>>& GetBoard();
};