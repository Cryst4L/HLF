/////////////////////////////////////////////////////////////////////////////////////
// A Layer class, for the MLP model implemented in 'MLP.h'
//Author: B.Halimi - 2016 
/////////////////////////////////////////////////////////////////////////////////////
#pragma once

enum IO {I, H, O};
enum ActFunc {SIGM, RELU, LIN}; 

class Layer
{ 
  public:

    // Layer attributes /////////////////////////////////////////////////////////////
    int input_size;
	int output_size;

	IO layer_type;
    ActFunc activation;    

    MatrixXd theta, grad;
    VectorXd potential, output;
    VectorXd delta, eout;
    
  	Layer(int m, int n, ActFunc af, IO io)
  	: input_size(m), output_size(n), 
	  activation(af), layer_type(io) {

		theta = MatrixXd::Zero(n, m + 1);
		grad = MatrixXd::Zero(n, m + 1);

		potential = VectorXd::Zero(n);
		output = VectorXd::Zero(n + 1);
		
		delta = VectorXd::Zero(n);
		eout = VectorXd::Zero(m);

		theta.setRandom();
		theta *= std::sqrt(6. / (m + n));
	}

	// Forward Propagation //////////////////////////////////////////////////////////
	void forwardProp(VectorXd& input) {
		potential = theta * input;
		output.head(output_size) = actFunc(potential);
		output(output_size) = 1.; // setup the bias
	}
	
	// Back Propagation /////////////////////////////////////////////////////////////
	void backProp(VectorXd input, VectorXd ein) {
		delta = ein.cwiseProduct(actGrad(potential));
		grad += delta * input.transpose();
		if (layer_type != I) 
			eout = (theta.transpose() * delta).head(input_size); // ein ???
	}
	
	// Activation function //////////////////////////////////////////////////////////
	VectorXd actFunc(VectorXd& X) {
		VectorXd Y(X.size());
		switch(activation) {
		    case SIGM : 
		    	Y.array() = 1.0 / (1.0 + (-X).array().exp()); 
		    	break;
		    case RELU : 
		    	Y.array() = X.array().max(0); 
		    	break;
		    case LIN  : 
		    	Y.array() = X.array(); 
		    	break;
		}
		return Y;
	}
	
	// Activation gradient //////////////////////////////////////////////////////////
	VectorXd actGrad(VectorXd& X) {
		VectorXd Y(X.size());
		switch(activation) {
		    case SIGM : 
		    	Y.array() = actFunc(X).array() * (1. - actFunc(X).array()); 
		    	break;
		    case RELU : 
		    	Y.array() = (X.array() > 0).cast<double>(); 
		    	break;
		    case LIN  : 
		    	Y.setOnes(); 
		    	break;
		}
		return Y;
	}
};
