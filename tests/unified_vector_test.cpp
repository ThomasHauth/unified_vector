#define BOOST_TEST_MODULE test module name
#include <boost/test/included/unit_test.hpp>

namespace utf = boost::unit_test;

#include <unified_vector.h>

class TestClassA {
public:
	int value = 23;
};

class TestClassB {
public:
	float value = 5.5;
};

BOOST_AUTO_TEST_CASE( unified_vector_push_back, * utf::tolerance(0.0001) ) {
	unified_vector<TestClassA, TestClassB> uvec;

	uvec.push_back(TestClassA());
	uvec.push_back(TestClassB());

	BOOST_TEST(uvec.size() == 2);

	uvec.clear();
	BOOST_TEST(uvec.size() == 0);

	uvec.push_back(TestClassA());
	uvec.push_back(TestClassB());
	uvec.push_back(TestClassB());
	BOOST_TEST(uvec.size() == 3);

	// compute sum of entries
	float sum = 0.0f;
	uvec.visit([&sum]( auto const& e ) {sum+=e.value;});
	BOOST_TEST(sum == 34);
}

