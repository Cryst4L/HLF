#include "GUI/Button.h"

Button::Button(int w, int h, std::string string)
	:	m_focused(false), m_pressed(false), m_clicked(false)
{
	m_dimension = sf::Vector2f(w, h);
	m_font = initFont(FONT_NAME);
	m_text.setFont(m_font);
	m_text.setColor(COLOR_BASE);
	m_text.setCharacterSize(FONT_MEDIUM);
	m_text.setString(string);
}

void Button::update(CommandStatus& status)
{
	bool bl = status.mouse.x > m_position.x;
	bool br = status.mouse.x < m_position.x + m_dimension.x;
	bool bu = status.mouse.y > m_position.y;
	bool bd = status.mouse.y < m_position.y + m_dimension.y;
	m_focused = bl && br && bu && bd;

	m_pressed = m_focused && status.mouse.pressed;
	m_clicked = m_focused && status.mouse.clicked;
}

void Button::render(sf::RenderWindow& window)
{
	sf::RectangleShape shape(m_dimension);
	shape.setFillColor(COLOR_FILL);
	shape.setOutlineColor(COLOR_BASE);
	shape.setOutlineThickness(SHAPE_OUTLINE);

	if (!m_focused) {
		shape.setFillColor(COLOR_BUT0);
	} else {
		shape.setFillColor(COLOR_BUT1);
	}

	sf::Vector2f dash(-3.f, 3.f);

	if (!m_pressed) {
		shape.setPosition(m_position);
	} else {
		shape.setPosition(m_position + dash);
	}

	window.draw(shape);
	// this cannot be called inside the constructor since the window must
	// be created and a draw must be called before "getLocalBounds()" is used
	m_offset.x = std::floor(0.5 * (m_dimension.x - m_text.getLocalBounds().width)  - 2.f);
	m_offset.y = std::floor(0.5 * (m_dimension.y - m_text.getLocalBounds().height) - 5.f);

	m_text.setPosition(shape.getPosition() + m_offset);
	window.draw(m_text);
}

void Button::setText(std::string string)
{
	m_text.setString(string);
}

bool Button::isClicked()
{
	return m_clicked;
}

