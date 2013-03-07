#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE SmBiosTest
#include <boost/test/included/unit_test.hpp>

#include <string>
#include "SmBiosBinary.h"

using namespace std;

BOOST_AUTO_TEST_SUITE(BinaryFileReader)

BOOST_AUTO_TEST_CASE(SimpleRead)
{
	SmBios *smbios = new SmBiosBinary("hp-dl380-g8.dmi");
		
	struct BiosInfo binfo;
	smbios->get(binfo);
	BOOST_REQUIRE_EQUAL(binfo.vendor, "quicktest");
}

BOOST_AUTO_TEST_SUITE_END()
