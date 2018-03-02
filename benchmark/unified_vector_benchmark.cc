#include <unified_vector.h>

#include <functional>
#include <iostream>
#include <string>
#include <chrono>
#include <random>

#include "benchmark_support.h"

#include <boost/range/irange.hpp>

int main(int argc, char* argv[]) {

	unified_vector<unified::SiliconPixelHit, unified::SiliconStripHit,
			unified::DriftChamberHit> uv;

	const std::size_t avgContainerEntrySize = float(
			sizeof(unified::SiliconPixelHit) + sizeof(unified::SiliconStripHit)
					+ sizeof(unified::DriftChamberHit)) / 3.0;

	benchmark_support bsupport(argc, argv, avgContainerEntrySize);

	bsupport.runBenchmark(
			uv, //
			[&bsupport]( auto & unifiedVector, std::size_t containerEntries ) {
				for ( std::size_t i = 0; i < (containerEntries/3); i++) {
					unifiedVector.push_back(unified::SiliconPixelHit(bsupport.getSiliconHitLocation(),
									bsupport.getSiliconHitLocation(),
									bsupport.getHitEnergy()));
				}
				for ( std::size_t i = 0; i < (containerEntries/3); i++) {
					if (i % 2) {
						unifiedVector.push_back(unified::SiliconStripHit::createUHit(bsupport.getSiliconHitLocation(),
										bsupport.getHitEnergy()));
					} else {
						unifiedVector.push_back(unified::SiliconStripHit::createVHit(bsupport.getSiliconHitLocation(),
										bsupport.getHitEnergy()));

					}
				}
				for ( std::size_t i = 0; i < (containerEntries/3); i++) {
					unifiedVector.push_back(unified::DriftChamberHit(bsupport.getWireHitDriftTime(),
									bsupport.getHitEnergy(),
									bsupport.getWireHitLocation(),
									bsupport.getWireHitLocation()));
				}
			},
			[]( auto & unifiedVector, std::vector<Measurement> & measurements ) {
				// make sure this lambda actually gets compiled out
				unifiedVector.visit(
						[&measurements](auto & obj ) -> void {measurements.emplace_back(obj.createMeasurement());
						});
			}, "unified_vector_results.txt");

	return 0;
}

