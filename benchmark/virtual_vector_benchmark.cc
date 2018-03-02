#include <unified_vector.h>

#include <functional>
#include <iostream>
#include <string>
#include <chrono>
#include <random>
#include <memory>
#include <algorithm>
#include <iostream>
#include <fstream>

#include "benchmark_support.h"

int main(int argc, char* argv[]) {

	std::vector<std::unique_ptr<virtualized::HitBase>> uv;

	const std::size_t avgContainerEntrySize = float(
			sizeof(virtualized::SiliconPixelHit)
					+ sizeof(virtualized::SiliconStripHit)
					+ sizeof(virtualized::DriftChamberHit)) / 3.0;

	benchmark_support bsupport(argc, argv, avgContainerEntrySize);

	bsupport.runBenchmark(
			uv, //
			[&bsupport]( auto & virtualizedVector, std::size_t containerEntries ) {

				for ( std::size_t i = 0; i < (containerEntries/3); i++) {
					virtualizedVector.emplace_back(std::make_unique<virtualized::SiliconPixelHit>(bsupport.getSiliconHitLocation(),
									bsupport.getSiliconHitLocation(),
									bsupport.getHitEnergy()));
				}
				for ( std::size_t i = 0; i < (containerEntries/3); i++) {
					if (i % 2) {
						virtualizedVector.emplace_back(virtualized::SiliconStripHit::createUHit(bsupport.getSiliconHitLocation(),
										bsupport.getHitEnergy()));
					} else {
						virtualizedVector.emplace_back(virtualized::SiliconStripHit::createVHit(bsupport.getSiliconHitLocation(),
										bsupport.getHitEnergy()));
					}
				}
				for ( std::size_t i = 0; i < (containerEntries/3); i++) {
					virtualizedVector.emplace_back(std::make_unique<virtualized::DriftChamberHit>(bsupport.getWireHitDriftTime(),
									bsupport.getHitEnergy(),
									bsupport.getWireHitLocation(),
									bsupport.getWireHitLocation() ));
				}
			},
			[]( auto & virtualizedVector, std::vector<Measurement> & measurements ) {

				for (auto & hit : virtualizedVector) {
					measurements.emplace_back( hit->createMeasurement());
				}
			}, "virtual_vector_results.txt");

	return 0;
}

