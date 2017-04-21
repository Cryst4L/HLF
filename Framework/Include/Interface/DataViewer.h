#pragma once

#include "GUI.h"

#include <iomanip>
#include <iostream>

#include <Eigen/Core>
using namespace Eigen;


namespace HLF
{
class DataViewer
{
    private:

    static const int BORDER     = 10;
	static const int MIN_WIDTH  = 600;

	double m_zoom;
	int m_padding;
	bool m_normalized;

	int m_nrows;
	int m_ncols;

	int m_sample_width;
	int m_sample_height;

	std::vector <MatrixXd> m_data;

	sf::Image m_image;

	sf::Thread* m_thread_p;

	sf::RenderWindow m_window;

    public:

	DataViewer(std::vector <MatrixXd> data, double zoom = 3., 
        int nrows = 0, int padding = 1, bool normalize = true);

	void run();
	void wait();

	~DataViewer();
};
}
