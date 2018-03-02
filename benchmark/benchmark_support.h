#pragma once

#include <random>

#include "virtual_benchmark_classes.h"
#include "unified_benchmark_classes.h"

#include <unified_vector.h>
#include <vector>
#include <array>
#include <chrono>
#include <memory>
#include <iostream>
#include <fstream>
#include <algorithm>

#include <boost/lexical_cast.hpp>
#include <boost/range/irange.hpp>

#include <map>

class benchmark_logger {
public:
	void push_quantity(std::string name, float value) {
		m_log[name].push_back(value);
	}

	void store(std::string fileName) const {
		std::ofstream file(fileName, std::ofstream::trunc);

		std::size_t largest_entry = 0;
		for (auto map_entry : m_log) {
			file << map_entry.first << ",";
			largest_entry = std::max(largest_entry, map_entry.second.size());
		}
		file << std::endl;

		for (auto i : boost::irange(std::size_t(0), largest_entry)) {
			for (auto map_entry : m_log) {
				if (map_entry.second.size() > i) {
					file << map_entry.second[i] << ",";
				} else {
					file << map_entry.second[i] << ",";
				}
			}
			file << std::endl;
		}
	}

private:
	std::map<std::string, std::vector<float>> m_log;
};

class benchmark_timer {
public:

	benchmark_timer(std::string actionName, benchmark_logger & logger) :
			m_actionName(actionName), m_logger(logger) {
		m_startPoint = std::chrono::system_clock::now();
	}

	~benchmark_timer() {
		auto end = std::chrono::system_clock::now();
		auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(
				end - m_startPoint);

		m_logger.push_quantity(m_actionName, elapsed.count());
	}

	void start() {
	}

	void stop() {
		auto end = std::chrono::system_clock::now();
		auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(
				end - m_startPoint);
		m_times.push_back(double(elapsed.count()));
	}

	double avg() {
		m_times.erase(m_times.begin());
		m_times.pop_back();

		return std::accumulate(m_times.begin(), m_times.end(), 0.0)
				/ double(m_times.size());
	}

private:
	std::chrono::time_point<std::chrono::system_clock> m_startPoint;
	std::vector<double> m_times;
	std::string m_actionName;
	benchmark_logger & m_logger;
};

struct AppFillerData {

	std::vector<float> someArray;
};

class benchmark_support {
public:

	benchmark_support(int argc, char* argv[],
			std::size_t avgContainerEntrySize /*in bytes*/) :
			uniform_dist_union(-1.0, 1.0), uniform_dist_zero_to_one(0.0, 1.0), uniform_dist_drift_time(
					0.01, 10.0), uniform_dist_wire_location(-50.0, 50.0), normal_dist(
					5.0, 0.8), uniform_dist_int(1, m_appDataAverageEntries * 2) {
		if (argc < 5) {
			std::cout
					<< "Usage: executable <container size [MB]> <other app data size [MB]> <churn fraction> <iterations>"
					<< std::endl;
			exit(1);
		}

		m_containerMemory = boost::lexical_cast<std::size_t>(argv[1]);
		m_containerEntries = std::size_t(
				(double(m_containerMemory) * 1e6)
						/ double(avgContainerEntrySize));

		m_appDataMemory = boost::lexical_cast<float>(argv[2]);
		const std::size_t avgAppDataSize = m_appDataAverageEntries
				* sizeof(float);
		m_appDataEntries = std::size_t(
				(double(m_appDataMemory) * 1e6) / double(avgAppDataSize));

		m_churnFraction = boost::lexical_cast<float>(argv[3]);

		if ((m_churnFraction > 1.0) or (m_churnFraction < 0.0)) {
			std::cout << "Churn fraction must be > 0 and =< 1" << std::endl;
			exit(1);
		}

		m_iterations = boost::lexical_cast<std::size_t>(argv[4]);

		std::cout << "Using " << m_containerMemory
				<< " (MB) Container Memory with " << m_containerEntries
				<< " entries" << std::endl;
		std::cout << "Using " << m_appDataMemory
				<< " (MB) artifical App Memory with " << m_appDataEntries
				<< " entries" << std::endl;
	}

	void generateAppData() {
		// fill up the other useless app data
		while (m_uselessDump.size() < m_appDataEntries) {
			auto appFiller = std::make_unique<AppFillerData>();
			for (int i = 0; i < uniform_dist_int(generator); i++) {
				appFiller->someArray.push_back(normal_dist(generator));
			}
			m_uselessDump.push_back(std::move(appFiller));
		}
	}

	void clearAddData() {
		m_uselessDump.clear();
	}

	void churnAppData() {
		std::random_shuffle(m_uselessDump.begin(), m_uselessDump.end());
		// remove some entries

		std::uniform_int_distribution<int> toremove_dist_int(0,
				std::floor<int>(float(m_uselessDump.size()) * m_churnFraction));

		const auto entriesToRemove = toremove_dist_int(generator);

		for (auto i : boost::irange(0, entriesToRemove)) {
			const auto entryToRemove = std::uniform_int_distribution<int>(0,
					m_uselessDump.size() - 1)(generator);

			m_uselessDump.erase(
					m_uselessDump.begin() + std::size_t(entryToRemove));
		}

		// fill up again
		generateAppData();
	}

	std::size_t getIterations() const {
		return m_iterations;
	}

	template < class TContainerType, class TFuncFillContainer, class TFuncCreateMeasurements >
	void runBenchmark ( TContainerType & container, TFuncFillContainer funcFillContainer,
			TFuncCreateMeasurements funcCreateMeasurements, std::string resultsFileName)
	{
		benchmark_logger logger;
		for (auto i : boost::irange(std::size_t(0), getIterations())) {
			// filling uv vector
			{
				benchmark_timer btimer("fill_vector", logger);
				//bsupport.fillUnifiedVector(uv);
				funcFillContainer(container, m_containerEntries );
			}

			// simulate some other work of the app
			churnAppData();

			std::vector<Measurement> measurements(container.size());
			// creating measurements
			{
				benchmark_timer btimer("visit_vector", logger);
				//uv.visit(
				//		[&measurements](auto & obj ) -> void {measurements.emplace_back(obj.createMeasurement());});
				funcCreateMeasurements( container, measurements );
			}
			container.clear();
		}

		logger.store(resultsFileName);

	}

	float getSiliconHitLocation() {
		return uniform_dist_union(generator);
	}

	float getHitEnergy() {
		return std::abs(normal_dist(generator));
	}

	float getWireHitLocation() {
		return uniform_dist_wire_location(generator);
	}

	float getWireHitDriftTime() {
		return uniform_dist_drift_time(generator);
	}

private:
	std::default_random_engine generator;
	std::uniform_real_distribution<float> uniform_dist_union;
	std::uniform_real_distribution<float> uniform_dist_zero_to_one;
	std::uniform_real_distribution<float> uniform_dist_drift_time;
	std::uniform_real_distribution<float> uniform_dist_wire_location;
	std::normal_distribution<float> normal_dist;
	std::uniform_int_distribution<int> uniform_dist_int;

	static const std::size_t m_appDataAverageEntries = 1000;
	std::size_t m_containerMemory;
	std::size_t m_containerEntries;

	std::size_t m_appDataMemory;
	std::size_t m_appDataEntries;
	std::size_t m_iterations;
	float m_churnFraction;

	std::vector<std::unique_ptr<AppFillerData> > m_uselessDump;
};
