//////////////////////////////////////////////////////////////////////////////////////////
// A C++ and Eigen3 based implementation of the Meijster algorithm,
// which computes the distance transform of a 2D array.
// We use it in to render the flesh hull around the hand skeleton.
// The Meister algoritm has a O(n²) complexity, which allows this task to be quite fast.
//////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include <Eigen/Core>
#include <vector>

namespace HLF
{

static int bowl(int x, int i, int g_i);
static int sep(int i, int u, int g_i, int g_u);

static void firstPass(Eigen::MatrixXi &input);
static void secondPass(Eigen::MatrixXi &input);

void distanceTransform(Eigen::MatrixXi &input);

}

