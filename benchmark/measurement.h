#pragma once

#include <limits>

class Measurement{
public:
	// required to resize a vector of Measurements
	Measurement() {}


	Measurement(float x, float y, float z) : m_x(x), m_y(y), m_z(z) {
	}

private:
	const float m_x = 0.0f;
	const float m_y = 0.0f;
	const float m_z = 0.0f;

	const float m_x_uncertainty = std::numeric_limits<float>::max();
};

