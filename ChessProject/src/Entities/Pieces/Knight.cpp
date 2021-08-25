#include "Knight.hpp"
#include "../Tile.hpp"

Knight::Knight(const std::string& boardPos, Color color, char id)
	: Piece(boardPos, color, id) {

	if (color == Color::BLACK)
		m_Body.setTexture(&ResourceHolder::Get().GetTexture("knight_black"));
	else
		m_Body.setTexture(&ResourceHolder::Get().GetTexture("knight_white"));
}

std::vector<std::pair<int, int>> Knight::GetPossibleMoves(const std::vector<std::vector<Tile>>& board, Tile* pEnPassantTile) const {
	int x = m_BoardPos[0] - 96;
	int y = m_BoardPos[1] - 48;
	std::vector<std::pair<int, int>> intermediate{ {x - 1, y - 2}, {x + 1, y - 2}, {x + 2, y - 1}, {x + 2, y + 1}, {x + 1, y + 2}, {x - 1, y + 2}, {x - 2, y + 1}, {x - 2, y - 1} };
	std::vector<std::pair<int, int>> out;
	
	for (auto& [moveX, moveY] : intermediate) {
		if (moveX > 0 && moveX <= 8 && moveY > 0 && moveY <= 8) {
			Piece* pPiece = board[moveY - 1][moveX - 1].GetPiecePtr().get();

			if (pPiece) {
				if (pPiece->GetColor() != m_Color)
					out.push_back({ moveX, moveY });
			}
			else
				out.push_back({ moveX, moveY });
		}
	}

	return out;
}

std::vector<std::pair<int, int>> Knight::GetAttackedTiles(const std::vector<std::vector<Tile>>& board) const {
	return GetPossibleMoves(board);
}
