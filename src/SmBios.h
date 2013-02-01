#pragma once
#include <string>
#include <vector>
#include "SmElement.h"
#include "types.h"

void log(std::string);


/**
 *  @class SmBios SmBios.h
 *  @brief Parse System Management BIOS (SMBIOS) structures.
 */
class SmBios 
{
public:
	SmBios();
	SmBios(std::string p) : path(p) {};

	bool decode();
	const std::string getVersion();

private:
	u8 *getNonEfiEntryPoint();
	bool parseEfiEntryPoint();
	bool parseTable();

	const std::string path;
	
	u8 majorVer;
	u8 minorVer;
	u16 maxSize;
	u16 tableLen;
	u32 tablePtr;
	u16 nStructs;
	std::vector<SmElement> elem;

};
