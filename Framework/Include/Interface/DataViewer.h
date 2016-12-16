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
  
    static const int MIN_WIDTH		= 620;
    static const int HEAD_HEIGHT	= 50;
      	
	int m_zoom;
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
  
	DataViewer(std::vector <MatrixXd> data, int zoom=3, int padding=1, bool normalize=true, int nrows=0);
       
	void run();
	void wait();

	~DataViewer();
};
}
