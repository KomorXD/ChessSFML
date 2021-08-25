#include "App.hpp"
#include "ResManagers/ResourceHolder.hpp"

App::App(const std::string& fileName)
	: m_Window({ 1280, 800 }, "Chess by Pawel Kupczak", sf::Style::Titlebar | sf::Style::Close) {

	ResourceHolder::Get().AddFont("OpenSans");
	m_pState = std::make_unique<ChessState>(this, fileName);
}

void App::Run() {
	sf::Event ev;
	sf::Clock dTimer;
	float dt;

	while (m_Window.isOpen()) {
		dt = dTimer.restart().asSeconds();

		while (m_Window.pollEvent(ev)) {
			m_pState->HandleEvents(ev);

			switch (ev.type) {
				case sf::Event::Closed:
					m_Window.close();
					break;
				default:
					break;
			}
		}

		m_Window.clear({ 33, 33, 33 });
		
		m_pState->Update(dt);
		m_pState->Render(m_Window);

		m_Window.display();
	}
}

const sf::RenderWindow& App::GetWindow() const {
	return m_Window;
}

void App::CloseWindow() {
	m_Window.close();
}
