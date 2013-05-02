#pragma once
#include <map>
#include <stdexcept>
#include <string>
#include <vector>

enum StructType { 
	Bios, 
	System, 
	Enclosure,
};

// SMBIOS structures are translated into key-value containers...
typedef std::map<std::string, std::string> StructMap;

// Many structure types can appear more than once...
typedef std::vector<StructMap> StructVector;

// Slot each structure vector into a map; the key is the type.
typedef std::map<StructType, StructVector> SmBiosMap;

struct SmBiosInfo 
{
	const std::string desc = "SMBIOS Entry Point";
	std::string version;
};

struct SmBiosBaseStruct {
	SmBiosBaseStruct(std::string d, int t) : desc(d), type(t) {}
	const std::string desc;
	const int type;
};

struct BiosInfo : public SmBiosBaseStruct
{
	BiosInfo() : SmBiosBaseStruct("BIOS Information", 0) {}

	std::string vendor, version, releaseDate;
};

struct SystemInfo : public SmBiosBaseStruct
{
	SystemInfo() : SmBiosBaseStruct("System Information", 1) {}

	std::string manufacturer, product, version, serial;
};

struct Enclosure : public SmBiosBaseStruct
{
	Enclosure() : SmBiosBaseStruct("System Enclosure", 3) {}
	std::string manufacturer, type, version, serial, asset, height;
};

struct Processor : public SmBiosBaseStruct
{
	Processor() : SmBiosBaseStruct("Processor Information", 4) {}
	std::string manufacturer, family, version;
	std::string maxSpd, currentSpd, cores, coresEnabled;
	bool is64bit;
};


/**
 *  @class SmBios SmBios.h
 *  @brief Provide access to System Management BIOS (SMBIOS)
 *  structures.
 *
 *  This class is responsible for opening and parsing a file
 *  containing SMBios data structures.  It does not verify that the
 *  file exists or that the user is authorized to read it.  Path
 *  access is tested in the constructor.  If it fails, an exception is
 *  thrown. 
 *
 *  An SmBios object can be constructed from three types of sources:
 *  system memory, a dmidecode dump file, and dmidecode text output.
 */

class SmBios 
{
public:
	// Base constructor.  Sets infile to /dev/mem.
	SmBios(const std::string f) : infile(f) {};
	virtual ~SmBios() = default;

	std::string source();
	std::string dumpText();
	SmBiosInfo smbInfo;
	BiosInfo biosInfo;
	StructMap getNode(StructType t);
	virtual SmBiosMap getAllStructs();

protected:
	std::string infile;
	SmBiosMap smbios;

private:
	virtual void getTable() {};
	virtual void getSmBiosInfo() {};
	virtual void getBiosInfo() {};
	virtual void getSystemInfo() {};
};

