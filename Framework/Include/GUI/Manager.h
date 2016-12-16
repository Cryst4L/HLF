#pragma once

#include "Widget.h"

class Manager
{
  private:
  
	sf::RenderWindow& m_window;  
  	std::vector <Widget*> widgets;
	CommandStatus status;
	bool m_close_event;

  public:
  
	Manager(sf::RenderWindow& window);

	void addWidget(Widget& widget);

	void update();	
	void render();

	bool closeEvent();
};