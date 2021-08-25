#pragma once

#include "Piece.hpp"

class Knight : public Piece {
	public:
		Knight(const std::string& boardPos, Color color, char id);

		std::vector<std::pair<int, int>> GetPossibleMoves(const std::vector<std::vector<Tile>>& board, Tile* pEnPassantTile = nullptr) const override;
		std::vector<std::pair<int, int>> GetAttackedTiles(const std::vector<std::vector<Tile>>& board) const override;
};
