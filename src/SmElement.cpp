#include "SmElement.h"

using namespace std
;
// from dmidecode.c
const string SmElement::strType()
{
	static const char *t[] = {
		"BIOS", /* 0 */
		"System",
		"Base Board",
		"Chassis",
		"Processor",
		"Memory Controller",
		"Memory Module",
		"Cache",
		"Port Connector",
		"System Slots",
		"On Board Devices",
		"OEM Strings",
		"System Configuration Options",
		"BIOS Language",
		"Group Associations",
		"System Event Log",
		"Physical Memory Array",
		"Memory Device",
		"32-bit Memory Error",
		"Memory Array Mapped Address",
		"Memory Device Mapped Address",
		"Built-in Pointing Device",
		"Portable Battery",
		"System Reset",
		"Hardware Security",
		"System Power Controls",
		"Voltage Probe",
		"Cooling Device",
		"Temperature Probe",
		"Electrical Current Probe",
		"Out-of-band Remote Access",
		"Boot Integrity Services",
		"System Boot",
		"64-bit Memory Error",
		"Management Device",
		"Management Device Component",
		"Management Device Threshold Data",
		"Memory Channel",
		"IPMI Device",
		"Power Supply",
		"Additional Information",
		"Onboard Device",
		"Management Controller Host Interface", /* 42 */
	};

	if (type <= 42)
		return string(t[type]);
	return "unknown";
};

// per spec 2.7... TODO vary based on version of this smbios
inline bool SmElementFactory::isValidType()
{
	if ((type < 0) || (type > 127)) {
		printf("type out of bounds\n");
		return false;
	}
	if ((type > 42) && (type < 126)) {
		printf("illegal type\n");
		return false;
	}
	return true;
};

SmElement *SmElementFactory::create(u8 *ptr)
{
	u8 type = ptr[0];
	if (! isValidType()) {
		printf("element create: invalid type\n");
		return nullptr;
	}
	switch (type) {
	case 0: //bios
		return new Bios(ptr);
		break;
	case 1: //system
		return new System(ptr);
		break;
	case 2: //baseboard
		return new Baseboard(ptr);
		break;
	default:
		printf("factory: unknown type %d\n", type);
		return nullptr;
	}
	return nullptr;  // only for testing
};
