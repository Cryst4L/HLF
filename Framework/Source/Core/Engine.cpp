#include "Core/Engine.h"

namespace HLF
{
Engine::Engine(int f)
	:	m_fov(f), m_screen(MatrixXi::Zero(1, 1)), m_T(Vector3d::Zero()),
	  m_Rx(Matrix3d::Identity()), m_Ry(Matrix3d::Identity()), m_Rz(Matrix3d::Identity())
{}

void Engine::wireFrame(Puppet &puppet, int w, int h)
{
	m_screen = MatrixXi::Zero(w, h);
	Matrix3d R = m_Rz * m_Ry * m_Rx;
	std::vector <Vector2d> view;

	// Vertices projection
	for (int i = 0; i < puppet.nbVert(); i++) {
		Vector3d P = R * puppet.getVertex(i) + m_T;
		view.push_back(Vector2d(P(0) / P(2), P(1) / P(2)));
	}

	double scale = 0.5 / std::tan(m_fov * M_PI / 360);

	// Edges rendering
	for (int i = 0; i < puppet.nbEdge(); i++) {
		Vector2i O(0.5 * w, 0.5 * h);
		Vector2i A = O + (scale * h * view[puppet.getEdge(i)(0)]).cast <int> ();
		Vector2i B = O + (scale * h * view[puppet.getEdge(i)(1)]).cast <int> ();
		bool a_in = A(0) > 0 && A(0) < w && A(1) > 0 && A(1) < h;
		bool b_in = B(0) > 0 && B(0) < w && B(1) > 0 && B(1) < h;

		if (a_in && b_in)
			bresenham(A, B);
	}

	// Triangles Rasterization
	for (int i = 0; i < puppet.nbTril(); i++) {
		Vector2d O(0.5 * w, 0.5 * h);
		Vector2d A = (O + scale * h * view[puppet.getTriangle(i)(0)]);
		Vector2d B = (O + scale * h * view[puppet.getTriangle(i)(1)]);
		Vector2d C = (O + scale * h * view[puppet.getTriangle(i)(2)]);
		bool a_in = A(0) > 0 && A(0) < w && A(1) > 0 && A(1) < h;
		bool b_in = B(0) > 0 && B(0) < w && B(1) > 0 && B(1) < h;
		bool c_in = C(0) > 0 && C(0) < w && C(1) > 0 && C(1) < h;

		if (a_in && b_in && c_in)
			triangle(A, B, C);
	}
}

// Flesh hull rendered using the Meijster algorithm
void Engine::fleshHull(double thickness, double fuzziness)
{
	thickness *= 0.5 * m_screen.cols() / m_T(2);
	thickness /= std::tan(0.5 * m_fov / 180 * M_PI);
	thickness *= thickness;
	distanceTransform(m_screen);

	for(int x = 0; x < m_screen.rows(); x++)
		for(int y = 0; y < m_screen.cols(); y++)
			m_screen(x, y) = (m_screen(x, y) > thickness) ? 0 : 1;
}

// Set global cartesian pose
void Engine::setPosition(double x, double y, double z)
{
	m_T = Vector3d(x, y, z);
}

// Set the rotation around the X axis
void Engine::setRotX(double mag)
{
	m_Rx.row(0) <<               1,              0,              0;
	m_Rx.row(1) <<               0,  std::cos(mag), -std::sin(mag);
	m_Rx.row(2) <<               0,  std::sin(mag),  std::cos(mag);
}

// Set the rotation around the Y axis
void Engine::setRotY(double mag)
{
	m_Ry.row(0) <<   std::cos(mag),              0,  std::sin(mag);
	m_Ry.row(1) <<               0,              1,              0;
	m_Ry.row(2) <<  -std::sin(mag),              0,  std::cos(mag);
}

// Set the rotation around the Z axis
void Engine::setRotZ(double mag)
{
	m_Rz.row(0) <<   std::cos(mag), -std::sin(mag),              0;
	m_Rz.row(1) <<   std::sin(mag),  std::cos(mag),              0;
	m_Rz.row(2) <<               0,              0,              1;
}

// Get a copy of the screen
MatrixXi Engine::getScreen()
{
	return m_screen;
}

// Horizontal segment drawing primitive
void Engine::flatLine(int x1, int x2, int y)
{
	if (x1 < x2) {
		for(int x = x1 - 1; x < x2 + 1; x++)
			m_screen(x, y) = 1;
	} else {
		for(int x = x2 - 1; x < x1 + 1; x++)
			m_screen(x, y) = 1;
	}
}

void Engine::bresenham(Vector2i A, Vector2i B)
{
	int x0 = A(0), y0 = A(1);
	int x1 = B(0), y1 = B(1);
	int dx =  std::abs(x1 - x0);
	int sx = x0 < x1 ? 1 : -1;
	int dy = -std::abs(y1 - y0);
	int sy = y0 < y1 ? 1 : -1;
	int err = dx + dy, e2;

	while (x0 != x1 || y0 != y1) {
		m_screen(x0, y0) = 1;
		e2 = 2 * err;

		if (e2 >= dy) {
			err += dy;
			x0 += sx;
		}

		if (e2 <= dx) {
			err += dx;
			y0 += sy;
		}
	}
}

void Engine::triangle(Vector2d A, Vector2d B, Vector2d C)
{
	A << floor(A(0)), floor(A(1));
	B << floor(B(0)), floor(B(1));
	C << floor(C(0)), floor(C(1));

	if (A(1) > B(1)) A.swap(B);

	if (A(1) > C(1)) A.swap(C);

	if (B(1) > C(1)) B.swap(C);

	double dx1 = (B(1) - A(1) > 0) ? (B(0) - A(0)) / (B(1) - A(1)) : 0;
	double dx2 = (C(1) - A(1) > 0) ? (C(0) - A(0)) / (C(1) - A(1)) : 0;
	double dx3 = (C(1) - B(1) > 0) ? (C(0) - B(0)) / (C(1) - B(1)) : 0;
	Vector2d S = A, E = A;

	if(dx1 > dx2) {
		for (; S(1) < B(1); S(1)++, E(1)++, S(0) += dx2, E(0) += dx1)
			flatLine(S(0), E(0), S(1));

		E = B;

		for (; S(1) < C(1); S(1)++, E(1)++, S(0) += dx2, E(0) += dx3)
			flatLine(S(0), E(0), S(1));
	} else {
		for (; S(1) < B(1); S(1)++, E(1)++, S(0) += dx1, E(0) += dx2)
			flatLine(S(0), E(0), S(1));

		S = B;

		for (; S(1) < C(1); S(1)++, E(1)++, S(0) += dx3, E(0) += dx2)
			flatLine(S(0), E(0), S(1));
	}
}
}

