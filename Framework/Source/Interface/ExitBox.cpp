#include "Interface/ExitBox.h"

namespace HLF
{
ExitBox::ExitBox()
	: 	m_cancel(false) {}

void ExitBox::run()
{
	sf::RenderWindow window;
	Manager manager(window);

	Canva canva_text(325, 45);
	canva_text.setPosition(15, 15);
	manager.addWidget(canva_text);

	Button button_cancel(155, 45, "CANCEL");
	button_cancel.setPosition(15, 75);
	manager.addWidget(button_cancel);

	Button button_ok(155, 45, "OK");
	button_ok.setPosition(185, 75);
	manager.addWidget(button_ok);

	Label label_text("Are you sure you want to quit?");
	label_text.setPosition(25, 25);
	manager.addWidget(label_text);

	window.create(sf::VideoMode(355, 135), "Exit", sf::Style::Close);

	while (window.isOpen()) {
		manager.update();

		if (manager.closeEvent() || button_cancel.isClicked())
			window.close();

		if (button_ok.isClicked())
			exit(0);

		globalMutex.lock();

		manager.render();
		window.display();

		globalMutex.unlock();
	}
}

ExitBox::~ExitBox() {};
}
