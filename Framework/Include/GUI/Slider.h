#pragma once

#include "Widget.h"

class Slider : public Widget
{
  private:
  	float m_value;
  	
    sf::Vector2f m_midpoint;  
	sf::Vector2f m_cursor_size;
    
	bool m_focused;
	bool m_grabbed;
	bool m_new_value;
	
  public:
    Slider(int width, int height) ;

    void update(CommandStatus& status);
    void render(sf::RenderWindow& window);

    bool isGrabbed();

    float getValue();
	void setValue(float value);
};
