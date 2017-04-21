#include "Interface/DataViewer.h"

namespace HLF
{
DataViewer::DataViewer(std::vector <MatrixXd> data, double zoom, 
	int nrows, int padding, bool normalize)
:	m_data(data), m_zoom(zoom), m_normalized(normalize), m_padding(padding),
	m_nrows(nrows), m_ncols(0)
{
	m_nrows = (nrows != 0) ? nrows : std::sqrt(data.size());
	m_ncols = (data.size() + m_nrows - 1) / m_nrows;

	m_sample_width  = data[0].rows();
	m_sample_height = data[0].cols();

	int image_width  = (m_sample_width  + 1) * m_ncols + 1;
	int image_height = (m_sample_height + 1) * m_nrows + 1;

	int app_width  = m_zoom * image_width + 2 * BORDER;
	int app_height = m_zoom * image_height + 3 * BORDER + 30;

	app_width = std::max(app_width, MIN_WIDTH);

	m_image.create(image_width, image_height);

	sf::VideoMode video_mode(app_width, app_height);
	sf::ContextSettings settings(0, 0, ANTI_ALIASING);

	m_window.create(video_mode, "", sf::Style::Close, settings);
	m_window.setTitle("[HLF] Data Viewer");
	m_window.setActive(false);

	m_thread_p = new sf::Thread(&DataViewer::run, this);
	m_thread_p->launch();
}

void DataViewer::run()
{
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

	std::stringstream sstr;
	sstr << std::setprecision(2) << std::scientific << "max:" << max_value << " | ";
	sstr << std::setprecision(2) << std::scientific << "min:" << min_value;

	Label label_text(sstr.str());
	label_text.setPosition(BORDER, BORDER + 3);
	label_text.showOutline();
	manager.addWidget(label_text);

	Entry entry_save(150, 30);
	entry_save.setPosition(m_window.getSize().x - 280 - BORDER, BORDER);
	entry_save.setText("data.png");
	manager.addWidget(entry_save);

	Button button_save(120, 30, "SAVE");
	button_save.setPosition(m_window.getSize().x - 120 - BORDER, BORDER);
	manager.addWidget(button_save);

	Picture picture(m_image);
	int offset = 0.5 * (m_window.getSize().x - m_zoom * m_image.getSize().x);
	picture.setPosition(offset, 2 * BORDER + 30);
	picture.setScale(m_zoom);
	picture.showOutline();
	manager.addWidget(picture);

//	sf::RectangleShape separator(sf::Vector2f(m_window.getSize().x, SHAPE_OUTLINE));
//	separator.setPosition(0, HEAD_HEIGHT);

	while (m_window.isOpen()) {
		manager.update();

		if (manager.closeEvent())
			m_window.close();

		if (button_save.isClicked())
			m_image.saveToFile(entry_save.getText());

		globalMutex.lock();

		manager.render();
		m_window.display();

		globalMutex.unlock();
	}
}

void DataViewer::wait()
{
	m_thread_p->wait();
}

DataViewer::~DataViewer()
{
//	std::cout << "dtv: wait" << std::endl;
	m_thread_p->wait();
	delete m_thread_p;
}
}
