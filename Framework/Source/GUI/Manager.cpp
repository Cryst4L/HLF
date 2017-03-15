#include "GUI/Manager.h"

Manager::Manager(sf::RenderWindow& window)
	: m_window(window), m_close_event(false)
{
//	m_window.setVerticalSyncEnabled(true);
	m_window.setFramerateLimit(30);
}

void Manager::addWidget(Widget& widget)
{
	widgets.push_back(&widget);
}

void Manager::update()
{
	status.mouse.x = sf::Mouse::getPosition(m_window).x;
	status.mouse.y = sf::Mouse::getPosition(m_window).y;
	status.mouse.pressed = sf::Mouse::isButtonPressed(sf::Mouse::Left);

	sf::Event event;

	status.mouse.clicked = false;
	status.keyboard.stroked = false;
	m_close_event = false;

//		globalMutex.lock();

	while (m_window.pollEvent(event)) {
		if(event.type == sf::Event::MouseMoved) {
			status.mouse.x = sf::Mouse::getPosition(m_window).x;
			status.mouse.y = sf::Mouse::getPosition(m_window).y;
		}

		if (event.type == sf::Event::Closed)
			m_close_event = true;

		if (event.mouseButton.button == sf::Mouse::Left) {
			if (event.type == sf::Event::MouseButtonReleased)
				status.mouse.clicked = true;
		}

		if (event.type == sf::Event::KeyPressed) {
			status.keyboard.code = event.key.code;
			status.keyboard.stroked = true;
		}

		if (event.type == sf::Event::TextEntered) {
			status.keyboard.unicode = event.text.unicode;
		}
	}

//		globalMutex.unlock();

	for (int i; i < widgets.size(); i++)
		widgets[i]->update(status);
}

void Manager::render()
{
//		globalWMutex.lock();
	m_window.clear(COLOR_WALL);
	for (int i; i < widgets.size(); i++) {
		widgets[i]->render(m_window);
	}
//		globalWMutex.unlock();
}

bool Manager::closeEvent()
{
	return m_close_event;
}

// Here: overload destructor, might cause some issues
// Manager::~Manager() {};
