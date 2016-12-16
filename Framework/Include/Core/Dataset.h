#pragma once

#include "Puppet.h"
#include "Sampler.h"
#include "Evaluate.h"
#include "Interface.h"

namespace HLF
{
class Dataset
{
    private:

	std::vector <MatrixXd> m_data;
	std::vector <VectorXd> m_targets;

	int m_seed;
	Vector3d m_neutral;

    public:

	Dataset(Vector3d neutral = Vector3d::Zero(), int seed = 1234);
	// Build the hand snap-shots dataset
	void build(int data_size, int wh);
	// Set the random seed (to ensure repeatability)
	void setSeed(int seed);
	// Set the hand model anchor angular position
	void setNeutral(Vector3d& neutral);
	// Get the hand samples
	std::vector <MatrixXd>& getData();
	// Get the values of their DOFs
	std::vector <VectorXd>& getTargets();
};
}

