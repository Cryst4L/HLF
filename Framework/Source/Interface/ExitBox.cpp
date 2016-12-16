#include "Interface/ExitBox.h"

namespace HLF
{
ExitBox::ExitBox()
	: 	m_cancel(false) {}

void ExitBox::run()
{
	sf::RenderWindow window;
	Manager manager(window);

	Button button_cancel(155, 50, "CANCEL");
	button_cancel.setPosition(15, 80);
	manager.addWidget(button_cancel);

	Button button_ok(155, 50, "OK");
	button_ok.setPosition(185, 80);
	manager.addWidget(button_ok);

	Label label_text("Are you sure you want to quit?");
	label_text.setPosition(25, 25);
	manager.addWidget(label_text);

	sf::RectangleShape canva_text(sf::Vector2f(325, 50));
	canva_text.setPosition(15, 15);
	canva_text.setOutlineColor(COLOR_BASE);
	canva_text.setOutlineThickness(SHAPE_OUTLINE);
	canva_text.setFillColor(sf::Color::Transparent);

	window.create(sf::VideoMode(355, 145), "Exit", sf::Style::Close);

	while (window.isOpen()) {
		manager.update();

		if (manager.closeEvent() || button_cancel.isClicked())
			window.close();

		if (button_ok.isClicked())
			exit(0);

		globalMutex.lock();

		window.clear();
		manager.render();
		window.draw(canva_text);
		window.display();

		globalMutex.unlock();
	}
}

ExitBox::~ExitBox() {};
}