#include <iostream>  // can remove when debug log impl

// C headers
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "SmBios.h"

using namespace std;

void perror_(string msg) 
{
	perror(msg.c_str());
}

// from dmidecode util.c
int checksum(const u8 *buf, size_t len)
{
	u8 sum = 0;
	size_t a;

	for (a = 0; a < len; a++)
		sum += buf[a];
	return (sum == 0);
}

// TODO move to a global debug function
void SmBios::log(string msg)
{
	cout << msg << endl;
};

SmBios::SmBios()
{
};

bool SmBios::decode() 
{
	u8 *buf;
	if ((buf = getNonEfiEntryPoint()) == NULL)  {
		log("failed to find non-EFI entry point");
		return false;
	}
	if (!parseEntryPoint(buf)) {
		log("failed to parse entry point");
		return false;
	}
};

// this code (c) Alan Cox, Jean Delvare
// allocates memory
u8 *SmBios::getNonEfiEntryPoint()
{
	printf("%s: path: %s\n", __func__, path.c_str()); 
	int fd = open(path.c_str(), O_RDONLY);
	if (fd == -1) {
		perror_(path);
		return NULL;
	}
	
	// map 64K memory into process virtual space
	const size_t base = 0xF0000; // smbios 2.7.1, sec. 5.2.1
	const size_t offset = base % sysconf(_SC_PAGESIZE);
	const size_t len = 0x10000;

	void *m = mmap(NULL, offset + len, PROT_READ, MAP_SHARED, fd, base - offset);
	if (m == MAP_FAILED) {
		perror_("mmap");
		return NULL;
	}

	// copy mapped memory into our own buffer
	u8 *buf = (u8 *)malloc(len);
	if (buf == NULL) {
		perror_("malloc");
		return NULL;
	}
	memcpy(buf, (u8 *)m + offset, len);
	if (munmap(m, offset + len) == -1) {
		perror_("munmap");
	}

	// find the entry point
	bool found = false;
	u8 *entry;
	for (size_t fp = 0; fp < 0xFFF0; fp += 16) {
		if (memcmp((unsigned char *)buf + fp, "_SM_", 4) == 0 
		    && fp <= 0xFFE0) {
			entry = buf + fp;
			found = true;
		}
	}
	return found ? entry : NULL;
};

bool SmBios::parseEntryPoint(u8 *ep) 
{
	printf("entry: 0x%08X\n", ep);

	size_t eplen = ep[0x05];
	if (eplen < 0x1f) return false;
	if (!checksum(ep, ep[0x05])) return false;

	majorVer = ep[0x06];
	minorVer = ep[0x07];
	tableLen = WORD(ep+0x16);
	tablePtr = DWORD(ep+0x18);
	printf("version: %u.%u\n", majorVer, minorVer);
	printf("len: %u\n", tableLen);
	printf("add: 0x%08X\n", tablePtr);
	return true;
};
