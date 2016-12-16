#pragma once

#include "Widget.h"
#include <cmath>
#include <sstream>

class TrackBar : public Widget
{
    private:
	float m_value;

	sf::Text m_text;
	sf::Font m_font;
	sf::Vector2f m_offset;
	FontSize m_font_size;

    public:
	TrackBar(int width, int height);

	void update(CommandStatus& status);
	void render(sf::RenderWindow& window);

	void setValue(float value);
};
