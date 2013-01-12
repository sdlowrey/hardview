#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE OsCollectorTest

#include <boost/test/included/unit_test.hpp>
#include <boost/test/output_test_stream.hpp>
#include "OsCollector.hpp"

using boost::test_tools::output_test_stream;
using namespace std;

BOOST_AUTO_TEST_CASE(sanity_test)
{
	output_test_stream out;
	OsCollector os;
	string k = os.getKernel();
	BOOST_CHECK( k == "3.6.10-2.fc17.x86_64" );
}
