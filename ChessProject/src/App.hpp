#pragma once

#include "ChessState.hpp"

#include <SFML/Graphics.hpp>
#include <memory>

class App {
	private:
		sf::RenderWindow m_Window;

		std::unique_ptr<ChessState> m_pState;

	public:
		App(const std::string& fileName = "");

		void Run();

		const sf::RenderWindow& GetWindow() const;
		void CloseWindow();
};