#pragma once

#include "measurement.h"

namespace unified {

class SiliconPixelHit {
public:
	SiliconPixelHit( float u_position, float v_position, float energyDeposit) :
		m_u_position(u_position), m_v_position(v_position), m_energyDeposit(energyDeposit) {};

	void dumpContent() {
		//std::cout << "This is class A" << std::endl;
	}

	float getEnergy() const {
		return 23.0f;
	}

	Measurement createMeasurement() const;

private:
	const float m_u_position;
	const float m_v_position;
	const float m_energyDeposit;
};

class SiliconStripHit {
public:
	enum class HitType {
		UHit,
		VHit
	};

	void dumpContent() {
		//std::cout << "This is class B" << std::endl;
	}

	float getEnergy() const {
		return 23.0f;
	}

	Measurement createMeasurement() const;

	static SiliconStripHit createUHit( float vPosition, float energyDeposit)  {
		return SiliconStripHit( HitType::UHit, vPosition, energyDeposit);
	}

	static SiliconStripHit createVHit( float uPosition, float energyDeposit)  {
		return SiliconStripHit( HitType::VHit, uPosition, energyDeposit);
	}

private:
	SiliconStripHit (HitType hitType, float position, float energyDeposit) : m_hitType(hitType), 
		m_position(position),
		m_energyDeposit(energyDeposit) {
	
	}

	const HitType m_hitType;
	const float m_position;
	const float m_energyDeposit;
};

class DriftChamberHit {
public:
	DriftChamberHit( float driftTime, float energyDeposit, float xLocation, float yLocation) :
		m_driftTime(driftTime), m_energyDeposit(energyDeposit), m_x_wireLocation(xLocation), m_y_wireLocation(yLocation) 
	{
	}

	void dumpContent() {
		//std::cout << "This is class C" << std::endl;
	}

	float getEnergy() const {
		return 23.0f;
	}

	Measurement createMeasurement() const;
private:
	const float m_driftTime;
	const float m_energyDeposit;
	// a wire has no z position;
	const float m_x_wireLocation;
	const float m_y_wireLocation;
};

}

