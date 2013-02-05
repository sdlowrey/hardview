#include <iostream>  // can remove when debug log impl
#include <vector>

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
u8 * copymem(const size_t base, const size_t len, const string path)
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
void log(string msg)
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
	return true;
};

// allocates memory
bool SmBios::parseEfiEntryPoint() 
{
	// SMBios Spec 2.7, sec 5.2.1
	const size_t base = 0xF0000;
	const size_t len = 0x10000;
	u8 *membuf = (u8 *)copymem(base, len, path);
	if (membuf == NULL) return false;

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
		free(membuf);
		return false;
	}

	size_t eplen = ep[0x05];
	if (eplen < 0x1f || !checksum(ep, ep[0x05])
	    || memcmp("_DMI_", ep + 0x10, 5) != 0
	    || !checksum(ep + 0x10, 0x0f)) {
		log ("Invalid SMBios entry point structure");
		free(membuf);
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

	free (membuf);
	return true;
};

// Responsibility: Extract all the packed structs and return a vector
// of "elements".
//
// Rationale: The packed SMBios structures are unwieldy for high-level
// programming languages.  Also, we don't care about every single data
// element in every structure.  Therefore, boil each one down to an
// object that is easy to use and contains only what we need. 
//
// Elements are dynamically allocated because we don't know which
// elements -- or how many of each -- are there.  A vector works well
// as a container.
//
// Drawbacks: vector requires iteration to find elements of a certain
// (sub) type.

u8 * advance(u8 *p, u8 len) 
{
	p += len;
	// walk to double null + 1;
	printf("advancing to next struct");
	while (! (WORD(p) == 0x0000)) {
		printf(".");
		p++;
	}
	printf("\n");
	return p+2;
}

//allocates memory and frees it
bool SmBios::parseTable()
{
	u8 *buf = copymem(tablePtr, tableLen, path);
	
	u8 *p = buf;
	SmElementFactory elementFactory;
	SmElement *element;

	for (int i = 0; i < nStructs; ++i) {
		u8 len = p[1];
		element = elementFactory.create(p);
		// skip unsupported elements (null ptr)
		if (element != nullptr) {
			elem.push_back(*element);
		}
		p = advance(p, len);
		// validate the pointer based on tableLen?
	}
	free(buf);
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
