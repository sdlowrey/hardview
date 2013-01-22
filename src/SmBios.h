#pragma once
#include <string>

// comment shows smbios spec equivalent
typedef unsigned char u8;	// byte
typedef unsigned short u16;	// word
typedef unsigned int u32;	// dword
typedef unsigned long u64;

class SmBios {
public:
	SmBios();
	SmBios(std::string path);
private:
	void log(std::string);
	bool getNonEfiEntryPoint(std::string path);
	bool getEfiEntryPoint(std::string path);

	u8 *smbuf;
	u8 majorVer;
	u8 minorVer;
};
