#include "Board.hpp"
#include "Pieces/Pawn.hpp"
#include "Pieces/Rook.hpp"
#include "Pieces/Knight.hpp"
#include "Pieces/Bishop.hpp"
#include "Pieces/Queen.hpp"
#include "Pieces/King.hpp"
#include "../ResManagers/ResourceHolder.hpp"
#include "../ChessState.hpp"
#include "../Helper.hpp"

#include <fstream>

void Board::BoardInit(const sf::Vector2f& windSize, const std::string& fen, ChessState* cs) {
	std::vector<std::string> parts = Helper::Get().StringSplit(fen);

	if (parts.size() != 6) {
		BoardInit(windSize, "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1", cs);

		return;
	}

	if (!BoardInitPositions(windSize, parts[0])) {
		BoardInit(windSize, "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1", cs);

		return;
	}

	if (parts[1] == "b")
		cs->SetActiveColor(Color::BLACK);

	if (parts[2] != "-") {
		if (parts[2].find('K') == std::string::npos)
			cs->UnsetCastleKingSide(Color::WHITE);
		if (parts[2].find('Q') == std::string::npos)
			cs->UnsetCastleQueenSide(Color::WHITE);
		if (parts[2].find('k') == std::string::npos)
			cs->UnsetCastleKingSide(Color::BLACK);
		if (parts[2].find('q') == std::string::npos)
			cs->UnsetCastleQueenSide(Color::BLACK);
	}

	if (parts[3] != "-") {
		if (parts[3].length() == 2 && ((parts[3][0] >= 'a' && parts[3][0] <= 'h') || (parts[3][0] >= 'A' && parts[3][0] <= 'H')) && (parts[3][1] == '3' || parts[3][1] == '6'))
			cs->SetEnPassant(parts[3]);
	}

	bool isNumber = true;

	for (auto& c : parts[4]) {
		if (c < '0' || c > '9') {
			isNumber = false;

			break;
		}
	}

	if (isNumber)
		cs->SetHalfMoves(std::stoi(parts[4]));

	isNumber = true;

	for (auto& c : parts[4]) {
		if (c < '0' || c > '9') {
			isNumber = false;

			break;
		}
	}

	if (isNumber)
		cs->SetFullMoves(std::stoi(parts[5]));
}

bool Board::BoardInitPositions(const sf::Vector2f& windSize, const std::string& fen) {
	std::size_t row = 7;
	std::size_t col = 0;
	int kings = 0;

	for (auto& c : fen) {
		if (c == '/') {
			--row;
			col = 0;
		}
		else if (c >= '1' && c <= '8')
			col += (int)c - 48;
		else {
			Color color = c >= 'a' && c <= 'z' ? Color::BLACK : Color::WHITE;
			std::string currPos = "";
			int letterCase = color == Color::BLACK ? 0 : -32;

			currPos += (char)(col + 97);
			currPos += (char)(row + 49);

			switch (c) {
			case 'p':
			case 'P':
				m_Board[row][col++].SetPiece(std::make_unique<Pawn>(currPos, color, 'p' + letterCase));

				if ((color == Color::WHITE && row != 1) || (color == Color::BLACK && row != 6))
					m_Board[row][col - 1].GetPiecePtr()->SetMoved();

				break;
			case 'r':
			case 'R':
				m_Board[row][col++].SetPiece(std::make_unique<Rook>(currPos, color, 'r' + letterCase));
				break;
			case 'n':
			case 'N':
				m_Board[row][col++].SetPiece(std::make_unique<Knight>(currPos, color, 'n' + letterCase));
				break;
			case 'b':
			case 'B':
				m_Board[row][col++].SetPiece(std::make_unique<Bishop>(currPos, color, 'b' + letterCase));
				break;
			case 'q':
			case 'Q':
				m_Board[row][col++].SetPiece(std::make_unique<Queen>(currPos, color, 'q' + letterCase));
				break;
			case 'k':
			case 'K':
				m_Board[row][col++].SetPiece(std::make_unique<King>(currPos, color, 'k' + letterCase));
				++kings;
				break;
			}
		}
	}

	if (kings != 2) {
		for (auto& row : m_Board) {
			for (auto& tile : row) {
				if (tile.GetPiecePtr())
					tile.EraseTile();
			}
		}

		return false;
	}

	return true;
}

Board::Board(const sf::Vector2f& windSize, const std::string& fileName, ChessState* cs) {
	ResourceHolder::Get().AddTexture("pawn_black");
	ResourceHolder::Get().AddTexture("pawn_white");
	ResourceHolder::Get().AddTexture("rook_black");
	ResourceHolder::Get().AddTexture("rook_white");
	ResourceHolder::Get().AddTexture("knight_black");
	ResourceHolder::Get().AddTexture("knight_white");
	ResourceHolder::Get().AddTexture("bishop_black");
	ResourceHolder::Get().AddTexture("bishop_white");
	ResourceHolder::Get().AddTexture("queen_black");
	ResourceHolder::Get().AddTexture("queen_white");
	ResourceHolder::Get().AddTexture("king_black");
	ResourceHolder::Get().AddTexture("king_white");
	ResourceHolder::Get().AddTexture("tile_light");
	ResourceHolder::Get().AddTexture("tile_dark");
	ResourceHolder::Get().AddTexture("tile_highlight");

	m_Board.reserve(8);
	m_Indexes.reserve(16);

	for (std::size_t row = 0; row < 8; ++row) {
		bool isDark = row % 2 == 1;

		m_Board.push_back(std::vector<Tile>());
		m_Board[row].reserve(8);

		for (std::size_t col = 0; col < 8; ++col) {
			std::string currPos = "";

			currPos += (char)(col + 97);
			currPos += (char)(row + 49);

			m_Board[row].emplace_back(sf::Vector2f(windSize.x / 4.0f + col * 80.0f, windSize.y / 15.0f + (8 - row - 1) * 80.0f), currPos);

			if (isDark)
				m_Board[row].back().SetTexture(&ResourceHolder::Get().GetTexture("tile_dark"));
			else
				m_Board[row].back().SetTexture(&ResourceHolder::Get().GetTexture("tile_light"));

			isDark = !isDark;
		}
	}

	for (std::size_t i = 0; i < 8; ++i) {
		std::string temp = "";

		temp += (char)(i + 49);

		m_Indexes.emplace_back(sf::Vector2f(80.0f, 80.0f), temp);
		m_Indexes.back().SetPosition({ windSize.x / 4.0f - 40.0f, windSize.y / 15.0f + (8 - i - 1) * 80.0f + 40.0f });
		m_Indexes.back().SetBgColor({ 33, 33, 33 });
	}

	for (std::size_t i = 0; i < 8; ++i) {
		std::string temp = "";

		temp += (char)(i + 97);

		m_Indexes.emplace_back(sf::Vector2f(80.0f, 80.0f), temp);
		m_Indexes.back().SetPosition({ windSize.x / 4.0f + 40.0f + i * 80.0f, windSize.y / 15.0f + 8 * 80.0f + 40.0f });
		m_Indexes.back().SetBgColor({ 33, 33, 33 });
	}

	if(fileName.empty())
		BoardInit(windSize, "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1", cs);
	else {
		std::ifstream file(fileName);

		if(!file.good())
			BoardInit(windSize, "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1", cs);
		else {
			std::string line;

			std::getline(file, line);
			BoardInit(windSize, line, cs);
		}
	}
}

Board::~Board() {
	ResourceHolder::Get().EraseTexture("pawn_white");
	ResourceHolder::Get().EraseTexture("pawn_black");
	ResourceHolder::Get().EraseTexture("rook_white");
	ResourceHolder::Get().EraseTexture("rook_black");
	ResourceHolder::Get().EraseTexture("knight_white");
	ResourceHolder::Get().EraseTexture("knight_black");
	ResourceHolder::Get().EraseTexture("bishop_white");
	ResourceHolder::Get().EraseTexture("bishop_black");
	ResourceHolder::Get().EraseTexture("queen_white");
	ResourceHolder::Get().EraseTexture("queen_black");
	ResourceHolder::Get().EraseTexture("king_white");
	ResourceHolder::Get().EraseTexture("king_black");
	ResourceHolder::Get().EraseTexture("tile_light");
	ResourceHolder::Get().EraseTexture("tile_dark");
	ResourceHolder::Get().EraseTexture("tile_highlight");
}

void Board::Render(sf::RenderTarget& renderer) {
	for (auto& ind : m_Indexes)
		ind.Render(renderer);

	for (auto& row : m_Board) {
		for (auto& tile : row)
			tile.RenderTile(renderer);
	}

	for (auto& row : m_Board) {
		for (auto& tile : row)
			tile.RenderPiece(renderer);
	}
}

void Board::HighlightTiles(std::vector<std::pair<int, int>>& moves) {
	for (auto& [moveX, moveY] : moves) {
		for (auto& row : m_Board) {
			for (auto& tile : row) {
				if (tile.GetBoardPos() == Helper::Get().PosToString({ moveX, moveY })) {
					tile.SetTexture(&ResourceHolder::Get().GetTexture("tile_highlight"));
					tile.SetHighlighted(true);
				}
			}
		}
	}
}

void Board::UnhighlightTiles() {
	for (auto& row : m_Board) {
		for (auto& tile : row) {
			if (tile.IsHighlighted()) {
				auto [col, row] = Helper::Get().StringToPos(tile.GetBoardPos());

				if ((col + row) % 2)
					tile.SetTexture(&ResourceHolder::Get().GetTexture("tile_dark"));
				else
					tile.SetTexture(&ResourceHolder::Get().GetTexture("tile_light"));

				tile.SetHighlighted(false);
			}
		}
	}
}

std::unique_ptr<Piece>&& Board::GetCurrentlyHoveredPiece(const sf::RenderWindow& window) {
	sf::Vector2i mousePos = sf::Mouse::getPosition(window);

	for (auto& row : m_Board) {
		for (auto& tile : row) {
			if (tile.IsHoveredOver(mousePos))
				return std::move(tile.GetPiecePtr());
		}
	}

	return nullptr;
}

Tile* Board::GetCurrentlyHoveredTile(const sf::RenderWindow& window) {
	sf::Vector2i mousePos = sf::Mouse::getPosition(window);

	for (auto& row : m_Board) {
		for (auto& tile : row) {
			if (tile.IsHoveredOver(mousePos))
				return &tile;
		}
	}

	return nullptr;
}

std::vector<std::vector<Tile>>& Board::GetBoard() {
	return m_Board;
}
