//////////////////////////////////////////////////////////////////////////////////////////
// The role of the sampler is to generate a snapshot of the 10 DoF hand model.
// Each degree of freedom is normalized so that they take values inside [0:1].
//
// As the user is not supposed to know the distance to the hand observed, it
// can be desirable to perform a size normalization of the sample.
//
// To do so we proceed using the following method:
//		- we compute C, the center of mass of the sample
// 		- we search for P, the farthest non zero point from it
//		- we compute R, as the distance between C and P, plus some margin
// 		- we build a square box centered on C with a width of 2 x R
//		- we crop the sample accordingto this box an resizing it to fit the requirement
//
// For the moment the resizing operation is performed using a straight bilinear scaling
//////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include "Puppet.h"
#include "Engine.h"
#include "Distance.h"

namespace HLF
{

const int DOF = 10;
static const char* MESH_PATH = "../Framework/Assets/hand.ms";

class Sampler
{
    private:
	static const double CALIBER		= 0.9;
	static const int DRAFT_SIZE		= 48;
	static const double AMPLITUDE	= 1.05;

	Puppet m_hand;
	Engine m_engine;
	Vector3d m_neutral;

	double scale(double x, double a, double b);
	void bilinear(MatrixXi& input, int wh_dest);

    public:
	Sampler(Puppet p);

	// Create a raw hand snapshot
	MatrixXi createRaw(int wh, bool skeleton = true, double dist = 40.);
	// Create a refined hand snapshot (unaliased, and normalized in size)
	MatrixXi createAdjusted(int wh, bool skeleton = true, double dist = 40.);

	void setNeutral(Vector3d neutral);
	void setDoF(int id, double dr);
};
}

