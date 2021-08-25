#include "ChessState.hpp"
#include "Entities/Pieces/Pawn.hpp"
#include "App.hpp"
#include "Helper.hpp"

Tile* ChessState::GetEnPassantTile() {
	return m_EnPassantTile;
}

void ChessState::PickUpPiece(Tile* pTileHovered, Piece* pPieceHovered) {
	if (!pTileHovered || !pPieceHovered)
		return;

	if (pPieceHovered->GetColor() == m_CurrentPlayerColor) {
		m_Moves.clear();
		m_pHeldFrom = pTileHovered;
		m_pCurrentlyHeld = std::move(pTileHovered->GetPiecePtr());
		m_Moves = m_pCurrentlyHeld->GetPossibleMoves(m_Board.GetBoard(), m_EnPassantTile);
		m_Moves = m_GameLogic.ValidMoves(m_Board.GetBoard(), m_Moves, m_pCurrentlyHeld, m_CurrentPlayerColor);
		m_Board.HighlightTiles(m_Moves);
	}
}

void ChessState::DropPiece(Tile* pTileHovered, Piece* pPieceHovered) {
	if (!pTileHovered) {
		m_pHeldFrom->SetPiece(std::move(m_pCurrentlyHeld));

		return;
	}

	if (std::find(m_Moves.begin(), m_Moves.end(), Helper::Get().StringToPos(pTileHovered->GetBoardPos())) != m_Moves.end()) {
		char kingID = m_CurrentPlayerColor == Color::WHITE ? 'K' : 'k';
		int xDiff	= std::abs((int)(pTileHovered->GetBoardPos()[0] - 96) - (int)(m_pHeldFrom->GetBoardPos()[0] - 96));

		if (*m_pCurrentlyHeld == kingID && xDiff > 1) {
			Castle(pTileHovered);

			m_EnPassantTile = nullptr;
			++m_HalfMoves;
			m_MoveSound.play();

			UpdateAfterMove();

			return;
		}

		char pawnID = m_CurrentPlayerColor == Color::WHITE ? 'P' : 'p';

		if (*m_pCurrentlyHeld == pawnID && xDiff != 0 && !pTileHovered->GetPiecePtr() && m_EnPassantTile) {
			EnPassantTake(pTileHovered);
			
			m_EnPassantTile = nullptr;
			m_HalfMoves = 1;
			m_CaptureSound.play();

			UpdateAfterMove();

			return;
		}

		if (*m_pCurrentlyHeld == pawnID && !m_pCurrentlyHeld->HasMoved()) {
			int dir = m_CurrentPlayerColor == Color::WHITE ? 1 : -1;
			auto [posX, posY] = Helper::Get().StringToPos(m_pHeldFrom->GetBoardPos());

			if (std::abs(posY - (int)(pTileHovered->GetBoardPos()[1] - 48)) > 1) {
				m_EnPassantTile = &m_Board.GetBoard()[posY - 1 + dir][posX - 1];
				m_HalfMoves = 1;

				m_pCurrentlyHeld->SetMoved();
				pTileHovered->SetPiece(std::move(m_pCurrentlyHeld));
				m_MoveSound.play();

				UpdateAfterMove();

				return;
			}
		}

		if (pTileHovered->GetPiecePtr()) {
			if (pTileHovered->GetPiecePtr()->GetColor() == Color::WHITE)
				--m_WhitePieces;
			else
				--m_BlackPieces;

			m_HalfMoves = 1;
			m_CaptureSound.play();
		}
		else
			m_MoveSound.play();

		if (*m_pCurrentlyHeld == pawnID)
			m_HalfMoves = 1;
		else
			++m_HalfMoves;

		m_pCurrentlyHeld->SetMoved();
		pTileHovered->SetPiece(std::move(m_pCurrentlyHeld));

		UpdateAfterMove();

		m_EnPassantTile = nullptr;

		return;
	}

	m_pHeldFrom->SetPiece(std::move(m_pCurrentlyHeld));
	m_pHeldFrom = nullptr;
}

void ChessState::UpdateAfterMove() {
	if (m_CurrentPlayerColor == Color::BLACK)
		++m_FullMoves;
	
	m_CurrentPlayerColor = m_CurrentPlayerColor == Color::WHITE ? Color::BLACK : Color::WHITE;
	m_State = m_GameLogic.AnalyzeBoard(m_Board.GetBoard(), m_CurrentPlayerColor);
}

void ChessState::Castle(Tile* pTileHovered) {
	int side = (int)(pTileHovered->GetBoardPos()[0] - 96) - (int)(m_pCurrentlyHeld->GetBoardPos()[0] - 96);
	int y = m_CurrentPlayerColor == Color::WHITE ? 0 : 7;

	m_pCurrentlyHeld->SetMoved();

	if (side > 0) {
		m_Board.GetBoard()[y][6].SetPiece(std::move(m_pCurrentlyHeld));
		m_Board.GetBoard()[y][5].SetPiece(std::move(m_Board.GetBoard()[y][7].GetPiecePtr()));
		m_Board.GetBoard()[y][5].GetPiecePtr()->SetMoved();

		return;
	}

	m_Board.GetBoard()[y][2].SetPiece(std::move(m_pCurrentlyHeld));
	m_Board.GetBoard()[y][3].SetPiece(std::move(m_Board.GetBoard()[y][0].GetPiecePtr()));
	m_Board.GetBoard()[y][3].GetPiecePtr()->SetMoved();
}

void ChessState::EnPassantTake(Tile* pTileHovered) {
	if (m_CurrentPlayerColor == Color::WHITE)
		--m_BlackPieces;
	else
		--m_WhitePieces;

	pTileHovered->SetPiece(std::move(m_pCurrentlyHeld));
	
	auto [posX, posY] = Helper::Get().StringToPos(pTileHovered->GetBoardPos());

	if (posY == 6)
		m_Board.GetBoard()[posY - 2][posX - 1].EraseTile();
	else
		m_Board.GetBoard()[posY][posX - 1].EraseTile();
	
	m_EnPassantTile = nullptr;
}

ChessState::ChessState(App* pApp, const std::string& fileName)
	: m_pApp(pApp), m_CurrentPlayerColor(Color::WHITE), m_EnPassantTile(nullptr), m_pHeldFrom(nullptr), m_HalfMoves(0), m_FullMoves(1), m_Board(sf::Vector2f(m_pApp->GetWindow().getSize()), fileName, this)
	, m_GameLogic(this), m_pCurrentlyHeld(nullptr), m_State(MatchState::NORMAL), m_EndTextbox({ 300.0f, 100.0f }), m_WhiteTimerBox({200.0f, 100.0f}, "15:00")
	, m_BlackTimerBox({ 200.0f, 100.0f }, "15:00"), m_WhiteSecondsLeft(60 * 15.0f), m_BlackSecondsLeft(60 * 15.0f), m_WhitePieces(0), m_BlackPieces(0) {

	m_EndTextbox.SetBgColor({ 0, 0, 0, 0 });
	m_EndTextbox.SetPosition({ 1125.0f, 300.0f });

	m_WhiteTimerBox.SetBgColor({ 0, 0, 0, 0 });
	m_WhiteTimerBox.SetPosition({ 175.0f, 700.0f });

	m_BlackTimerBox.SetBgColor({ 0, 0, 0, 0 });
	m_BlackTimerBox.SetPosition({ 175.0f, 100.0f });

	for (auto& row : m_Board.GetBoard()) {
		for (auto& tile : row) {
			Piece* pPiece = tile.GetPiecePtr().get();

			if (pPiece) {
				if (pPiece->GetID() >= 65 && pPiece->GetID() <= 90)
					++m_WhitePieces;
				else
					++m_BlackPieces;
			}
		}
	}

	ResourceHolder::Get().AddSound("sound_move");
	ResourceHolder::Get().AddSound("sound_capture");

	m_MoveSound.setBuffer(ResourceHolder::Get().GetSound("sound_move"));
	m_CaptureSound.setBuffer(ResourceHolder::Get().GetSound("sound_capture"));
}

ChessState::~ChessState() {
	ResourceHolder::Get().EraseSound("sound_move");
	ResourceHolder::Get().EraseSound("sound_capture");
}

void ChessState::HandleEvents(sf::Event& ev) {
	if (ev.type == sf::Event::KeyPressed && ev.key.code == sf::Keyboard::Escape) {
		m_pApp->CloseWindow();

		return;
	}

	if ((m_State == MatchState::NORMAL || m_State == MatchState::CHECKED) && ev.type == sf::Event::MouseButtonPressed && ev.mouseButton.button == sf::Mouse::Left) {
		Tile*  pTileHovered  = m_Board.GetCurrentlyHoveredTile(m_pApp->GetWindow());
		Piece* pPieceHovered = m_Board.GetCurrentlyHoveredPiece(m_pApp->GetWindow()).get();

		if (!m_pCurrentlyHeld) {
			PickUpPiece(pTileHovered, pPieceHovered);

			return;
		}

		DropPiece(pTileHovered, pPieceHovered);
		m_Board.UnhighlightTiles();
	}
}

void ChessState::Update(float dt) {
	if (m_pCurrentlyHeld)
		m_pCurrentlyHeld->SetPosition(sf::Vector2f(sf::Mouse::getPosition(m_pApp->GetWindow())));

	if (m_HalfMoves == 50) {
		m_State = MatchState::STALEMATED;
		m_EndTextbox.SetText("Tied by\nfifty-move rule!");

		return;
	}

	if (m_WhitePieces == 1 && m_BlackPieces == 1) {
		m_State = MatchState::STALEMATED;
		m_EndTextbox.SetText("Tied!");

		return;
	}

	if (m_WhiteSecondsLeft <= 0.0f) {
		m_WhiteSecondsLeft = 0.0f;
		m_State = MatchState::TIMED_OUT;
		m_EndTextbox.SetText("Black won by\ntimeout!");

		return;
	}

	if (m_BlackSecondsLeft <= 0.0f) {
		m_BlackSecondsLeft = 0.0f;
		m_State = MatchState::TIMED_OUT;
		m_EndTextbox.SetText("White won by\ntimeout!");

		return;
	}

	if (m_State == MatchState::CHECKMATED) {
		if (m_CurrentPlayerColor == Color::WHITE)
			m_EndTextbox.SetText("Black won by\ncheckmate!");
		else
			m_EndTextbox.SetText("White won by\ncheckmate!");

		return;
	}

	if (m_State == MatchState::STALEMATED) {
		m_EndTextbox.SetText("Tied by stalemate!");

		return;
	}

	if (m_CurrentPlayerColor == Color::WHITE) {
		m_WhiteSecondsLeft -= dt;
		m_WhiteTimerBox.SetText(Helper::Get().SecondsToStringTime(m_WhiteSecondsLeft));

		return;
	}
	
	m_BlackSecondsLeft -= dt;
	m_BlackTimerBox.SetText(Helper::Get().SecondsToStringTime(m_BlackSecondsLeft));
}

void ChessState::Render(sf::RenderTarget& renderer) {
	m_Board.Render(renderer);
	
	if (m_pCurrentlyHeld)
		m_pCurrentlyHeld->Render(renderer);

	if (m_State == MatchState::CHECKMATED || m_State == MatchState::STALEMATED || m_State == MatchState::TIMED_OUT)
		m_EndTextbox.Render(renderer);

	m_WhiteTimerBox.Render(renderer);
	m_BlackTimerBox.Render(renderer);
}

void ChessState::SetActiveColor(Color color) {
	m_CurrentPlayerColor = color;
}

void ChessState::UnsetCastleKingSide(Color color) {
	int y = color == Color::WHITE ? 1 : 8;
	char rookID = color == Color::WHITE ? 'R' : 'r';
	Piece* pPiece = m_Board.GetBoard()[y - 1][7].GetPiecePtr().get();
	
	if (pPiece && *pPiece == rookID)
		pPiece->SetMoved();
}

void ChessState::UnsetCastleQueenSide(Color color) {
	int y = color == Color::WHITE ? 1 : 8;
	char rookID = color == Color::WHITE ? 'R' : 'r';
	Piece* pPiece = m_Board.GetBoard()[y - 1][0].GetPiecePtr().get();

	if (pPiece && *pPiece == rookID)
		pPiece->SetMoved();
}

void ChessState::SetEnPassant(const std::string& pos) {
	auto [posX, posY] = Helper::Get().StringToPos(pos);
	
	m_EnPassantTile = &m_Board.GetBoard()[posY - 1][posX - 1];
}

void ChessState::SetHalfMoves(int hm) {
	m_HalfMoves = hm;
}

void ChessState::SetFullMoves(int fm) {
	m_FullMoves = fm;
}
