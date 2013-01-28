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

// based on dmidecode mem_chunk
void * copymem(const size_t base, const size_t len, const string path)
{
	int fd = open(path.c_str(), O_RDONLY);
	if (fd == -1) {
		perror_(path);
		return NULL;
	}
	
	const size_t offset = base % sysconf(_SC_PAGESIZE);

	void *m = mmap(NULL, offset + len, PROT_READ, MAP_SHARED, 
		       fd, base - offset);
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
	close(fd);
	return buf;
};

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
	if (!parseEfiEntryPoint()) {
		log("failed to parse entry point");
		return false;
	}
	if (!parseTable()) {
		log("failed to parse table");
		return false;
	};
};

bool SmBios::parseEfiEntryPoint() 
{
	// SMBios Spec - 5.2.1
	const size_t base = 0xF0000;
	const size_t len = 0x10000;
	u8 *membuf = (u8 *)copymem(base, len, path);

	// find the entry point
	bool found = false;
	u8 *ep;
	for (size_t fp = 0; fp < 0xFFF0; fp += 16) {
		if (memcmp((unsigned char *)membuf + fp, "_SM_", 4) == 0 
		    && fp <= 0xFFE0) {
			ep = membuf + fp;
			found = true;
		}
	}
	if (!found) {
		log("Failed to find SMBios entry point");
		return false;
	}

	size_t eplen = ep[0x05];
	if (eplen < 0x1f || !checksum(ep, ep[0x05])
	    || memcmp("_DMI_", ep + 0x10, 5) != 0
	    || !checksum(ep + 0x10, 0x0f)) {
		log ("Invalid SMBios entry point structure");
		return false;
	}

	majorVer = ep[0x06];
	minorVer = ep[0x07];
	maxSize = WORD(ep + 0x08);
	tableLen = WORD(ep + 0x16);
	tablePtr = DWORD(ep + 0x18);
	nStructs = WORD(ep + 0x1c);

	printf("SMBios %u.%u\nStructures: %d  Max Size: %d bytes\nTable is %d bytes starting at 0x%08X\n", 
	       majorVer, minorVer, nStructs, maxSize, tableLen, tablePtr);

	parseTable();
	return true;
};

bool SmBios::parseTable()
{
	log("not implemented");
	return true;
};

const string SmBios::getVersion()
{
	size_t maxlen = 6;
	char s[maxlen];
	if (snprintf(s, maxlen, "%u.%u", majorVer, minorVer) >= maxlen) {
		log("warning: version truncated");
	}
	return string(s);
};
