#include "Core/Evaluate.h"

namespace HLF
{
VectorXd evaluate(
        Model *model,
        std::vector <MatrixXd> &data,
        std::vector <VectorXd> &targets,
        CostType costType)
{
	VectorXd cost = VectorXd::Zero(DOF);
	std::string cost_tag = "";

	switch (costType) {
		case MSE:
			cost_tag = "MSE";
			break;

		case MAE:
			cost_tag = "MAE";
			break;
	}

	double progress = 0;
	StatusBox status(progress);
	status.setText("Evaluating the " + cost_tag + " of the model ...");

	for (int i = 0; i < targets.size(); i++) {
		MatrixXd sample = data[i];
		VectorXd error = model->predict(sample) - targets[i];

		switch (costType) {
			case MSE:
				cost += error.cwiseProduct(error);
				break;

			case MAE:
				cost += error.cwiseAbs();
				break;
		}

		progress = (i + 1.f) / targets.size();
	}

	return cost / data.size();
}
}
