#include "King.hpp"
#include "../Tile.hpp"

King::King(const std::string& boardPos, Color color, char id)
	: Piece(boardPos, color, id) {

	if (color == Color::BLACK)
		m_Body.setTexture(&ResourceHolder::Get().GetTexture("king_black"));
	else
		m_Body.setTexture(&ResourceHolder::Get().GetTexture("king_white"));
}

std::vector<std::pair<int, int>> King::GetPossibleMoves(const std::vector<std::vector<Tile>>& board, Tile* pEnPassantTile) const {
	int x = m_BoardPos[0] - 96;
	int y = m_BoardPos[1] - 48;
	std::vector<std::pair<int, int>> intermediate{ {x, y - 1}, {x + 1, y - 1}, {x + 1, y}, {x + 1, y + 1}, {x, y + 1}, {x - 1, y + 1}, {x - 1, y}, {x - 1, y - 1} };
	std::vector<std::pair<int, int>> out;

	if (!m_HasMoved) {
		intermediate.push_back({ x + 2, y });
		intermediate.push_back({ x - 2, y });
	}

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

std::vector<std::pair<int, int>> King::GetAttackedTiles(const std::vector<std::vector<Tile>>& board) const {
	int x = m_BoardPos[0] - 96;
	int y = m_BoardPos[1] - 48;
	std::vector<std::pair<int, int>> intermediate = GetPossibleMoves(board);
	std::vector<std::pair<int, int>> out;

	for (auto& [moveX, moveY] : intermediate) {
		if (std::abs(x - moveX) <= 1)
			out.push_back({ moveX, moveY });
	}

	return out;
}
