#pragma once

#include "GUI.h"

#include "Core.h"
#include "../Model.h"
#include <sys/time.h>

namespace HLF
{
class BenchPanel
{
    private :

	static const int PREVIEW_SIZE   = 325;
	static const int HEADER_SIZE	= 125;
//    const int HORIZONTAL_LEFTOVER = 20;

	static const int MAX_INERTIA	= 25;
	static const double MAX_SPEED	= 1.5;

	static const int PREVIEW_ZOOM	= 8;
	static const double SCALE_RATIO	= 0.65;

	static const double ENTROPY		= 1E-2;
	static const double DAMPENING	= 5E-3;

	Model* m_model_p;

	sf::RenderWindow m_window;

	Manager m_manager;

	int m_sample_size;
	int m_scale_inertia;
	double m_scale_speed;

	Puppet m_puppet;
	Sampler m_sampler;

	MatrixXi m_screen;
	MatrixXi m_input;

	sf::Image m_image_screen;
	sf::Image m_image_input;

	VectorXd m_walk;
	VectorXd m_state;
	VectorXd m_speed;
	VectorXd m_predicted;
	VectorXd m_averaged;

	MatrixXd m_motion_record;

	double wave(double x);

    public:

	BenchPanel(Model *model_p, int sample_size);

	void run();

	~BenchPanel();
};
}
