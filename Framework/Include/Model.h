/////////////////////////////////////////////////////////////////////////////////////
// This file describes the base Class of HLF's Machine Learning models.
// You model must inherit from it in order to be used by the Framework.
// Both the training and the inference methods are purely virtual
// so you must declare both of them.
/////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include <vector>
#include <Eigen/Core>
using namespace Eigen;

namespace HLF
{
class Model
{
public :
	// Training Method
	virtual void train(std::vector <MatrixXd> &data, std::vector <VectorXd> &targets) = 0;

	// Sample Propagation Method
	virtual VectorXd predict(MatrixXd &sample) = 0;

	// Dtor
	virtual ~Model() {};
};
}
