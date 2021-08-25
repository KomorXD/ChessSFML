#include "Bishop.hpp"
#include "../Tile.hpp"

Bishop::Bishop(const std::string& boardPos, Color color, char id)
	: Piece(boardPos, color, id) {

	if (color == Color::BLACK)
		m_Body.setTexture(&ResourceHolder::Get().GetTexture("bishop_black"));
	else
		m_Body.setTexture(&ResourceHolder::Get().GetTexture("bishop_white"));
}

std::vector<std::pair<int, int>> Bishop::GetPossibleMoves(const std::vector<std::vector<Tile>>& board, Tile* pEnPassantTile) const {
	int x = m_BoardPos[0] - 96;
	int y = m_BoardPos[1] - 48;
	int moveX = x - 1;
	int moveY = y - 1;
	std::vector<std::pair<int, int>> out;

	while (moveX > 0 && moveY > 0) {
		Piece* pPiece = board[moveY - 1][moveX - 1].GetPiecePtr().get();

		if (!pPiece)
			out.push_back({ moveX, moveY });
		else {
			if (pPiece->GetColor() != m_Color)
				out.push_back({ moveX, moveY });

			break;
		}

		--moveX;
		--moveY;
	}

	moveX = x + 1;
	moveY = y + 1;

	while (moveX <= 8 && moveY <= 8) {
		Piece* pPiece = board[moveY - 1][moveX - 1].GetPiecePtr().get();

		if (!pPiece)
			out.push_back({ moveX, moveY });
		else {
			if (pPiece->GetColor() != m_Color)
				out.push_back({ moveX, moveY });

			break;
		}

		++moveX;
		++moveY;
	}

	moveX = x - 1;
	moveY = y + 1;

	while (moveX > 0 && moveY <= 8) {
		Piece* pPiece = board[moveY - 1][moveX - 1].GetPiecePtr().get();

		if (!pPiece)
			out.push_back({ moveX, moveY });
		else {
			if (pPiece->GetColor() != m_Color)
				out.push_back({ moveX, moveY });

			break;
		}

		--moveX;
		++moveY;
	}

	moveX = x + 1;
	moveY = y - 1;

	while (moveX <= 8 && moveY > 0) {
		Piece* pPiece = board[moveY - 1][moveX - 1].GetPiecePtr().get();

		if (!pPiece)
			out.push_back({ moveX, moveY });
		else {
			if (pPiece->GetColor() != m_Color)
				out.push_back({ moveX, moveY });

			break;
		}

		++moveX;
		--moveY;
	}

	return out;
}

std::vector<std::pair<int, int>> Bishop::GetAttackedTiles(const std::vector<std::vector<Tile>>& board) const {
	return GetPossibleMoves(board);
}
