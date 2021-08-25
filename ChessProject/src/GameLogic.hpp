#pragma once

#include <vector>
#include <memory>

class ChessState;
class Tile;
class Piece;
class King;
enum class Color;

enum class MatchState {
	NORMAL,
	TIMED_OUT,
	CHECKED,
	CHECKMATED,
	STALEMATED
};

class GameLogic {
	private:
		ChessState* m_pChess;

		bool IsValidMove(int x, int y, std::vector<std::vector<Tile>>& board, std::unique_ptr<Piece>& pHovered, King* pKing, Color currColor);
		bool IsValidKingMove(int x, int y, std::vector<std::vector<Tile>>& board, King* pKing, Color currColor);
		bool Checkmate(std::vector<std::vector<Tile>>& board, Color currColor);
		bool CanCastle(int x, int y, std::vector<std::vector<Tile>>& board, King* pKing, Color currColor);
		King* GetKingPtr(const std::vector<std::vector<Tile>>& board, Color currColor);

	public:
		GameLogic(ChessState* pPtr);

		std::vector<std::pair<int, int>> ValidMoves(std::vector<std::vector<Tile>>& board, std::vector<std::pair<int, int>>& moves, std::unique_ptr<Piece>& pHovered, Color currColor);
		MatchState AnalyzeBoard(std::vector<std::vector<Tile>>& board, Color currColor);
};