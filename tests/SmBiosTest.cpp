#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE SmBiosTest
#include <boost/test/included/unit_test.hpp>

#include <string>
#include "SmBiosBinary.h"

using namespace std;

BOOST_AUTO_TEST_SUITE(BinaryFileReader)

BOOST_AUTO_TEST_CASE(basicSanity)
{
	SmBios *smbios = new SmBiosBinary("hp-dl380-g8.dmi");
	struct SmBiosInfo sminfo;
	smbios->get(sminfo);
	BOOST_REQUIRE_EQUAL(sminfo.version, "2.6");
	delete smbios;
}

BOOST_AUTO_TEST_CASE(smBiosGetter)
{
	SmBios *smbios = new SmBiosBinary("hp-dl380-g8.dmi");
	struct BiosInfo binfo;
	smbios->get(binfo);
	BOOST_REQUIRE_EQUAL(binfo.vendor, "quicktest");
}

BOOST_AUTO_TEST_SUITE_END()
