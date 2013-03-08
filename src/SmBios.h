#pragma once
#include <stdexcept>
#include <string>
#include <vector>

struct SmBiosInfo {
	std::string desc = "SMBIOS Entry Point";
	std::string version;
};

struct BiosInfo {
	std::string desc = "BIOS";
	std::string vendor;
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
	SmBios(const std::string f);
	virtual ~SmBios() = default;

	virtual void getTable() {};
	virtual void get(BiosInfo& b) {};
	virtual void get(SmBiosInfo& s) {};

	std::string source();

protected:
	std::string infile;
};

