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
	// slide
	sf::RectangleShape slide(sf::Vector2f(m_dimension.x, SHAPE_OUTLINE));
	sf::Vector2f position;
	position.x = m_position.x - .5f * SHAPE_OUTLINE;
	position.y = m_position.y - .5f * SHAPE_OUTLINE + .5f * m_dimension.y;
	slide.setPosition(position);
	window.draw(slide);
	// endpoints
	sf::CircleShape round(4 + SHAPE_OUTLINE);
	round.setFillColor(COLOR_FILL);
	round.setOutlineColor(COLOR_BASE);
	round.setOutlineThickness(SHAPE_OUTLINE);
	position.x = m_position.x - round.getRadius();
	position.y = m_position.y - round.getRadius() + .5f * m_dimension.y;
	round.setPosition(position);
	window.draw(round);
	position.x += m_dimension.x;
	round.setPosition(position);
	window.draw(round);
	// cursor
	sf::RectangleShape cursor(m_cursor_size);
	cursor.setFillColor(COLOR_FILL);
	cursor.setOutlineColor(COLOR_BASE);
	cursor.setOutlineThickness(SHAPE_OUTLINE);
	position = m_midpoint - .5f * m_cursor_size;
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
