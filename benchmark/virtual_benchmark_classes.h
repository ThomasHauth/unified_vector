#pragma once

#include <memory>

#include "measurement.h"

namespace virtualized {

class HitBase {
public:
	explicit HitBase( float energyDeposit ) : m_energyDeposit(energyDeposit) {
	}

	virtual ~HitBase() = default;

	float getEnergyDeposit() const {
		return m_energyDeposit;
	}

	virtual Measurement createMeasurement() const = 0;
	
private:
	const float m_energyDeposit = 0.0f;
};

class SiliconPixelHit final: public HitBase {
public:
	SiliconPixelHit( float u_position, float v_position, float energyDeposit) :
		m_u_position(u_position), m_v_position(v_position), HitBase(energyDeposit) {};

	void dumpContent() {
		//std::cout << "This is class A" << std::endl;
	}

	float getEnergy() const {
		return 23.0f;
	}

	Measurement createMeasurement() const override final;

private:
	const float m_u_position;
	const float m_v_position;
};

class SiliconStripHit final: public HitBase {
public:
	enum class HitType {
		UHit,
		VHit
	};
	float getEnergy() const {
		return 23.0f;
	}

	Measurement createMeasurement() const override final;

	static std::unique_ptr< SiliconStripHit > createUHit( float vPosition, float energyDeposit)  {
		return std::make_unique< SiliconStripHit>( HitType::UHit, vPosition, energyDeposit);
	}

	static std::unique_ptr< SiliconStripHit > createVHit( float uPosition, float energyDeposit)  {
		return std::make_unique<SiliconStripHit>( HitType::VHit, uPosition, energyDeposit);
	}

//private:
	SiliconStripHit (HitType hitType, float position, float energyDeposit) : m_hitType(hitType), 
		m_position(position),
		HitBase(energyDeposit) {
	
	}

	const HitType m_hitType;
	const float m_position;
};

class DriftChamberHit final: public HitBase {
public:
	DriftChamberHit( float driftTime, float energyDeposit, float xLocation, float yLocation) :
		m_driftTime(driftTime), m_x_wireLocation(xLocation), m_y_wireLocation(yLocation), HitBase(energyDeposit)  
	{
	}

	float getEnergy() const {
		return 23.0f;
	}

	Measurement createMeasurement() const override final;
private:
	const float m_driftTime;
	// a wire has no z position;
	const float m_x_wireLocation;
	const float m_y_wireLocation;
};

}

