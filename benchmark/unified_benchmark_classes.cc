#include "unified_benchmark_classes.h"

using namespace unified;

Measurement SiliconPixelHit::createMeasurement() const {
	return Measurement( m_u_position, m_v_position, m_v_position *0.5 );  
}

Measurement SiliconStripHit::createMeasurement() const {
	return Measurement( m_position, m_energyDeposit, m_position *0.7);
}

Measurement DriftChamberHit::createMeasurement() const {
	return Measurement(m_x_wireLocation,m_y_wireLocation,m_energyDeposit);
}

