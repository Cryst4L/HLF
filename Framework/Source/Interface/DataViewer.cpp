#include "Interface/DataViewer.h"

namespace HLF
{
DataViewer::DataViewer(std::vector <MatrixXd> data, int zoom, int padding, bool normalize, int nrows) 
:	m_data(data), m_zoom(zoom), m_normalized(normalize), m_padding(padding), 
	m_nrows(nrows), m_ncols(0)
{ 
    m_nrows = (nrows != 0) ? nrows : std::sqrt(data.size());
    m_ncols = (data.size() + m_nrows - 1) / m_nrows;
    
    m_sample_width  = data[0].rows();
    m_sample_height = data[0].cols();
    
    int image_width  = (m_sample_width  + 1) * m_ncols + 1;
    int image_height = (m_sample_height + 1) * m_nrows + 1;

    int app_width  = std::max(m_zoom * image_width, MIN_WIDTH);
    int app_height = m_zoom * image_height + HEAD_HEIGHT;
       
    m_image.create(image_width, image_height);
    	
    m_window.create(sf::VideoMode(app_width, app_height), "[HLF] Data Viewer", sf::Style::Close);
	m_window.setActive(false);
    
	m_thread_p = new sf::Thread(&DataViewer::run, this);
	m_thread_p->launch();
}   
   
void DataViewer::run() {

	m_window.setActive(true);

    double max_value = -INFINITY;
    double min_value =  INFINITY; 

    for (int n = 0; n < m_data.size(); n++) {
		max_value = std::max(m_data[n].maxCoeff(), max_value);
		min_value = std::min(m_data[n].minCoeff(), min_value);
	}
    
    for (int n = 0; n < m_data.size(); n++) {
    
        int i = (n % m_ncols);
        int j = (n / m_ncols);
        
        MatrixXd sample = m_data[n];

        for (int di = 0; di < m_sample_width; di++) {
            for (int dj = 0; dj < m_sample_height; dj++) {
            
                double value = sample(di + m_sample_width * dj);

                if (m_normalized) 
                	value =  2 * (value - min_value) / (max_value - min_value) - 1;

                value = 127 * (value + 1);
                
                m_image.setPixel(
                    i * (m_sample_width  + 1) + di + 1,
                    j * (m_sample_height + 1) + dj + 1,
                    sf::Color(value, value, value)
				);
			}
        }
    }

	Manager manager(m_window);

	Button button_save(120, 30, "SAVE");
	button_save.setPosition(m_window.getSize().x - 130, 10);
	manager.addWidget(button_save);
	
	Entry entry_save(150, 30);
	entry_save.setPosition(m_window.getSize().x - 290, 10);
	entry_save.setText("example.png");
	manager.addWidget(entry_save);

	Picture picture(m_image);
	picture.setPosition(0, HEAD_HEIGHT);
	picture.setScale(m_zoom);
	manager.addWidget(picture);
	
    std::stringstream sstr;
    sstr << std::setprecision(2) << std::scientific << "max: " << max_value << " | ";
    sstr << std::setprecision(2) << std::scientific << "min: " << min_value << std::endl;;
    
	Label label_text(sstr.str());
	label_text.setPosition(7, 10);
	manager.addWidget(label_text);	
	
	sf::RectangleShape separator(sf::Vector2f(m_window.getSize().x, SHAPE_OUTLINE));
	separator.setPosition(0, HEAD_HEIGHT);
	
	while (m_window.isOpen()) {
	
		manager.update();
	
		if (manager.closeEvent())
			m_window.close();
		
		if (button_save.isClicked())	
			m_image.saveToFile(entry_save.getText());

		globalMutex.lock();

		m_window.clear();
		manager.render();	
		m_window.draw(separator);				
		m_window.display();

		globalMutex.unlock();
	}
}

void DataViewer::wait() {
	m_thread_p->wait();	
}

DataViewer::~DataViewer() {
//	std::cout << "dtv: wait" << std::endl;
	m_thread_p->wait();
	delete m_thread_p;
}
}

