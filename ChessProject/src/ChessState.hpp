#pragma once

#include "Entities/Board.hpp"
#include "GameLogic.hpp"

class App;

class ChessState {
	private:
		App* m_pApp;

		Color m_CurrentPlayerColor;
		Tile* m_EnPassantTile;
		Tile* m_pHeldFrom;
		int   m_HalfMoves;
		int	  m_FullMoves;

		Board   m_Board;
		Textbox m_EndTextbox;
		Textbox m_WhiteTimerBox;
		Textbox m_BlackTimerBox;

		float m_WhiteSecondsLeft;
		float m_BlackSecondsLeft;
		int	  m_WhitePieces;
		int   m_BlackPieces;
		
		GameLogic  m_GameLogic;
		MatchState m_State;

		std::vector<std::pair<int, int>> m_Moves;

		std::unique_ptr<Piece> m_pCurrentlyHeld;

		sf::Sound m_MoveSound;
		sf::Sound m_CaptureSound;
		
		void PickUpPiece(Tile* pTileHovered, Piece* pPieceHovered);
		void DropPiece(Tile* pTileHovered, Piece* pPieceHovered);
		void UpdateAfterMove();
		void Castle(Tile* pTileHovered);
		void EnPassantTake(Tile* pTileHovered);

	public:
		ChessState(App* pApp, const std::string& fileName);
		~ChessState();

		void HandleEvents(sf::Event& ev);
		void Update(float dt);
		void Render(sf::RenderTarget& renderer);
		
		void SetActiveColor(Color color);
		void UnsetCastleKingSide(Color color);
		void UnsetCastleQueenSide(Color color);
		void SetEnPassant(const std::string& pos);
		void SetHalfMoves(int hm);
		void SetFullMoves(int fm);
		
		Tile* GetEnPassantTile();
};