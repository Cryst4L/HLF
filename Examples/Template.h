/////////////////////////////////////////////////////////////////////////////////////
//	Template of a HLF learning model
//	Author: B.Halimi - 2016
/////////////////////////////////////////////////////////////////////////////////////
#pragma once

class Template : public HLF::Model
{
    private:

	// Hyper-Parameters /////////////////////////////////////////////////////////////
	static const double PARAM = 0.;

	// Model parameters ////////////////////////////////////////////////////////
	MatrixXd _theta;

	// Training ////////////////////////////////////////////////////////////////
	void train(std::vector <MatrixXd> &data, std::vector <VectorXd> &targets)
	{
		// Do stuff ...
	}

	// Inference ///////////////////////////////////////////////////////////////
	VectorXd predict(MatrixXd &sample)
	{
		VectorXd prediction;

		// Do stuff ...

		return prediction;
	}

};

