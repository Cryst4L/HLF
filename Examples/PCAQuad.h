/////////////////////////////////////////////////////////////////////////////////////
// An example recipe for regressing the hand pose from a binary input using HLF,
// consisting of a PCA compression followed by a quadratic classifier.
// Suggested data set: sample_size = 32; nb_samples = 20K
// Author: B.Halimi - 2016 
/////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include <Eigen/Eigenvalues>

/*
#include <sys/time.h>
int get_wall_time() {
    struct timeval time;
    gettimeofday(&time, NULL);
    return time.tv_sec;
}
std::cout << " # time elapsed = " << get_wall_time() - starting_time << " (sec)" << std::endl;
*/

class PCAQuad : public HLF::Model
{
private:

	// Hyper-Parameters /////////////////////////////////////////////////////////////
	static const double PCA_RATIO	= 0.05;

	// Model parameters /////////////////////////////////////////////////////////////
    VectorXd m_magnitudes;
    MatrixXd m_components;
    MatrixXd m_theta;

	// Training /////////////////////////////////////////////////////////////////////
    void train(std::vector <MatrixXd> &data, std::vector <VectorXd> &targets) { 

    	int n = data[0].rows() * data[0].cols();

		// 1.Form the design Matrix 
        MatrixXd design(data.size(), n);
		for (int i = 0; i < (int) data.size(); i++)
        	design.row(i) = Map <VectorXd> (data[i].data(), n);

		// 2.Form the output Matrix 
		MatrixXd output(data.size(), HLF::DOF);
		for (int i = 0; i < (int) targets.size(); i++)
        	output.row(i) = targets[i];

    	// 3.Perform Dimensionality Reduction
        MatrixXd covariance = (design.transpose() * design) / design.rows();

        SelfAdjointEigenSolver<MatrixXd> es(covariance);

        m_components = es.eigenvectors().reverse().eval(); 
        m_magnitudes = es.eigenvalues().reverse().eval();
	
		int nb_components = PCA_RATIO * m_magnitudes.size();
//		std::cout << " # nb of components used = " << nb_components << std::endl;

        double explained_variance = m_magnitudes.segment(0, nb_components).sum() / m_magnitudes.sum();
//		std::cout << " # variance retained = " << explained_variance << std::endl;

		// 4.Project the Design Matrix onto the most significant PCA vectors
		MatrixXd compressed = design * m_components.leftCols(nb_components);

		// 5.Construct the Quadratic Expansion of the Compressed Data
		MatrixXd expanded(design.rows(), (nb_components + 2) * (nb_components + 1) / 2);
		
		expanded.col(0).setOnes(); // 0th order (bias)

		expanded.block(0, 1, design.rows(), nb_components) = compressed; // 1st order (as is)

		int index = nb_components + 1;
		for (int i = 0; i < nb_components; i++)
			for (int j = i; j < nb_components; j++, index++)
				expanded.col(index) = compressed.col(i).cwiseProduct(compressed.col(j)); // 2nd order coefs

		// 6.Perform Regression
		m_theta = (expanded.transpose() * expanded).ldlt().solve(expanded.transpose() * output);

		showPCA();
	}

	// Inference ////////////////////////////////////////////////////////////////////
    VectorXd predict(MatrixXd &sample) {

		int nb_components = PCA_RATIO * m_magnitudes.size();

		VectorXd unshaped = Map <VectorXd> (sample.data(), sample.size());

		// 1.Compression
		VectorXd compressed = m_components.leftCols(nb_components).transpose() * unshaped;

		// 2.Expansion
        VectorXd expanded((nb_components + 2) * (nb_components + 1) / 2);

        expanded(0) = 1; // 0th order

        expanded.segment(1, nb_components) = compressed; // 1st order

		int index = nb_components + 1;
		for (int i = 0; i < nb_components; i++)
			for (int j = i; j < nb_components; j++, index++)
				expanded(index) = compressed(i) * compressed(j); // 2nd order 
		
		// 3.Projection
        return (m_theta.transpose() * expanded);
    }

    // Display the Model Parameters /////////////////////////////////////////////////
    void showPCA()
    {
		int nb_components = PCA_RATIO * m_magnitudes.size();

        int size = std::sqrt(m_magnitudes.size());
      
		std::vector <MatrixXd> main_components;

		for (int i = 0; i < nb_components; i++) {
			VectorXd sample = m_components.col(i);
			MatrixXd reshaped = Map <MatrixXd> (sample.data(), size, size);
			main_components.push_back(reshaped);
		}
 
        HLF::DataViewer view_pca(main_components);
    }

};

