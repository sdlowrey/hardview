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

typedef struct __attribute__ ((packed))
{
	u8 anchorStr[4];
	u8 checksum;
	u8 length;
	u8 versionMajor;
	u8 versionMinor;
	u16 maxStructSize;
	u8 epRevision;
	u8 formattedArea[5];
	u8 intermediateAnchor[5];
	u8 intermediateChecksum;
	u16 tableLength;
	u32 tableAddr;
	u16 numStructs;
	u8 bcdRevision;
}  TableEntryPoint;

class SmBiosBinary : public SmBios
{
public:
	// Locate BIOS information immediately in system memory
	SmBiosBinary() = default;

	// Locate BIOS information in a dmidecode binary dump file
	SmBiosBinary(std::string f);

	~SmBiosBinary() noexcept(true);
	
private:
	void getTable() override;
	void getSmBiosInfo();
	void getBiosInfo();
	void getSystemInfo();

	u8 *mapToProcess(const size_t b, const size_t l, const std::string p);
	u8 *findTableEntryPoint(u8 *b);
	void processEntryPoint(void);
	u8 *findStructure(u8 t);
	u8 *findFirstStruct(SmBiosBaseStruct &b);
	std::string getString(u8 *s, u8 n);
	u8 *nextStruct(u8 *p);
	int checksum(const u8 *b, size_t l);

	TableEntryPoint tep;

	u8 *table = nullptr;
};
