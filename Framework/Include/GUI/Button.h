#pragma once

#include "Widget.h"
#include <cmath>

class Button : public Widget
{
    private:
	sf::Text m_text;
	sf::Font m_font;
	sf::Vector2f m_offset;

	bool m_focused;
	bool m_pressed;
	bool m_clicked;

    public:
	Button(int w, int h, std::string string);

	void update(CommandStatus& status);
	void render(sf::RenderWindow& window);

	void setText(std::string string);
	bool isClicked();
};
