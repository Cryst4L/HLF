#include "Interface/StatusBox.h"

namespace HLF
{
StatusBox::StatusBox(double& value)
	: 	m_value(value), m_string("Please wait ...")
{
	m_value = 0;

	sf::VideoMode video_mode(APP_WIDTH, APP_HEIGHT);
	sf::ContextSettings settings(0, 0, ANTI_ALIASING);
	
	m_window.create(video_mode, "[HLF] Status", sf::Style::Close, settings);
	m_window.setActive(false);

	m_thread_p = new sf::Thread(&StatusBox::run, this);
	m_thread_p->launch();
}

void StatusBox::setText(std::string string)
{
	m_string = string;
}

void StatusBox::setValue(double& value)
{
	m_value = value;
}

void StatusBox::run()
{
	m_window.setActive(true);

	Manager manager(m_window);

	Canva text_canva(APP_WIDTH - 30, APP_HEIGHT - 80);
	text_canva.setPosition(15, 15);
	manager.addWidget(text_canva);

	Label label_text(m_string);
	label_text.setPosition(25, 25);
	manager.addWidget(label_text);

	TrackBar status_bar(APP_WIDTH - 30, 35);
	status_bar.setPosition(15, APP_HEIGHT - 50);
	manager.addWidget(status_bar);

	ExitBox exit_box;

	while (m_window.isOpen()) {
		manager.update();

		if (manager.closeEvent())
			exit_box.run();

		status_bar.setValue(m_value);

		if (m_value >= (1 - EPSILON))
			m_window.close();

		globalMutex.lock();

		m_window.clear(COLOR_WALL);

		manager.render();

		m_window.display();

		globalMutex.unlock();
	}
}

StatusBox::~StatusBox()
{
	m_thread_p->wait();
	delete m_thread_p;
}

}
