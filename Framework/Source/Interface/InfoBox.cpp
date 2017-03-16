#include "Interface/InfoBox.h"

namespace HLF
{
InfoBox::InfoBox(std::string text)
	:	m_string(text)
{
	m_active_p = false;

	sf::Font font = initFont(FONT_NAME);
	sf::Text text_preview(text, font, FONT_MEDIUM);

	int app_width = std::max(180.f, text_preview.getLocalBounds().width + 60);
	int app_height = text_preview.getLocalBounds().height + 115;

	sf::VideoMode video_mode(app_width, app_height);
	sf::ContextSettings settings(0, 0, ANTI_ALIASING);

	m_window.create(video_mode, "", sf::Style::Close, settings);
	m_window.setTitle("[HLF] Hey Listen !");
	m_window.setActive(false);

	m_thread_p = new sf::Thread(&InfoBox::run, this);
	m_thread_p->launch();
}

void InfoBox::run()
{
//		globalMutex.lock();
	m_active_p = true;
	m_window.setActive(true);

	Manager manager(m_window);

	sf::Vector2f canva_size;
	canva_size.x = m_window.getSize().x - 30;
	canva_size.y = m_window.getSize().y - 90;

	Canva canva_text(canva_size.x, canva_size.y);
	canva_text.setPosition(15, 15);
	manager.addWidget(canva_text);

	Label label_text(m_string);
	label_text.setPosition(25, 20);
	manager.addWidget(label_text);

	sf::Vector2f button_position;
	button_position.x = .5 * (m_window.getSize().x - 150);
	button_position.y = m_window.getSize().y - 60;

	Button button_ok(150, 45, "OK");
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

		manager.render();
		m_window.display();

		globalMutex.unlock();
	}
}

void InfoBox::close()
{
	m_active_p = false;
}

InfoBox::~InfoBox()
{
	m_thread_p->wait();
	delete m_thread_p;
}
}
