#include "Interface/InfoBox.h"

namespace HLF
{
InfoBox::InfoBox(std::string text) 
:	m_string(text)
{
	m_active_p = false;	

	sf::Font font = initFont(FONT_NAME);
	sf::Text preview(text, font, FONT_MEDIUM);
	
	int app_width = std::max(180.f, preview.getLocalBounds().width + 60);
	int app_height = preview.getLocalBounds().height + 130;
	
	m_window.create(sf::VideoMode(app_width, app_height), "[HLF] Hey Listen !", sf::Style::Close);

	m_window.setActive(false);
	
	m_thread_p = new sf::Thread(&InfoBox::run, this);

	m_thread_p->launch();
}

void InfoBox::run() {
	
//		globalMutex.lock();

	m_active_p = true;
	m_window.setActive(true);

	Manager manager(m_window);
			
	Label label_text(m_string);
	label_text.setPosition(25, 25);
	manager.addWidget(label_text);
	
	sf::Vector2f button_size(150, 50);
	
	sf::Vector2f canva_size;
	canva_size.x = m_window.getSize().x - 30;
	canva_size.y = m_window.getSize().y - (button_size.y + 45);
	
	sf::RectangleShape canva_text(canva_size);
	canva_text.setPosition(15, 15);
	canva_text.setOutlineColor(COLOR_BASE);
	canva_text.setOutlineThickness(SHAPE_OUTLINE);
	canva_text.setFillColor(sf::Color::Transparent);
	
	sf::Vector2f button_position;
	button_position.x = .5 * (m_window.getSize().x - button_size.x);
	button_position.y = m_window.getSize().y - (button_size.y + 15);
	
	Button button_ok(button_size.x, button_size.y, "OK");
	button_ok.setPosition(button_position.x, button_position.y);
	manager.addWidget(button_ok);
			
//		globalMutex.unlock();

	while (m_window.isOpen()) {

		manager.update();

		if (manager.closeEvent() || button_ok.isClicked())
			m_active_p = false;
		
		if (!m_active_p)
			m_window.close();

				
		globalMutex.lock();

		m_window.clear();
		manager.render();
		m_window.draw(canva_text);
		m_window.display();

		globalMutex.unlock();
	}

}

void InfoBox::close() {
	m_active_p = false;
}

InfoBox::~InfoBox() {
	m_thread_p->wait();
	delete m_thread_p;
}
}