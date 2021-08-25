#include "Pawn.hpp"
#include "../Tile.hpp"

Pawn::Pawn(const std::string& boardPos, Color color, char id)
	: Piece(boardPos, color, id) {
	
	if(color == Color::BLACK)
		m_Body.setTexture(&ResourceHolder::Get().GetTexture("pawn_black"));
	else
		m_Body.setTexture(&ResourceHolder::Get().GetTexture("pawn_white"));
}

std::vector<std::pair<int, int>> Pawn::GetPossibleMoves(const std::vector<std::vector<Tile>>& board, Tile* pEnPassantTile) const {
	int x = m_BoardPos[0] - 96;
	int y = m_BoardPos[1] - 48;
	int dir = m_Color == Color::WHITE ? 1 : -1;
	std::vector<std::pair<int, int>> intermediate{ {x - 1, y + dir}, {x, y + dir}, {x + 1, y + dir} };
	std::vector<std::pair<int, int>> out;

	if (!m_HasMoved)
		intermediate.push_back({ x, y + 2 * dir });

	for (auto& [moveX, moveY] : intermediate) {
		if (moveX > 0 && moveX <= 8 && moveY > 0 && moveY <= 8) {
			Piece* pPiece = board[moveY - 1][moveX - 1].GetPiecePtr().get();
			
			if (pPiece && moveX != x) {
				if (pPiece->GetColor() != m_Color)
					out.push_back({ moveX, moveY });
			}
			else if(!pPiece && moveX == x)
				out.push_back({ moveX, moveY });
			else if (!pPiece && moveX != x && pEnPassantTile && (int)(pEnPassantTile->GetBoardPos()[0] - 96) == moveX && (int)(pEnPassantTile->GetBoardPos()[1] - 48) == moveY)
				out.push_back({ moveX, moveY });
		}
	}

	return out;
}

std::vector<std::pair<int, int>> Pawn::GetAttackedTiles(const std::vector<std::vector<Tile>>& board) const {
	int x = m_BoardPos[0] - 96;
	int y = m_BoardPos[1] - 48;
	std::vector<std::pair<int, int>> out;
	int dir = m_Color == Color::WHITE ? 1 : -1;

	if (x > 1)
		out.push_back({ x - 1, y + dir });
	if (x < 8)
		out.push_back({ x + 1, y + dir });
	
	return out;
}
