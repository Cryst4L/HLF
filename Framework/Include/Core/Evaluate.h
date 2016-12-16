/////////////////////////////////////////////////////////////////////////////////////
// The following routine evaluate the error of a model over an input dataset
/////////////////////////////////////////////////////////////////////////////////////
#pragma once
#include "Model.h"
#include "Dataset.h"

namespace HLF
{

enum CostType {MSE, MAE};

VectorXd evaluate(
	Model *model, 
	std::vector <MatrixXd> &data,  
	std::vector <VectorXd> &targets,
	CostType costType);
}
