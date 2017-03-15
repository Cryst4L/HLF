/////////////////////////////////////////////////////////////////////////////////////
// The standard usage of HLF consist in running the default processing pipeline.
// All the user needs to do is to define a Machine Learning blackbox inheriting
// from HLF::Model, and then call the benchmarking pipeline, as shown below:
//
// 		HLF::DefaultPipeline <MyBlackBox> ()
//
// Isn't that simple ? ;)
/////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include "Core.h"
#include "Interface.h"
#include "Model.h"

namespace HLF
{
template <class T>
void DefaultPipeline()
{
	Model *instance = (Model *) new T();

	ConfigPanel config;
	config.run();

	int data_size = config.getNumber();
	int sample_size = config.getSize();
	int init_seed = config.getSeed();

	Dataset data_set(config.getNeutral(), init_seed);
	data_set.build(data_size, sample_size);

	std::vector <MatrixXd>& data = data_set.getData();
	std::vector <VectorXd>& targets = data_set.getTargets();

	std::vector <MatrixXd> subset(&data[0], &data[64]);
	DataViewer view(subset);

	InfoBox info_train(
	        "Model training will start now !\n"
	        "This might take a while ..."
	);

	instance->train(data, targets);

	info_train.close();
	InfoBox info_eval_train(
	        "Training Finised !\n"
	        "Model's performance on the train set\n"
	        "are now evaluated. Please wait ..."
	);

	double train_error = std::sqrt(evaluate(instance, data, targets, MSE).sum());

	info_eval_train.close();
	InfoBox info_test(
	        "Building a test dataset to evaluate\n"
			"generalization ..."
	);

	Dataset test_set(config.getNeutral(), time(0));
	test_set.build(1E3, sample_size);
	data = test_set.getData();
	targets = test_set.getTargets();

	info_test.close();
	InfoBox info_eval_test(
	        "Model's performance on the train set are now evaluated ..."
	);

	double test_error = std::sqrt(evaluate(instance, data, targets, MSE).sum());

	std::ostringstream strs;
	strs << "Training Finised !\n";
	strs << "L2 train error : \t" << train_error << "\n";
	strs << "L2 test  error : \t" << test_error  << "\n";
	strs << "Model will now be bench-marked ...";

	info_eval_test.close();
	InfoBox info_score(strs.str());

	BenchPanel benchmark(instance, sample_size);
	benchmark.run();

//	InfoBox info_score(strs.str());
//	info_score.close();
//	view.wait();
//	std::cout << "finished" << std::endl;

	delete instance;
}
}
