#include <iostream>  // can remove when debug log impl

// C headers
#include <fcntl.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "SmBios.h"

using namespace std;

void perror_(string msg) 
{
	perror(msg.c_str());
}

// move to a global debug function
void SmBios::log(string msg)
{
	cout << msg << endl;
};

SmBios::SmBios()
{
	// first, do it the mmap way (dmidecode method)

	if (! getNonEfiEntryPoint("/dev/mem"))  {
		log("failed to find non-EFI entry point");
	}
};

bool SmBios::getNonEfiEntryPoint(string path)
{
	int fd = open(path.c_str(), O_RDONLY);
	if (fd == -1) {
		perror_(path);
		return false;
	}
	
	// map 64K memory into process virtual space
	const size_t base = 0xF0000; // smbios 2.7.1, sec. 5.2.1
	const size_t offset = base % sysconf(_SC_PAGESIZE);
	const size_t len = 0x10000;

	void *m = mmap(NULL, offset + len, PROT_READ, MAP_SHARED, fd, base - offset);
	if (m == MAP_FAILED) {
		perror_("mmap");
		return false;
	}

	// copy mapped memory into our own buffer
	u8 *buf = (u8 *)malloc(len);
	if (buf == NULL) {
		perror_("malloc");
		return false;
	}
	memcpy(buf, (u8 *)m + offset, len);
	if (munmap(m, offset + len) == -1) {
		perror_("munmap");
	}

	// find the entry point
	for (size_t fp = 0; fp < 0xFFF0; fp += 16) {
		if (memcmp((unsigned char *)buf + fp, "_SM_", 4) == 0 && fp <= 0xFFE0) {
			smbuf = buf + fp;
			log("smb found");
		} else if (memcmp((unsigned char *)buf + fp, "_DMI_", 5) == 0) {
			log("dmi found");
		}
	}
	
	// The contents of the Entry Point can be referenced like a char array
	if (smbuf) {		
		u8 eplen = smbuf[0x05];
		if (eplen != 0x1f && eplen != 0x1e) {
			log("Warning: unexpected SMBIOS entry point length");
		}
	}
	free(buf);
	return true;
};
