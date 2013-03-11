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
	BOOST_REQUIRE_EQUAL(smbios->source(), "hp-dl380-g8.dmi");
	struct SmBiosInfo sminfo;
	smbios->get(sminfo);
	BOOST_REQUIRE_EQUAL(sminfo.version, "2.7");
	struct BiosInfo binfo;
	smbios->get(binfo);
	BOOST_REQUIRE_EQUAL(binfo.vendor, "HP");
	delete smbios;
}

BOOST_AUTO_TEST_CASE(bios_information)
{
	SmBios *smbios = new SmBiosBinary("hp-dl380-g8.dmi");
	struct BiosInfo binfo;
	smbios->get(binfo);
	BOOST_REQUIRE_EQUAL(binfo.vendor, "HP");
	BOOST_REQUIRE_EQUAL(binfo.version, "P70");
	BOOST_REQUIRE_EQUAL(binfo.releaseDate, "08/20/2012");
	delete smbios;
}

BOOST_AUTO_TEST_CASE(system_information)
{
	SmBios *smbios = new SmBiosBinary("hp-dl380-g8.dmi");
	struct SystemInfo sinfo;
	smbios->get(sinfo);
	BOOST_REQUIRE_EQUAL(sinfo.manufacturer, "HP");
	BOOST_REQUIRE_EQUAL(sinfo.product, "ProLiant DL380p Gen8");
	BOOST_REQUIRE_EQUAL(sinfo.version, "Not Specified");
	BOOST_REQUIRE_EQUAL(sinfo.serial, "USE249NWXB");
	delete smbios;
}


BOOST_AUTO_TEST_SUITE_END()
