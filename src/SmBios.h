#pragma once
#include <string>

// comment shows smbios spec equivalent
typedef unsigned char u8;	// byte
typedef unsigned short u16;	// word
typedef unsigned int u32;	// dword
typedef unsigned long u64;


// This workaround is borrowed from dmidecode types.h.
// BIGENDIAN macro variants removed; add later if any platforms need it
#ifdef __ia64__
#define ALIGNMENT_WORKAROUND
#endif

// Use C++ explicit named casts for machines not requiring the workaround
// These are easier to read and understand than dmidecode C-style casts
#ifndef ALIGNMENT_WORKAROUND
#	define WORD(x) *reinterpret_cast<u16*>(x)
#	define DWORD(x) *reinterpret_cast<u32*>(x)
#	define QWORD(x) *reinterpret_cast<u64*>(x)
#else /* ALIGNMENT_WORKAROUND - use C-style casts*/
static inline u64 U64(u32 low, u32 high)
{
	u64 self;

	self.l = low;
	self.h = high;

	return self;
}
#	define WORD(x) (u16)((x)[0] + ((x)[1] << 8))
#	define DWORD(x) (u32)((x)[0] + ((x)[1] << 8) + ((x)[2] << 16) + ((x)[3] << 24))
#	define QWORD(x) (U64(DWORD(x), DWORD(x + 4)))
#endif /* ALIGNMENT_WORKAROUND */

/**
 *  @class SmBios SmBios.h
 *  @brief Parse System Management BIOS (SMBIOS) structures.
 */
class SmBios {
public:
	SmBios();
	SmBios(std::string p) : path(p) {};

	bool decode();
	const std::string getVersion();

private:
	void log(std::string);
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

};
