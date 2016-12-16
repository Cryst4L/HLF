/////////////////////////////////////////////////////////////////////////////////////
// This Puppet class implement our articulated 3D mesh.
// The wire frame take the form a simple graph {vertices, edges}.
// The articulations are implemented using the Hinge structure.
// Each hinges must be fed with:
//      - the index of the pivot vertice
//      - a swing axis consisting of the 3 associated Euler angles
//      - the set of indexes of all the vertices constrained by this hinge
/////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include <Eigen/Core>
#include <Eigen/Geometry>

#include <iostream>
#include <iomanip>
#include <vector>
#include <fstream>

using namespace Eigen;

namespace HLF
{
class Puppet
{
    private:

	struct Hinge {
		int pivot;
		Vector3d axis;
		std::vector <int> id;
		double magnitude;
	};

	std::vector <Vector3d> m_vertices;
	std::vector <Vector2i> m_edges;
	std::vector <Vector3i> m_triangles;
	std::vector <Hinge> m_hinges;

    public:

	Puppet(const char * path);

	void load(const char * path);

	// Rewrite this thing with printf (?)
	void printContent();


	// TODO: use a table to access the leafs
	void setHinge(int hi, double magnitude);

	int nbVert();
	int nbEdge();
	int nbTril();

	Vector3d getVertex(int vi);
	Vector2i getEdge(int ei);
	Vector3i getTriangle(int ti);

};
}

