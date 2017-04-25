#include "GUI/Slider.h"

Slider::Slider(int width, int height)
	:	m_value(0.5), m_cursor_size(8 + SHAPE_OUTLINE, height)
{
	m_dimension = sf::Vector2f(width, height);
}

// Replace by a 'listen' function
void Slider::update(CommandStatus& status)
{
	bool bl = status.mouse.x > m_midpoint.x - .5f * m_cursor_size.x;
	bool br = status.mouse.x < m_midpoint.x + .5f * m_cursor_size.x;
	bool bu = status.mouse.y > m_midpoint.y - .5f * m_cursor_size.y;
	bool bd = status.mouse.y < m_midpoint.y + .5f * m_cursor_size.y;
	m_focused = bl && br && bu && bd;

	if (m_focused && status.mouse.pressed) m_grabbed = true;

	if (m_grabbed && !status.mouse.pressed) m_grabbed = false;

	m_midpoint.y = m_position.y + .5f * m_dimension.y;
	m_midpoint.x = m_position.x + m_value * m_dimension.x;

	if (m_grabbed) {
		bool bsl = status.mouse.x >= m_position.x;
		bool bsr = status.mouse.x <= (m_position.x + m_dimension.x);

		if (bsl && bsr) {
			m_value = (status.mouse.x - m_position.x) / m_dimension.x;
			m_midpoint.x = m_position.x + m_value * m_dimension.x;
		}
	}
}

void Slider::render(sf::RenderWindow& window)
{
/*
	// slide
	sf::RectangleShape slide(sf::Vector2f(m_dimension.x, SHAPE_OUTLINE));

	sf::Vector2f position;
	position.x = m_position.x - .5f * SHAPE_OUTLINE;
	position.y = m_position.y - .5f * SHAPE_OUTLINE + .5f * m_dimension.y;

	slide.setFillColor(COLOR_MAIN);
	slide.setPosition(position);
	window.draw(slide);

	// endpoints
	sf::CircleShape round(0.2 * m_dimension.y);
	round.setFillColor(COLOR_BASE);
	round.setOutlineColor(COLOR_MAIN);
	round.setOutlineThickness(SHAPE_OUTLINE);

	position.x = m_position.x - round.getRadius();
	position.y = m_position.y - round.getRadius() + .5f * m_dimension.y;

	round.setPosition(position);
	window.draw(round);
	position.x += m_dimension.x;
	round.setPosition(position);
	window.draw(round);
*/

	// Slider Background
	sf::RectangleShape slide(sf::Vector2f(m_dimension.x, m_dimension.y));
	slide.setPosition(m_position.x, m_position.y + .25f * m_dimension.y);
	slide.setSize(sf::Vector2f(m_dimension.x, 0.5f * m_dimension.y));
	slide.setFillColor(COLOR_BASE);
	window.draw(slide);

	// Slider Filled
	slide.setSize(sf::Vector2f(m_value * m_dimension.x, 0.5f * m_dimension.y));
	slide.setFillColor(COLOR_FILL);
	window.draw(slide);

	// Slider Container
	slide.setSize(sf::Vector2f(m_dimension.x, 0.5 * m_dimension.y));
	slide.setFillColor(sf::Color::Transparent);
	slide.setOutlineColor(COLOR_MAIN);
	slide.setOutlineThickness(SHAPE_OUTLINE);
	window.draw(slide);

	// cursor
	sf::RectangleShape cursor(m_cursor_size);
	cursor.setFillColor(COLOR_BASE);
	cursor.setOutlineColor(COLOR_MAIN);
	cursor.setOutlineThickness(SHAPE_OUTLINE);

	sf::Vector2f position = m_midpoint - .5f * m_cursor_size;
	cursor.setPosition(position);
	window.draw(cursor);
}

bool Slider::isGrabbed()
{
	return m_grabbed;
}

float Slider::getValue()
{
	return m_value;
}

void Slider::setValue(float value)
{
	m_value = value;
}
