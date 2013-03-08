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

// I don't quite get why I need to define this, but the base class
// calls its own ctor... which is empty... and therefore never hits
// SmBiosBinary::getTable().

SmBiosBinary::SmBiosBinary(string f) : SmBios(f) 
{ 
	getTable(); 
}

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

u8 * SmBiosBinary::nextStruct(u8 *p) 
{
	p += p[0x01]; // go to end of formatted struct
	// walk to double null + 1, which is end of strings
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

u8 *SmBiosBinary::findStructure(u8 typ)
{
	int count = 1;
	u8 *sp;
	for (sp = table; 
	     (*sp != typ) && (count != nStructs); 
	     nextStruct(sp))
		count++;
	if (count == nStructs) {
		return NULL;
	}
	return sp;
}

string SmBiosBinary::getString(u8 *sp, u8 strNum)
{
	sp += sp[0x01];
	for (int i=1; i < strNum; ++i)
		while (*sp != '\0') ++sp;  // TODO need a cap on this
	return string(reinterpret_cast<char *>(sp));
}

void  SmBiosBinary::get(BiosInfo &b)
{
	// singular, so no need to iterate
	u8 structType = 0;
	u8 *s;

	s = findStructure(structType);
	if (s == NULL) throw runtime_error("BIOS Info not found");
	b.vendor = getString(s, *(s + 0x04));
};

void  SmBiosBinary::get(SmBiosInfo &s)
{
	s.version = to_string(specMajorVer) + "." + to_string(specMinorVer);
};

SmBiosBinary::~SmBiosBinary() noexcept(true)
{
	if (table) free(table);
};
