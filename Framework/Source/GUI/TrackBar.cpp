#include "GUI/TrackBar.h"

TrackBar::TrackBar(int width, int height)
	:	m_font_size(FONT_MEDIUM), m_value(0.f)
{
	m_dimension = sf::Vector2f(width, height);
	m_font = initFont(FONT_NAME);
	m_text.setFont(m_font);
	m_text.setColor(COLOR_BASE);
	m_text.setCharacterSize(FONT_MEDIUM);
}

// Replace by a 'listen' function
void TrackBar::update(CommandStatus& status) {}

void TrackBar::render(sf::RenderWindow& window)
{
	// Progress Bar
	sf::RectangleShape bar;
	bar.setPosition(m_position);
	bar.setSize(sf::Vector2f(m_value * m_dimension.x, m_dimension.y));
	bar.setFillColor(sf::Color::Blue);
	window.draw(bar);

	// Container
	sf::RectangleShape container(m_dimension);
	container.setPosition(m_position);
	container.setFillColor(sf::Color::Transparent);
	container.setOutlineColor(COLOR_BASE);
	container.setOutlineThickness(SHAPE_OUTLINE);
	window.draw(container);

	// Legend
	std::ostringstream strstr;
	strstr << static_cast <int> (100 * m_value);
	m_text.setString(strstr.str() + "%");
//		glFlush();
	m_offset.x = std::floor(.14 * m_font_size);
	m_offset.y = std::floor(.5 * (m_dimension.y - .72 * m_font_size) - .28 * m_font_size);
	m_text.setPosition(m_position + m_offset);
	window.draw(m_text);
}

void TrackBar::setValue(float value)
{
	m_value = value;
}

