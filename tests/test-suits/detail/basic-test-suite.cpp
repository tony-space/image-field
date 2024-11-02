#include <imf/core/core.hpp>

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(basics)

BOOST_AUTO_TEST_CASE(PlaceholderHashing)
{
	using namespace imf::core;

	auto v1 = PlaceholderNode::make_variable(42.0f);
	auto v2 = PlaceholderNode::make_variable(42.0f);
	auto c1 = PlaceholderNode::make_constant(42.0f);
	auto c2 = PlaceholderNode::make_constant(42.0f);

	const auto v1_hash = v1->hash();
	const auto v2_hash = v2->hash();
	const auto c1_hash = c1->hash();
	const auto c2_hash = c2->hash();

	// variables must not have the same hash even if they have the same value
	BOOST_TEST(v1->hash() != v2->hash());

	// constants must have the same hash when they hold the same value
	BOOST_TEST(c1->hash() == c2->hash());

	v1->setValue(1.0f);
	v2->setValue(1.0f);

	c1->setValue(1.0f);
	// constants must have different hashes when they hold different values
	BOOST_TEST(c1->hash() != c2->hash());
	c2->setValue(1.0f);

	// variables must not change their hash when their value changes
	BOOST_TEST(v1_hash == v1->hash());
	BOOST_TEST(v2_hash == v2->hash());

	// for constants hashes must change in the same way
	BOOST_TEST(c1->hash() == c2->hash());
	BOOST_TEST(c1_hash != c1->hash());
	BOOST_TEST(c2_hash != c2->hash());
}


BOOST_AUTO_TEST_SUITE_END()