#include "GUI/Entry.h"

bool Entry::numericFilter(int unicode)
{
	bool valid = (unicode > 47 && unicode < 58);

	if (unicode == 45) {
		valid |= m_string.empty();
	} else if (unicode == 46) {
		valid |= (m_string.find('.') == std::string::npos);
	}

	return valid;
}

bool Entry::textFilter(int unicode)
{
	bool valid = (unicode == 95);
	valid |= (unicode > 44 && unicode < 47);
	valid |= (unicode > 47 && unicode < 58);
	valid |= (unicode > 64 && unicode < 91);
	valid |= (unicode > 96 && unicode < 123);
	return valid;
}


Entry::Entry(int w, int h, bool numeric)
	:	m_font_size(FONT_MEDIUM), m_locked(false),
	  m_value(0), m_numeric(numeric)
{
	m_font = initFont(FONT_NAME);
	m_text.setFont(m_font);
	m_dimension = sf::Vector2f(w, h);
	m_text.setColor(COLOR_BASE);
	m_text.setCharacterSize(m_font_size);
}

void Entry::update(CommandStatus& status)
{
	bool bl = status.mouse.x > m_position.x;
	bool br = status.mouse.x < m_position.x + m_dimension.x;
	bool bu = status.mouse.y > m_position.y;
	bool bd = status.mouse.y < m_position.y + m_dimension.y;
	m_focused = bl && br && bu && bd;
	bool pre_locked = m_locked;

	if (m_locked && status.mouse.clicked && !m_focused)
		m_locked = false;

	if (m_focused && status.mouse.clicked)
		m_locked = true;

	if (m_locked && status.keyboard.stroked) {
		unsigned int code = status.keyboard.code;
		unsigned int unicode = status.keyboard.unicode;

		if(code == sf::Keyboard::BackSpace && m_string.size() != 0) {
			m_string.erase(m_string.end() - 1);
		} else if (code == sf::Keyboard::Return) {
			m_locked = false;
		} else if (m_text.getLocalBounds().width < m_dimension.x - m_font_size) {
			bool valid = m_numeric ? numericFilter(unicode) : textFilter(unicode);

			if (valid) m_string += static_cast <char> (unicode);
		}
	}

	m_new_entry = pre_locked && !m_locked;
//		std::cout << m_locked << std::endl;

	if (m_new_entry) {
		m_value = m_numeric ? std::atof(m_string.c_str()) : 0.f;
//			std::cout << m_value << std::endl;
	}
}

void Entry::render(sf::RenderWindow& window)
{
//		globalMutex.lock();
	sf::RectangleShape shape;
	shape.setSize(m_dimension);
	shape.setOutlineColor(COLOR_BASE);
	shape.setFillColor(COLOR_FILL);
	shape.setOutlineThickness(SHAPE_OUTLINE);

	if (!m_locked) {
		shape.setOutlineColor(COLOR_BASE);
	} else {
		shape.setOutlineColor(COLOR_LOCK);
	}

	m_offset.x = std::floor(.14 * m_font_size);
	m_offset.y = std::floor(.5 * (m_dimension.y - .72 * m_font_size) - .28 * m_font_size);
	shape.setPosition(m_position);
	m_text.setPosition(m_position + m_offset);
	m_text.setString(m_string);
	window.draw(shape);
	window.draw(m_text);
	// Display caret
	float time = m_clock.getElapsedTime().asSeconds();
	bool tic = (static_cast <int> (2 * time)) % 2;

	if (tic && m_locked) {
		sf::RectangleShape caret;
		sf::Vector2f caret_offset;
		caret_offset.x = .28 * m_font_size + m_text.getLocalBounds().width;
		caret_offset.y = m_offset.y + .14 * m_font_size;
		caret.setSize(sf::Vector2f(0.10 * m_font_size, m_font_size));
		caret.setPosition(m_position + caret_offset);
		window.draw(caret);
	}

//		globalMutex.unlock();
}

bool Entry::newEntry()
{
	return m_new_entry;
}

std::string Entry::getText()
{
	return m_string;
}

void Entry::setText(std::string string)
{
	m_string = string;
	m_value = 0.;
}

float Entry::getValue()
{
	return m_value;
}

void Entry::setValue(float value)
{
	m_value = value;
	std::ostringstream ostr;
	ostr << std::setprecision(4) << m_value;
	m_string = ostr.str();
}

