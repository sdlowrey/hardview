#pragma once
#include <stdexcept>
#include <string>
#include <vector>
#include "SmElement.h"
#include "types.h"

void log(std::string);

// need an enum of struct { int type, string name, bool required }

struct SmStructTypes {
	int type;
	std::string description;
	bool required;
};

const std::vector<SmStructTypes> smTypes = {
	{0,  "BIOS", true},
	{1,  "System", true},
	{2,  "Base Board", false},
	{3,  "Chassis", true},
	{4,  "Processor", true},
	{5,  "Memory Controller", false},
       	{6,  "Memory Module", false},
	{7,  "Cache", true},
	{8,  "Port Connector", false},
	{9,  "System Slots", true},
	{10, "On Board Devices", false},
	{11, "OEM Strings", false},
	{12, "System Configuration Options", false},
	{13, "BIOS Language", false},
	{14, "Group Associations", false},
	{15, "System Event Log", false},
	{16, "Physical Memory Array", true},
	{17, "Memory Device", true},
	{18, "32-bit Memory Error", false},
	{19, "Memory Array Mapped Address", true},
	{20, "Memory Device Mapped Address", false},
	{21, "Built-in Pointing Device", false},
	{22, "Portable Battery", false},
	{23, "System Reset", false},
	{24, "Hardware Security", false},
	{25, "System Power Controls", false},
	{26, "Voltage Probe", false},
	{27, "Cooling Device", false},
	{28, "Temperature Probe", false},
	{29, "Electrical Current Probe", false},
	{30, "Out-of-band Remote Access", false},
	{31, "Boot Integrity Services", false},
	{32, "System Boot", true},
	{33, "64-bit Memory Error", false},
	{34, "Management Device", false},
	{35, "Management Device Component", false},
	{36, "Management Device Threshold Data", false},
	{37, "Memory Channel", false},
	{38, "IPMI Device", false},
	{39, "Power Supply", false},
	{40, "Additional Information", false},
	{41, "Onboard Device", false},
	{42, "Management Controller Host Interface", false},
	{126, "Inactive", false},
	{127, "End Of Table", false},
};


/**
 *  @class SmBios SmBios.h
 *  @brief Provide access to System Management BIOS (SMBIOS) structures.
 */
class SmBios 
{
public:
	SmBios() {};
	SmBios(SmReader *);

	bool decodePath(std::string path);
	void dumpTypes();

	const std::string getVersion();
	SmElement getElement(u8 type);

private:
	SmReader *reader;
	SmElement *elements;

	u8 *getNonEfiEntryPoint();
	bool parseEfiEntryPoint();
	bool parseTable();
	bool isValidType(u8 t);

	std::string path;
	
	u8 majorVer;
	u8 minorVer;
	u16 maxSize;
	u16 tableLen;
	u32 tablePtr;
	u16 nStructs;
	std::vector<SmElement> elem;

};

