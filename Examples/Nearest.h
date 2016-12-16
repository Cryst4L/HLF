////////////////////////////////////////////////////////////////////////////////
// Straight-up implementation of Nearest Neighbour's algorithm for HLF,
// a C++ Machine Learning Playground, dedicated to hand pose estimation
// Author: B.Halimi - 2016
////////////////////////////////////////////////////////////////////////////////
#pragma once

class Nearest : public HLF::Model
{
    private:

	// Model parameters ////////////////////////////////////////////////////////
	std::vector <MatrixXd> m_data;
	std::vector <VectorXd> m_targets;

	// Training ////////////////////////////////////////////////////////////////
	void train(std::vector <MatrixXd> &data, std::vector <VectorXd> &targets)
	{
		m_data = data;
		m_targets = targets;
	}

	// Inference ///////////////////////////////////////////////////////////////
	VectorXd predict(MatrixXd &sample)
	{
		int neighbour_index = 0;
		double neighbour_cost = +INFINITY;

		// Loop over the dataset
		for (size_t i = 0; i < m_data.size(); i++) {
			double cost = (m_data[i] - sample).squaredNorm();

			if (cost < neighbour_cost) {
				neighbour_index = i;
				neighbour_cost = cost;
			}
		}

		return m_targets[neighbour_index];
	}
};
