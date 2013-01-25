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
	if ((buf = getNonEfiEntryPoint()) == NULL)  {
		log("failed to find non-EFI entry point");
		return false;
	}
	if (!parseEntryPoint()) {
		log("failed to parse entry point");
		return false;
	}
};

// this algorithm from dmidecode.c; allocates memory
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
	buf = (u8 *)malloc(len);
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

bool SmBios::parseEntryPoint() 
{
	size_t eplen = buf[0x05];
	if (eplen < 0x1f || !checksum(buf, buf[0x05])
	    || memcmp("_DMI_", buf + 0x10, 5) != 0
	    || !checksum(buf + 0x10, 0x0f)) {
		log ("Invalid SMBios entry point structure");
		return false;
	}
	majorVer = buf[0x06];
	minorVer = buf[0x07];
	tableLen = WORD(buf+0x16);
	tablePtr = DWORD(buf+0x18);
	nStructs = WORD(buf+0x1c);
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
