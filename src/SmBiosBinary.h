#include "SmBios.h"

// comment shows smbios spec equivalent
typedef unsigned char u8;	// BYTE
typedef unsigned short u16;	// WORD
typedef unsigned int u32;	// DWORD
typedef unsigned long u64;	// QWORD

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

class SmBiosBinary : public SmBios
{
public:
	// Locate BIOS information immediately in system memory
	SmBiosBinary() = default;

	// Locate BIOS information in a dmidecode binary dump file
	SmBiosBinary(std::string f);

	~SmBiosBinary() noexcept(true);
	
	void getTable() override;
	void get(BiosInfo& b);
	void get(SmBiosInfo& s);

private:
	u8 *mapToProcess(const size_t b, const size_t l, const std::string p);
	u8 *findTableEntryPoint(u8 *b);
	void processEntryPoint(u8 *b);
	u8 *findStructure(u8 t);
	u8 *findFirstStruct(SmBiosBaseStruct &b);
	std::string getString(u8 *s, u8 n);
	u8 *nextStruct(u8 *p);
	int checksum(const u8 *b, size_t l);

	// SMBIOS Table Entry Point info
	u16 maxSize;
	u16 tableLen;
	u32 tablePtr;
	u16 nStructs;
	u8 *table = nullptr;
	u8 specMajorVer;
	u8 specMinorVer;
};
