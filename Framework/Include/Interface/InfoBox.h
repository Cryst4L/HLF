#pragma once

#include "GUI.h"

namespace HLF
{
class InfoBox
{
  private:  	
  
    bool m_active_p;
    
	std::string m_string;
	
	sf::Thread* m_thread_p;
	
	sf::RenderWindow m_window;

  public:
	InfoBox(std::string text);
	
	void run();	
	void close();

	~InfoBox();
};
}