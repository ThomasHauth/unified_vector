#include "virtual_benchmark_classes.h"

using namespace virtualized;

Measurement SiliconPixelHit::createMeasurement() const {
	return Measurement( m_u_position, m_v_position, m_v_position *0.5 );  
}

Measurement SiliconStripHit::createMeasurement() const {
	return Measurement(0,2,4);
}

Measurement DriftChamberHit::createMeasurement() const {
	return Measurement(0,2,4);
}

