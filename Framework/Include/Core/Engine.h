/////////////////////////////////////////////////////////////////////////////////////
// The articulated hand puppet is drawn using the following minimal graphics engine.
// Edges are rendered using the Bresenham algorithm, and a flesh hull can be 
// optionally drawn using the Meijster distance transform of the wire frame. 
/////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include "Puppet.h"
#include "Distance.h"

#include <iostream>
#include <fstream>

using namespace Eigen;

namespace HLF
{
class Engine
{
  public:
    Engine(int f);
    // Wire Frame rendered via rasterization
    void wireFrame(Puppet &puppet, int w, int h);
    // Flesh hull rendered using the Meijster algorithm
    void fleshHull(double thickness, double fuzziness);
    // Set global cartesian pose
    void setPosition(double x, double y, double z);
    // Set the rotation around the X axis
    void setRotX(double mag);
    // Set the rotation around the Y axis
    void setRotY(double mag);
    // Set the rotation around the Z axis
    void setRotZ(double mag);
    // Get a copy of the screen
    MatrixXi getScreen();

  private:
    double m_fov;
    MatrixXi m_screen;
    Matrix3d m_Rx, m_Ry, m_Rz;
    Vector3d m_T;

    void flatLine(int x1, int x2, int y);
    void bresenham(Vector2i A, Vector2i B);
    void triangle(Vector2d A, Vector2d B, Vector2d C);
};
}

