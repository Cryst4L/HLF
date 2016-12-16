#include "Interface/BenchPanel.h"

namespace HLF
{
double BenchPanel::wave(double x) { 
	return 0.5 * (1 + sin(x)); 
}

BenchPanel::BenchPanel(Model *model_p, int sample_size)
:	m_model_p(model_p), m_sample_size(sample_size), m_manager(m_window),
    m_walk(DOF), m_state(0.5 * VectorXd::Ones(DOF)), m_predicted(DOF), m_averaged(DOF),
    m_speed(ENTROPY * VectorXd::Random(DOF)), m_puppet(MESH_PATH), m_sampler(m_puppet),
	m_motion_record(MatrixXd::Zero(DOF, MAX_INERTIA))
{}
		
void BenchPanel::run() {

	sf::VideoMode mode(2 * PREVIEW_SIZE, PREVIEW_SIZE + HEADER_SIZE);
	m_window.create(mode, "[HLF] Model Benchmark Panel", sf::Style::Close);
	m_window.setVerticalSyncEnabled(true);

	m_screen = MatrixXi::Zero(2 * PREVIEW_SIZE, PREVIEW_SIZE); 
	m_image_screen.create(m_screen.rows(), m_screen.cols());

	m_image_input.create(m_sample_size, m_sample_size);

	Label label_speed("Motion Speed   :");
	label_speed.setPosition(15, 15);
	m_manager.addWidget(label_speed);

	Slider slider_speed(100, 30);
	slider_speed.setPosition(190, 15);
	m_manager.addWidget(slider_speed);

	Label label_inertia("Output Inertia :");
	label_inertia.setPosition(15, 60);
	m_manager.addWidget(label_inertia);

	Slider slider_inertia(100, 30);
	slider_inertia.setPosition(190, 60);
	m_manager.addWidget(slider_inertia);

	Label label_time("Inference Time :");
	label_time.setPosition(15, 105);
	m_manager.addWidget(label_time);

	Label label_cost("[Inference Cost]");
	label_cost.setPosition(325, 15);
	m_manager.addWidget(label_cost);

	Label label_l1("L1 :");
	label_l1.setPosition(325, 60);
	m_manager.addWidget(label_l1);

	Label label_l2("L2 :");
	label_l2.setPosition(325, 105);
	m_manager.addWidget(label_l2);

	Picture picture_input;
	picture_input.setPosition(2 * PREVIEW_SIZE - HEADER_SIZE + 30, 45);
	picture_input.setScale((HEADER_SIZE - 60.) / m_sample_size); 
	m_manager.addWidget(picture_input);

	Label label_input("  [Input]");
	label_input.setPosition(2 * PREVIEW_SIZE - HEADER_SIZE + 15, 15);
	m_manager.addWidget(label_input);

	Picture picture_screen;
	picture_screen.setPosition(0, HEADER_SIZE);
	m_manager.addWidget(picture_screen);

	Label label_ground("Ground Truth");
	label_ground.setPosition(10, HEADER_SIZE + 5);
	m_manager.addWidget(label_ground);

	Label label_prediction("Prediction");
	label_prediction.setPosition(PREVIEW_SIZE + 10, HEADER_SIZE + 5);
	m_manager.addWidget(label_prediction);

	sf::RectangleShape separator_a(sf::Vector2f(SHAPE_OUTLINE, HEADER_SIZE));
	separator_a.setPosition(310, 0);

	sf::RectangleShape separator_b(sf::Vector2f(SHAPE_OUTLINE, HEADER_SIZE));
	separator_b.setPosition(2 * PREVIEW_SIZE - HEADER_SIZE, 0);

	sf::RectangleShape separator_c(sf::Vector2f(m_window.getSize().x, SHAPE_OUTLINE));
	separator_c.setPosition(0, HEADER_SIZE);

	sf::RectangleShape separator_d(sf::Vector2f(SHAPE_OUTLINE, PREVIEW_SIZE));
	separator_d.setPosition(PREVIEW_SIZE, HEADER_SIZE);

	ExitBox exit_box;

	m_scale_speed = slider_speed.getValue() * MAX_SPEED;
	m_scale_inertia = slider_inertia.getValue() * MAX_INERTIA;

	// MAIN LOOP ////////////////////////////////////////////////////////////////
	while (m_window.isOpen()) {
	
		// EVENT PROCESSING /////////////////////////////////////////////////////
		m_manager.update();
		
		if (m_manager.closeEvent()) {
			exit_box.run();
		}

		if (slider_speed.isGrabbed()) {
			m_scale_speed = slider_speed.getValue() * MAX_SPEED;
		}

		if (slider_inertia.isGrabbed()) {
			m_scale_inertia = slider_inertia.getValue() * MAX_INERTIA;
		}
		
		// COMPUTE THE PREVIEW //////////////////////////////////////////////////
        m_speed *= (1 - DAMPENING);
        m_speed += m_scale_speed * VectorXd::Random(DOF);  
        m_walk += ENTROPY * m_speed;      
/*
	    m_speed += ENTROPY * VectorXd::Random(DOF);
        m_speed *= (1 - DAMPENING);
        m_walk += m_scale_speed * m_speed;
*/
        for (int i = 0; i < 10; i++) {
			m_state(i) = wave(m_walk(i));
            m_sampler.setDoF(i, m_state(i));
        }

		// INFER THE NEW OUTPUT /////////////////////////////////////////////////
	    MatrixXd sample = m_sampler.createAdjusted(m_sample_size, false).cast <double> ();

	    for(int x = 0; x < m_sample_size; x++)
	        for(int y = 0; y < m_sample_size; y++) {
	            int value = sample(x, y);
	            m_image_input.setPixel(x, y, sf::Color(value, value, value));
	        }

	    sample.array() = (sample / 128).array() - 1;

		struct timeval t0, t1;
		std::ostringstream strstr;

		gettimeofday(&t0, NULL);
	    m_predicted = m_model_p->predict(sample);
		gettimeofday(&t1, NULL);

		strstr.precision(2);
		strstr << std::fixed << std::setw(6) << (t1.tv_usec - t0.tv_usec) / 1e3;
		label_time.setText("Inference Time : [" + strstr.str() + " ms]");
		strstr.str("");

		m_motion_record.rightCols(MAX_INERTIA - 1) = m_motion_record.leftCols(MAX_INERTIA - 1).eval();
		m_motion_record.col(0) = m_predicted;
		
		if (m_scale_inertia == 0)  m_scale_inertia = 1;
		m_averaged = m_motion_record.leftCols(m_scale_inertia).rowwise().mean();

		double l1_cost = (m_averaged - m_state).cwiseAbs().sum();
		strstr << std::fixed << std::setw(7) << l1_cost;
		label_l1.setText("L1 :   [" + strstr.str() + "]");
		strstr.str("");

		double l2_cost = (m_averaged - m_state).norm();
		strstr << std::fixed << std::setw(7) << l2_cost;
		label_l2.setText("L2 :   [" + strstr.str() + "]");
		strstr.str("");


		// COMPUTE THE PREVIEWS /////////////////////////////////////////////////
	    m_screen.block(0, 0, PREVIEW_SIZE, PREVIEW_SIZE) = m_sampler.createRaw(PREVIEW_SIZE, true);

	    for (int i = 0; i < DOF; i++)
	        m_sampler.setDoF(i, m_averaged(i));

	    m_screen.block(PREVIEW_SIZE - 1, 0, PREVIEW_SIZE, PREVIEW_SIZE) = m_sampler.createRaw(PREVIEW_SIZE, true);
	    
	    for(int x = 0; x < m_screen.rows(); x++)
	        for(int y = 0; y < m_screen.cols(); y++) {
	            int value = m_screen(x, y);
	            m_image_screen.setPixel(x, y, sf::Color(value, value, value));
	        }


		// RENDERING ////////////////////////////////////////////////////////////
		m_window.clear();
		
		picture_input.setImage(m_image_input);
		picture_screen.setImage(m_image_screen);

		m_manager.render();

		m_window.draw(separator_a);
		m_window.draw(separator_b);
		m_window.draw(separator_c);
		m_window.draw(separator_d);
	
		m_window.display();
	}
}

BenchPanel::~BenchPanel() {}
}