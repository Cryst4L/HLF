#pragma once

#include "Widget.h"

#include <sstream>
#include <iomanip>
#include <cmath>

class Entry : public Widget
{
  private:
	sf::Clock m_clock;
   
    sf::Text m_text;
	sf::Font m_font;
    FontSize m_font_size;
    sf::Vector2f m_offset;
    
	float m_value;
    std::string m_string;
    
	bool m_numeric;
	bool m_focused;
	bool m_locked;
	bool m_new_entry;
	
	bool numericFilter(int unicode);
	bool textFilter(int unicode);

  public:
    Entry(int w, int h, bool numeric=false);

    void update(CommandStatus& status);
    void render(sf::RenderWindow& window);

    bool newEntry();

    std::string getText();
    void setText(std::string string);

    float getValue();
    void setValue(float value);
};
