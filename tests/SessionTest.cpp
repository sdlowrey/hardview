#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE SessionTest

#include <boost/test/included/unit_test.hpp>
#include <boost/test/output_test_stream.hpp>
#include "Session.hpp"

using boost::test_tools::output_test_stream;
using namespace std;

BOOST_AUTO_TEST_CASE(sanity_test)
{
	output_test_stream out;
	Session s;
	BOOST_CHECK( !s.isInProgress() );
	s.begin();
	BOOST_CHECK( s.isInProgress() );
	s.end();
	
}
