#include <fcntl.h>
#include <stdexcept>
#include <string>
#include <sys/mman.h>
#include <sys/stat.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

#include "SmBiosBinary.h"

using namespace std;

void SmBiosBinary::getTable()
{
	size_t base, length;
	if (infile == "/dev/mem") {
		base = 0xF0000; // SMBios Spec 2.7, sec 5.2.1
		length = 0x10000;
	} else { // assume dmidecode binary dump file
		base = 0;  	// start of file
		length = 32;	// table entry point size
	}
	u8 *buf = mapToProcess(base, length, infile);
	u8 *ep = findTableEntryPoint(buf);
	try {
		processEntryPoint(ep);
	} catch (runtime_error e) {
		free(buf);
		throw e;
	}
	free(buf);
	table = mapToProcess(tablePtr, tableLen, infile);	
};

u8 *SmBiosBinary::findTableEntryPoint(u8 *buf)
{
	bool found = false;
	for (size_t fp = 0; fp < 0xFFF0; fp += 16) {
		if (memcmp((unsigned char *)buf + fp, "_SM_", 4) == 0 
		    && fp <= 0xFFE0) {
			return (buf + fp);
		}
	}
	throw runtime_error("failed to locate SMBIOS entry point");
};

void SmBiosBinary::processEntryPoint(u8 *ep)
{
	// validate ep struct: correct length, checksums, string anchors 
	size_t eplen = ep[0x05];
	if (eplen < 0x1f || !checksum(ep, ep[0x05])
	    || memcmp("_DMI_", ep + 0x10, 5) != 0
	    || !checksum(ep + 0x10, 0x0f)) {
		throw runtime_error("invalid entry point structure");
	}

	maxSize = WORD(ep + 0x08);
	specMajorVer = ep[0x06];
	specMinorVer = ep[0x07];
	tableLen = WORD(ep + 0x16);
	tablePtr = DWORD(ep + 0x18);
	nStructs = WORD(ep + 0x1c);
};

// based on dmidecode mem_chunk
u8 * SmBiosBinary::mapToProcess(const size_t base, 
				  const size_t len, 
				  const string path)
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

u8 * SmBiosBinary::advance(u8 *p, u8 len) 
{
	p += len;
	// walk to double null + 1;
	while (! (WORD(p) == 0x0000)) {
		p++;
	}
	return p+2;
};

// from dmidecode util.c
int SmBiosBinary::checksum(const u8 *buf, size_t len)
{
	u8 sum = 0;
	size_t a;

	for (a = 0; a < len; a++)
		sum += buf[a];
	return (sum == 0);
};

void  SmBiosBinary::get(BiosInfo& b)
{
	b.vendor = "quicktest";
};

void  SmBiosBinary::get(SmBiosInfo& s)
{
	s.version = "2.6";
};

SmBiosBinary::~SmBiosBinary() noexcept(true)
{
	if (table) free(table);
};
