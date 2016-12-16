/////////////////////////////////////////////////////////////////////////////////////
// This file is an implementation Back-Propagation Neural Network for HLF.
// A Machine Learning model which is also referred as Multi Layer Preceptron (MLP).
// ----------------------------------------------------------------------------------
// For commodity we first implement a Layer class, and then fill the HL::Model
// training and prediction methods.
// ----------------------------------------------------------------------------------
//	Author: B.Halimi - 2016
/////////////////////////////////////////////////////////////////////////////////////
#pragma once
#include "Layer.h"

class MLP : public HLF::Model
{
    private:
	// Hyper-Parameters /////////////////////////////////////////////////////////////
	static const int N_HIDDEN_LAYER	= 3;
	static const int UNIT_PER_LAYER	= 50;
	static const ActFunc ACTIVATION = RELU;

	static const int NB_EPOCH = 100;
	static const double EPSILON = 1E-3;

	// Model parameters /////////////////////////////////////////////////////////////
	std::vector <Layer> layer_stack;

	// Build the Network Architecture ///////////////////////////////////////////////
	void buildNetwork(int input_size, int output_size)
	{
		layer_stack.push_back(Layer(input_size - 1, UNIT_PER_LAYER, ACTIVATION, I));

		for (int i = 0; i < N_HIDDEN_LAYER - 1; i++)
			layer_stack.push_back(Layer(UNIT_PER_LAYER, UNIT_PER_LAYER, ACTIVATION, H));

		layer_stack.push_back(Layer(UNIT_PER_LAYER, output_size, ACTIVATION, O));
	}

	// Forward Propagation //////////////////////////////////////////////////////////
	void forwardPropagate(VectorXd& input, VectorXd& prediction)
	{
		layer_stack[0].forwardProp(input);

		for(int i = 1; i < (int) layer_stack.size(); i++)
			layer_stack[i].forwardProp(layer_stack[i - 1].output);

		prediction = layer_stack.back().output.head(HLF::DOF);
	}

	// Back Propagation /////////////////////////////////////////////////////////////
	void backPropagate(VectorXd& input, VectorXd& output)
	{
		int l = layer_stack.size() - 1;
		layer_stack[l].backProp(layer_stack[l - 1].output, output);

		for(int i = l - 1; i > 0; i--)
			layer_stack[i].backProp(layer_stack[i - 1].output, layer_stack[i + 1].eout);

		layer_stack[0].backProp(input, layer_stack[1].eout);
	}

	// Training /////////////////////////////////////////////////////////////////////
	void train(std::vector <MatrixXd> &data, std::vector <VectorXd> &targets)
	{
		int input_size = data[0].rows() * data[0].rows() + 1;
		buildNetwork(input_size, HLF::DOF);

		for(int ep = 0; ep < NB_EPOCH; ep++) {
			double cost = 0;

			// Loop over the dataset
			for (int i = 0; i < (int) data.size(); i++) {

				// Clear the gradients
				for (int j = 0; j < (int) layer_stack.size(); j++)
					layer_stack[j].grad.setZero();

				// Setup the data
				VectorXd input(input_size);
				input.head(input_size - 1) = Map <VectorXd> (data[i].data(), input_size - 1);
				input(input_size - 1) = 1.; // setup the bias

				VectorXd target = targets[i];
				VectorXd prediction(HLF::DOF);

				// Compute the gradients
				forwardPropagate(input, prediction);
				VectorXd error = prediction - target;
				backPropagate(input, error);

				// Accumulate the cost
				cost += error.cwiseProduct(error).sum() / data.size();

				// Perform gradient descent
				for(int j = 0; j < (int) layer_stack.size(); j++)
					layer_stack[j].theta -= EPSILON * layer_stack[j].grad;
			}

			std::cout << "Epoch " << ep << " | Cost = " << cost << std::endl;
		}

		display();
	}

	// Inference ////////////////////////////////////////////////////////////////////
	VectorXd predict(MatrixXd &sample)
	{
		int input_size = sample.rows() * sample.rows() + 1;

		VectorXd input(input_size);
		input.head(input_size - 1) = Map <VectorXd> (sample.data(), input_size - 1);
		input(input_size - 1) = 1.; // setup the bias

		VectorXd prediction(HLF::DOF);

		forwardPropagate(input, prediction);

		return prediction.head(HLF::DOF);
	}

	// Display the Model Parameters /////////////////////////////////////////////////
	void display()
	{
		int input_size = layer_stack[0].input_size;
		int sample_size = std::sqrt(input_size - 1);
		std::vector <MatrixXd> patterns;

		for (int i = 0; i < UNIT_PER_LAYER; i++) {
			VectorXd sample = layer_stack[0].theta.row(i).head(input_size - 1);
			MatrixXd reshaped = Map <MatrixXd> (sample.data(), sample_size, sample_size);
			patterns.push_back(reshaped);
		}

		HLF::DataViewer view_theta(patterns);
	}
};