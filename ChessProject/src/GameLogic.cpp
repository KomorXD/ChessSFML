#include "GameLogic.hpp"
#include "ChessState.hpp"
#include "Entities/Pieces/Queen.hpp"
#include "Entities/Pieces/King.hpp"
#include "Helper.hpp"

bool GameLogic::IsValidMove(int x, int y, std::vector<std::vector<Tile>>& board, std::unique_ptr<Piece>& pHovered, King* pKing, Color currColor) {
	std::unique_ptr<Piece> potential = std::move(board[y - 1][x - 1].GetPiecePtr());

	auto [kingX, kingY]			= Helper::Get().StringToPos(pKing->GetBoardPos());
	auto [originalX, originalY] = Helper::Get().StringToPos(pHovered->GetBoardPos());
	
	board[y - 1][x - 1].SetPiece(std::move(pHovered));

	bool isValid = IsValidKingMove(kingX, kingY, board, pKing, currColor);
	std::unique_ptr<Piece> original = std::move(board[y - 1][x - 1].GetPiecePtr());

	pHovered = std::move(original);

	if(potential)
		board[y - 1][x - 1].SetPiece(std::move(potential));

	return isValid;
}

bool GameLogic::IsValidKingMove(int x, int y, std::vector<std::vector<Tile>>& board, King* pKing, Color currColor) {
	std::vector<std::pair<int, int>> attackedTiles;

	for (auto& row : board) {
		for (auto& tile : row) {
			Piece* pPiece = tile.GetPiecePtr().get();

			if (pPiece) {
				if (pPiece->GetColor() != currColor) {
					std::vector<std::pair<int, int>> temp = pPiece->GetAttackedTiles(board);

					attackedTiles.insert(attackedTiles.end(), temp.begin(), temp.end());
				}
			}
		}
	}

	return std::find(attackedTiles.begin(), attackedTiles.end(), std::make_pair(x, y)) == attackedTiles.end();
}

bool GameLogic::Checkmate(std::vector<std::vector<Tile>>& board, Color currColor) {
	for (auto& row : board) {
		for (auto& tile : row) {
			std::unique_ptr<Piece> piece = std::move(tile.GetPiecePtr());

			if (piece && piece->GetColor() == currColor) {
				std::vector<std::pair<int, int>> possibleMoves = piece->GetPossibleMoves(board, m_pChess->GetEnPassantTile());
				std::vector<std::pair<int, int>> validMoves = ValidMoves(board, possibleMoves, piece, currColor);

				if (!validMoves.empty()) {
					tile.SetPiece(std::move(piece));

					return false;
				}
			}

			if(piece)
				tile.SetPiece(std::move(piece));
		}
	}

	return true;
}

bool GameLogic::CanCastle(int x, int y, std::vector<std::vector<Tile>>& board, King* pKing, Color currColor) {
	if (pKing->HasMoved())
		return false;

	auto [posX, posY] = Helper::Get().StringToPos(pKing->GetBoardPos());
	int  step		  = (x - posX) / std::abs(x - posX);
	bool freePath	  = true;
	char rookID		  = currColor == Color::WHITE ? 'R' : 'r';
	std::vector<std::pair<int, int>> tilesVisited{ {posX, posY} };

	while (posX >= 1 && posX <= 8) {
		Piece* pPiece = board[posY - 1][posX - 1].GetPiecePtr().get();

		if (pPiece) {
			if (pPiece->GetColor() != pKing->GetColor() || (pPiece->GetColor() == pKing->GetColor() && *pPiece != rookID)
			|| (pPiece->GetColor() == pKing->GetColor() && *pPiece == rookID && pPiece->HasMoved())) {
				freePath = false;

				break;
			}
		}

		tilesVisited.push_back({ posX, posY });
		posX += step;
	}

	if (!freePath)
		return false;

	if (posX < 1 && !board[posY - 1][0].GetPiecePtr())
		return false;

	if (!board[posY - 1][7].GetPiecePtr())
		return false;

	for (auto& row : board) {
		for (auto& tile : row) {
			if (tile.GetPiecePtr() && tile.GetPiecePtr()->GetColor() != currColor) {
				std::vector<std::pair<int, int>> temp = tile.GetPiecePtr()->GetAttackedTiles(board);

				for (auto& tile : tilesVisited) {
					if (std::find(temp.begin(), temp.end(), tile) != temp.end())
						return false;
				}
			}
		}
	}

	return true;
}

King* GameLogic::GetKingPtr(const std::vector<std::vector<Tile>>& board, Color currColor) {
	King* pKing = nullptr;
	char kingID = currColor == Color::WHITE ? 'K' : 'k';

	for (auto& row : board) {
		for (auto& tile : row) {
			if (tile.GetPiecePtr() && *tile.GetPiecePtr() == kingID) {
				pKing = (King*)tile.GetPiecePtr().get();

				break;
			}
		}

		if (pKing)
			break;
	}

	return pKing;
}

GameLogic::GameLogic(ChessState* pPtr)
	: m_pChess(pPtr) { }

std::vector<std::pair<int, int>> GameLogic::ValidMoves(std::vector<std::vector<Tile>>& board, std::vector<std::pair<int, int>>& moves, std::unique_ptr<Piece>& pHovered, Color currColor) {
	std::vector<std::pair<int, int>> out;

	King* pKing = GetKingPtr(board, currColor);

	if (!pKing)
		pKing = (King*)pHovered.get();

	for (auto [moveX, moveY] : moves) {
		if (pHovered.get() != pKing) {
			if (IsValidMove(moveX, moveY, board, pHovered, pKing, currColor))
				out.push_back({ moveX, moveY });
		}
		else {
			if (std::abs(moveX - (int)(pKing->GetBoardPos()[0] - 96)) > 1) {
				if (CanCastle(moveX, moveY, board, pKing, currColor))
					out.push_back({ moveX, moveY });
			}
			else {
				std::unique_ptr<Piece> piece = std::move(board[moveY - 1][moveX - 1].GetPiecePtr());

				if (IsValidKingMove(moveX, moveY, board, pKing, currColor))
					out.push_back({ moveX, moveY });

				if (piece)
					board[moveY - 1][moveX - 1].SetPiece(std::move(piece));
			}
		}
	}

	return out;
}

MatchState GameLogic::AnalyzeBoard(std::vector<std::vector<Tile>>& board, Color currColor) {
	King* pKing = GetKingPtr(board, currColor);

	for (auto& row : board) {
		for (auto& tile : row) {
			std::unique_ptr<Piece> piece = std::move(tile.GetPiecePtr());
			
			if (piece && (*piece == 'p' || *piece == 'P') && (piece->GetBoardPos()[1] == '1' || piece->GetBoardPos()[1] == '8'))
				tile.SetPiece(std::make_unique<Queen>(piece->GetBoardPos(), piece->GetColor(), piece->GetColor() == Color::WHITE ? 'Q' : 'q'));
			else if(piece)
				tile.SetPiece(std::move(piece));
		}
	}

	auto [posX, posY] = Helper::Get().StringToPos(pKing->GetBoardPos());
	bool checked = !IsValidKingMove(posX, posY, board, pKing, currColor);

	if (Checkmate(board, currColor))
		return checked ? MatchState::CHECKMATED : MatchState::STALEMATED;

	return checked ? MatchState::CHECKED : MatchState::NORMAL;
}
