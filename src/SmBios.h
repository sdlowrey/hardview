#pragma once
#include <string>

// comment shows smbios spec equivalent
typedef unsigned char u8;	// byte
typedef unsigned short u16;	// word
typedef unsigned int u32;	// dword
typedef unsigned long u64;

/**
 *  @class SmBios SmBios.h
 *  @brief Parse System Management BIOS (SMBIOS) structures.
 */
class SmBios {
public:
	SmBios();
	SmBios(std::string p) : path(p) {};
	bool decode();
private:
	void log(std::string);
	u8 *getNonEfiEntryPoint();
	u8 *getEfiEntryPoint() {};
	void parseEntryPoint(u8 *ep);

	const std::string path;
	u8 majorVer;
	u8 minorVer;
};
