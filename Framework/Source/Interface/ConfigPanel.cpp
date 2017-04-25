#include "Interface/ConfigPanel.h"

namespace HLF
{

ConfigPanel::ConfigPanel()
	:	m_manager(m_window), m_theta(0),
	  m_puppet(MESH_PATH), m_sampler(m_puppet),
	  m_state(0.5 * VectorXd::Ones(DOF)), m_speed(VectorXd::Random(DOF))
{}

double ConfigPanel::wave(double x)
{
	return 0.5 * (1 + sin(x));
}

double ConfigPanel::clamp(double x, double lower, double upper)
{
	return std::max(lower, std::min(x, upper));
}

void ConfigPanel::run()
{
	sf::ContextSettings settings(0, 0, ANTI_ALIASING);
	m_window.create(sf::VideoMode(720, 450), "", sf::Style::Close, settings);

	m_window.setTitle("[HLF] Dataset Configuration Panel");
	m_window.setVerticalSyncEnabled(true);
	m_window.setFramerateLimit(60);

	ExitBox exit_box;
	bool first_iteration = true;

	// BANNER //////////////////////////////////////////////////////////////////

	Picture picture_banner;
	picture_banner.loadFromFile(LOGO_PATH);
	picture_banner.setPosition(15, 15);
	picture_banner.showOutline();
	m_manager.addWidget(picture_banner);

	// PREVIEW /////////////////////////////////////////////////////////////////

	Canva canva_preview(330, 330);
	canva_preview.setPosition(15, 105);
	canva_preview.setFillColor(sf::Color::Black);
	m_manager.addWidget(canva_preview);

	m_preview.create(PREVIEW_SIZE, PREVIEW_SIZE);

	Picture picture_preview(m_preview);
	picture_preview.setPosition(30, 110);
	m_manager.addWidget(picture_preview);

	Canva canva_label_preview(200, 25);
	canva_label_preview.setPosition(30, 90);
	m_manager.addWidget(canva_label_preview);

	Label label_preview("Rendering Preview");
	label_preview.setPosition(35, 90);
	m_manager.addWidget(label_preview);

	// NEUTRAL PANEL ///////////////////////////////////////////////////////////

	Canva canva_neutral(345, 155);
	canva_neutral.setPosition(360, 30);
	m_manager.addWidget(canva_neutral);

	Canva canva_label_neutral(175, 25);
	canva_label_neutral.setPosition(375, 15);
	m_manager.addWidget(canva_label_neutral);

	Label label_neutral("Neutral Position");
	label_neutral.setPosition(380, 15);
	m_manager.addWidget(label_neutral);

	Label label_alpha("Alpha   :");
	label_alpha.setPosition(375, 50);
	m_manager.addWidget(label_alpha);

	Slider slider_alpha(90, 30);
	slider_alpha.setPosition(490, 50);
	m_manager.addWidget(slider_alpha);

	Entry entry_alpha(90, 30, true);
	entry_alpha.setPosition(600, 50);
	m_manager.addWidget(entry_alpha);

	Label label_beta("Beta    :");
	label_beta.setPosition(375, 95);
	m_manager.addWidget(label_beta);

	Slider slider_beta(90, 30);
	slider_beta.setPosition(490, 95);
	m_manager.addWidget(slider_beta);

	Entry entry_beta(90, 30, true);
	entry_beta.setPosition(600, 95);
	m_manager.addWidget(entry_beta);

	Label label_gamma("Gamma   :");
	label_gamma.setPosition(375, 140);
	m_manager.addWidget(label_gamma);

	Slider slider_gamma(90, 30);
	slider_gamma.setPosition(490, 140);
	m_manager.addWidget(slider_gamma);

	Entry entry_gamma(90, 30, true);
	entry_gamma.setPosition(600, 140);
	m_manager.addWidget(entry_gamma);

	// DATASET CONFIGUATION PANEL ///////////////////////////////////////////////////////////

	Canva canva_config(345, 155);
	canva_config.setPosition(360, 210);
	m_manager.addWidget(canva_config);

	Canva canva_label_config(175, 25);
	canva_label_config.setPosition(375, 195);
	m_manager.addWidget(canva_label_config);

	Label label_config("Meta Data");
	label_config.setPosition(380, 195);
	m_manager.addWidget(label_config);

	Label label_size("Samples Size  :");
	label_size.setPosition(375, 230);
	m_manager.addWidget(label_size);

	Entry entry_size(150, 30, true);
	entry_size.setPosition(540, 230);
	m_manager.addWidget(entry_size);

	Label label_number("Nb of Samples :");
	label_number.setPosition(375, 275);
	m_manager.addWidget(label_number);

	Entry entry_number(150, 30, true);
	entry_number.setPosition(540, 275);
	m_manager.addWidget(entry_number);

	Label label_seed("Random Seed   :");
	label_seed.setPosition(375, 320);
	m_manager.addWidget(label_seed);

	Entry entry_seed(150, 30, true);
	entry_seed.setPosition(540, 320);
	m_manager.addWidget(entry_seed);

	Button button_reset(165, 55, "RESET");
	button_reset.setPosition(360, 380);
	m_manager.addWidget(button_reset);

	Button button_generate(165, 55, "GENERATE");
	button_generate.setPosition(540, 380);
	m_manager.addWidget(button_generate);

	// MAIN LOOP ///////////////////////////////////////////////////////////////

	while (m_window.isOpen()) {

		m_manager.update();
		if (m_manager.closeEvent()) {
			exit_box.run();
		}

		// CONTROL THE CONFIGURATIONS //////////////////////////////////////////

		if (entry_size.newEntry()) {
			m_sample_size = clamp(entry_size.getValue(), MIN_SIZE, MAX_SIZE);
			entry_size.setValue(m_sample_size);
		}

		if (entry_number.newEntry()) {
			m_sample_number = clamp(entry_number.getValue(), MIN_NMBR, MAX_NMBR);
			entry_number.setValue(m_sample_number);
		}

		if (entry_seed.newEntry()) {
			m_sample_seed = std::max(0.f, entry_seed.getValue());
			entry_seed.setValue(m_sample_seed);
		}

		if (entry_alpha.newEntry()) {
			m_neutral(0) = clamp(entry_alpha.getValue(), -0.5 * M_PI, 0.5 * M_PI);
			entry_alpha.setValue(m_neutral(0));
			slider_alpha.setValue(m_neutral(0) / M_PI + 0.5);
		}

		if (entry_beta.newEntry()) {
			m_neutral(1) = clamp(entry_beta.getValue(),  -0.5 * M_PI, 0.5 * M_PI);
			entry_beta.setValue(m_neutral(1));
			slider_beta.setValue(m_neutral(1) / M_PI + 0.5);
		}

		if (entry_gamma.newEntry()) {
			m_neutral(2) = clamp(entry_gamma.getValue(), -0.5 * M_PI, 0.5 * M_PI);
			entry_gamma.setValue(m_neutral(2));
			slider_gamma.setValue(m_neutral(2) / M_PI + 0.5);
		}

		if (slider_alpha.isGrabbed()) {
			m_neutral(0) = (slider_alpha.getValue() - 0.5) * M_PI;
			entry_alpha.setValue(m_neutral(0));
		}

		if (slider_beta.isGrabbed()) {
			m_neutral(1) = (slider_beta.getValue() - 0.5) * M_PI;
			entry_beta.setValue(m_neutral(1));
		}

		if (slider_gamma.isGrabbed()) {
			m_neutral(2) = (slider_gamma.getValue() - 0.5) * M_PI;
			entry_gamma.setValue(m_neutral(2));
		}

		if (first_iteration || button_reset.isClicked()) {

			m_neutral.setZero();
			entry_alpha.setValue(0);
			entry_beta.setValue(0);
			entry_gamma.setValue(0);

			m_sample_size = DEF_SIZE;
			entry_size.setValue(m_sample_size);

			m_sample_number = DEF_NMBR;
			entry_number.setValue(m_sample_number);

			m_sample_seed = DEF_SEED;
			entry_seed.setValue(m_sample_seed);

			first_iteration = false;
		}

		if (button_generate.isClicked()) {
			m_window.close();
		}

		// COMPUTE THE PREVIEW /////////////////////////////////////////////////

		m_sampler.setNeutral(m_neutral);

		MatrixXi sample = m_sampler.createRaw(PREVIEW_SIZE, true, 35.);

		m_speed *= (1 - DAMPENING);
		m_speed += VectorXd::Random(DOF);
		m_state += ENTROPY * m_speed;

		for (int i = 0; i < 10; i++) {
			m_sampler.setDoF(i, wave(m_state(i)));
		}

		for (int x = 0; x < PREVIEW_SIZE; x++) {
			for (int y = 0; y < PREVIEW_SIZE; y++) {
				int v = sample(x, y);
				m_preview.setPixel(x, y, sf::Color(v, v, v));
			}
		}

		// RENDERING OF THE GUI ////////////////////////////////////////////////

		picture_preview.setImage(m_preview);

		m_manager.render();

		m_window.display();
	}
}

int ConfigPanel::getSize()
{
	return m_sample_size;
}

int ConfigPanel::getNumber()
{
	return m_sample_number;
}

int ConfigPanel::getSeed()
{
	return m_sample_seed;
}

Vector3d ConfigPanel::getNeutral()
{
	return m_neutral;
}

ConfigPanel::~ConfigPanel() {};

}
