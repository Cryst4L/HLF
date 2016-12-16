#include "GUI/Label.h"

Label::Label(std::string string) 
  : m_font_size(FONT_MEDIUM) {	
	m_font = initFont(FONT_NAME);
    m_text.setFont(m_font);
    
	m_text.setColor(COLOR_BASE);
    m_text.setCharacterSize(m_font_size);
	m_text.setString(string);
}

void Label::update(CommandStatus& status) {}

void Label::render(sf::RenderWindow& window) {
	
	m_dimension.x = m_text.getLocalBounds().width  + 0.07 * m_font_size;
	m_dimension.y = m_text.getLocalBounds().height + 0.42 * m_font_size;
	
    m_offset.x = .05 * m_font_size;
    m_offset.y = .05 * m_font_size;
	
	sf::RectangleShape shape(m_dimension);
	shape.setPosition(m_position);

    shape.setFillColor(COLOR_FILL);
//  shape.setFillColor(sf::Color::Green);
    m_text.setPosition(shape.getPosition() + m_offset);

	window.draw(shape);
	window.draw(m_text);
}

void Label::setText(std::string words) {
    m_text.setString(words);
//		glFlush();
}


