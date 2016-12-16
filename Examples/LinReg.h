/////////////////////////////////////////////////////////////////////////////////////
//	Straight implementation of Biased and Regularized Multilinear Regression
//	for HLF, the Hand Learning Framework.
//	Author: B.Halimi - 2016 
/////////////////////////////////////////////////////////////////////////////////////
#pragma once

class LinReg : public HLF::Model
{
private:

	// Hyper-Parameters /////////////////////////////////////////////////////////////
	static const double L2_PENALITY = 1e3;

	// Model parameters /////////////////////////////////////////////////////////////
    MatrixXd m_theta;

	// Training /////////////////////////////////////////////////////////////////////
    void train(std::vector <MatrixXd> &data, std::vector <VectorXd> &targets) { 

    	int dim = 1 + data[0].rows() * data[0].cols();

		// Form the design Matrix 
        MatrixXd design(data.size(), dim);
        design.col(0).setOnes(); // bias
		for (int i = 0; i < (int) data.size(); i++)
        	design.row(i).tail(dim - 1) = Map <VectorXd> (data[i].data(), dim - 1);

		// Form the output Matrix 
		MatrixXd output(data.size(), HLF::DOF);
		for (int i = 0; i < (int) targets.size(); i++)
        	output.row(i) = targets[i];

		// Form the regularization Matrix
        MatrixXd penality_matrix = MatrixXd::Zero(dim, dim);
        penality_matrix.diagonal().segment(1, dim - 1).array() += L2_PENALITY;

		// Gauss style's Linear Least Squares (the regression itself)
        m_theta = (design.transpose() * design + penality_matrix).ldlt().solve(design.transpose() * output);

        display();
    }

	// Inference ////////////////////////////////////////////////////////////////////
    VectorXd predict(MatrixXd &sample) {

        int dim = m_theta.rows();

        VectorXd input(dim);
        input.tail(dim - 1) = Map <VectorXd> (sample.data(), dim - 1);
        input(0) = 1;

        return (m_theta.transpose() * input);
    }

    // Display the Model Parameters /////////////////////////////////////////////////
    void display() {

        int dim = m_theta.rows();
        int size = std::sqrt(dim - 1);

		std::vector <MatrixXd> regressed_patterns;

		for (int i = 0; i < HLF::DOF; i++) {
			VectorXd sample = m_theta.col(i).tail(dim - 1);
			MatrixXd reshaped = Map <MatrixXd> (sample.data(), size, size);
			regressed_patterns.push_back(reshaped);
		}

        HLF::DataViewer view_theta(regressed_patterns);

    }

};

/*
int main(void) {

	HLF::DefaultPipeline <LinReg> ();
	return 0;
}
*/


