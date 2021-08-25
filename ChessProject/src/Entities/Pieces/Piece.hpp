#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>

#include "../../ResManagers/ResourceHolder.hpp"

class Tile;

enum class Color {
	WHITE,
	BLACK
};

class Piece {
	protected:
		sf::RectangleShape m_Body;
		std::string		   m_BoardPos;

		Color m_Color;
		char  m_ID;
		bool  m_HasMoved;

	public:
		Piece(const std::string& boardPos, Color color, char id);
		virtual ~Piece() = default;

		virtual void Render(sf::RenderTarget& renderer);
		virtual void SetPosition(const sf::Vector2f& pos);
		virtual void SetBoardPosition(const std::string& boardPos);
		virtual void SetMoved();

		virtual Color		 GetColor()	   const;
		virtual std::string  GetBoardPos() const;
		virtual char		 GetID()	   const;
		virtual bool		 HasMoved()	   const;

		virtual bool operator ==(char id);
		virtual bool operator !=(char id);

		virtual std::vector<std::pair<int, int>> GetPossibleMoves(const std::vector<std::vector<Tile>>& board, Tile* pEnPassantTile = nullptr) const = 0;
		virtual std::vector<std::pair<int, int>> GetAttackedTiles(const std::vector<std::vector<Tile>>& board) const = 0;
};