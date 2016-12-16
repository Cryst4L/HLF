#pragma once

#include "GUI.h"
#include "Core/Sampler.h"
#include "Core/Puppet.h"

#include "ExitBox.h"

namespace HLF
{

static const char* LOGO_PATH = "../Framework/Assets/Banner.png";

class ConfigPanel
{
  private:
    static const int PREVIEW_SIZE	= 300;
        
    static const double ENTROPY		= 1E-2;
    static const double DAMPENING	= 5E-3;
    
    static const int MIN_SIZE		= 12;
	static const int DEF_SIZE		= 32;
	static const int MAX_SIZE		= 64;
	
	static const int MIN_NMBR		= 1E2; //1E0
	static const int DEF_NMBR		= 2.5E4; //1E2
	static const int MAX_NMBR		= 1E6;
	
	static const int DEF_SEED		= 1234;
  
	Manager m_manager;

	sf::RenderWindow m_window;
	
    Puppet m_puppet;
    Sampler m_sampler;
   
    sf::Image m_preview;
	
    double m_theta;
    VectorXd m_state;
    VectorXd m_speed;
    
    int m_sample_size;
    int m_sample_number;
    int m_sample_seed;
    
    Vector3d m_neutral;
       
	double wave(double x);
	double clamp(double x, double lower, double upper);

  public:
  
  	ConfigPanel();

	void run();

	int getSize();
	int getNumber();
	int getSeed();

	Vector3d getNeutral();

	~ConfigPanel();
};
}