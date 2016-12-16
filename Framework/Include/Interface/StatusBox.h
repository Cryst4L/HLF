#pragma once

#include "GUI.h"
#include "ExitBox.h"

namespace HLF
{
class StatusBox
{
    private:

	static const int APP_WIDTH		= 480;
	static const int APP_HEIGHT		= 130;
	static const float EPSILON		= 1E-2;

	double& m_value;

	std::string m_string;

	sf::Thread* m_thread_p;

	sf::RenderWindow m_window;

    public:

	StatusBox(double& value);

	void setText(std::string string);

	void setValue(double& value);

	void run();

	~StatusBox();
};
}
