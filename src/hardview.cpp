#include <iostream>
#include <string>

// C headers
#include <fcntl.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>

using namespace std;

void log(string msg)
{
	cout << msg << endl;
}

int main(int argc, char * const argv[])
{
	// first, do it the mmap way (dmidecode method)

	size_t base = 0xF0000, len = 0x10000;
	size_t offset = base % sysconf(_SC_PAGESIZE);
	log("offset: " + to_string(offset));

	int fd = open("/dev/mem", O_RDONLY);
	if (fd == -1) {
		perror("/dev/mem");
		return errno;
	}
	
	void *p = malloc(len);
	if (p == NULL) {
		perror("malloc");
		return errno;
	}
	
	void *m = mmap(0, offset + len, PROT_READ, MAP_SHARED, fd, base - offset);
	if (m == MAP_FAILED) {
		log("map failed");
		return -1;
	}

	memcpy(p, (unsigned char *)m + offset, len);
	if (munmap(m, offset + len) == -1) {
		perror("munmap");
	}
	log("/dev/mem copied to buffer");

	for (size_t fp = 0; fp < 0xFFF0; fp += 16) {
		if (memcmp((unsigned char *)p + fp, "_SM_", 4) == 0 && fp <= 0xFFE0) {
			log("found smbios anchor! fp: " + to_string(fp));
		} else if (memcmp((unsigned char *)p + fp, "_DMI_", 5) == 0) {
			log("found legacy anchor! fp: " + to_string(fp));
		}
	}
	free(p);
}
