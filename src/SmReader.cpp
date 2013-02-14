#include <stdexcept>
#include <string>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "SmReader.h"

using namespace std;

// based on dmidecode mem_chunk: map file, copy to our buffer
// can be moved to generic utility lib
u8 * copymem(const size_t base, const size_t len, const string path)
{
	int fd = open(path.c_str(), O_RDONLY);
	if (fd == -1) {
		throw runtime_error("unable to open " + path);
	}
	
	const size_t offset = base % sysconf(_SC_PAGESIZE);

	void *m = mmap(NULL, offset + len, PROT_READ, MAP_SHARED, 
		       fd, base - offset);
	if (m == MAP_FAILED) {
		throw runtime_error("unable to map file to memory");
	}

	u8 *buf = (u8 *)malloc(len);
	if (buf == NULL) {
		throw runtime_error("unable to allocate memory");
	}
	memcpy(buf, (u8 *)m + offset, len);

	munmap(m, offset + len);  // don't care if this fails?
	close(fd);
	return buf;
};

u8 * advance(u8 *p, u8 len) 
{
	p += len;
	// walk to double null + 1;
	while (! (WORD(p) == 0x0000)) {
		p++;
	}
	return p+2;
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

SmReader::~SmReader()
{
	cout << "SmReader destructor" << endl;
}

SmBinaryReader::SmBinaryReader(string p)
{
	// verify path is accessible
	path = p;
	try {
		processEntryPoint();
	}
	catch (runtime_error err) {
		throw;
	}

	table = copymem(tablePtr, tableLen, path);
};

SmBinaryReader::~SmBinaryReader()
{
	free(table);
};

vector<SmElement> SmBinaryReader::getAllElements()
{
	u8 *p = table;
	SmElementFactory elementFactory;
	SmElement *element;
	vector<SmElement> elements;

	for (int i = 0; i < nStructs; ++i) {
		u8 len = p[1];
		element = elementFactory.create(p);
		// skip unsupported elements (null ptr)
		if (element != nullptr) {
			elements.push_back(*element);
		}
		p = advance(p, len);
		// validate the pointer based on tableLen?
	}
	return elements;
};

SmElement *SmBinaryReader::getElement(u8 t)
{
};

std::string SmReader::getVersion()
{
	size_t maxlen = 6;
	char s[maxlen];
	if (snprintf(s, maxlen, "%u.%u", majorVer, minorVer) >= maxlen) {
		throw runtime_error("SMBios version truncated");
	}
	return string(s);
};

std::string SmBinaryReader::getVersion()
{
	return SmReader::getVersion();
};

std::string SmBinaryReader::getTableInfo()
{
	return "plonk";
};

inline bool SmBinaryReader::isValidType(u8 t)
{
// per spec 2.7... TODO vary based on version of this smbios
	if ((t < 0) || (t > 127)) {
		return false;
	}
	if ((t > 42) && (t < 126)) {
		return false;
	}
	return true;
};

// allocates and frees memory, even if exception thrown.
void SmBinaryReader::processEntryPoint()
{
	// SMBios Spec 2.7, sec 5.2.1
	const size_t base = 0xF0000;
	const size_t len = 0x10000;

	// this can throw; let it
	u8 *membuf = (u8 *)copymem(base, len, path);

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
		free(membuf);
		throw runtime_error("failed to locate SMBIOS entry point");
	}

	// validate ep struct: correct length, checksums, string anchors 
	size_t eplen = ep[0x05];
	if (eplen < 0x1f || !checksum(ep, ep[0x05])
	    || memcmp("_DMI_", ep + 0x10, 5) != 0
	    || !checksum(ep + 0x10, 0x0f)) {
		free(membuf);
		// not necessarily a reason to quit but...
		throw runtime_error("invalid entry point structure");
	}

	maxSize = WORD(ep + 0x08);
	tableLen = WORD(ep + 0x16);
	tablePtr = DWORD(ep + 0x18);
	nStructs = WORD(ep + 0x1c);
	free(membuf);
};


// non-member functions

