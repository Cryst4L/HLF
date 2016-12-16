#include "Core/Dataset.h"

namespace HLF
{
Dataset::Dataset(Vector3d neutral, int seed)
	:	m_neutral(neutral), m_seed(seed) {}

void Dataset::build(int data_size, int wh)
{
	srand((unsigned int) m_seed);
	Puppet puppet(MESH_PATH);
	Sampler sampler(puppet);
	MatrixXd sample(wh, wh);
	double progress;
	StatusBox status(progress);
	status.setText("Generating the Dataset ...");

	for (int i = 0; i < data_size; i++) {
		ArrayXd dof = 0.5 * (ArrayXd::Random(DOF) + 1);

		for (int d = 0; d < DOF; d++)
			sampler.setDoF(d, dof(d));

		sample = sampler.createAdjusted(wh, false).cast <double> ();
		sample.array() = (sample / 128).array() - 1;
		m_data.push_back(sample);
		m_targets.push_back(dof);
		progress = (i + 1.f) / data_size;
	}
}

void Dataset::setSeed(int seed)
{
	m_seed = seed;
}

void Dataset::setNeutral(Vector3d& neutral)
{
	m_neutral = neutral;
}

std::vector <MatrixXd>& Dataset::getData()
{
	return m_data;
}

std::vector <VectorXd>& Dataset::getTargets()
{
	return m_targets;
}
}

