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

SmElement SmBios::getElement(u8 type) 
{
};

// per spec 2.7... TODO vary based on version of this smbios
inline bool SmBios::isValidType(u8 t)
{
	if ((t < 0) || (t > 127)) {
		return false;
	}
	if ((t > 42) && (t < 126)) {
		return false;
	}
	return true;
};

void SmBios::dumpTypes()
{
	for (auto it = smTypes.begin(); it != smTypes.end(); ++it)
		cout << it->type << " " <<  it->description << endl;
};

void perror_(string msg) 
{
	perror(msg.c_str());
}


// TODO move to a global debug function
void log(string msg)
{
	cout << msg << endl;
};

bool SmBios::decodePath(string p) 
{
	path = p;
	// find a strategy to deal with path
	// validate data in path has SMBIOS info
	
	//if path is devmem or dmidecode bindump
	SmReader *reader = new SmReader(new SmBinaryStratgy(path));
	// if path is dmidecode text output
	SmReader *reader = new SmReader(new SmTextStrategy(path));

	if (! pathContainsSmbios()) 
		throw runtime_error("Unable to find SMBIOS data in " + path);
	
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

const string SmBios::getVersion()
{
	size_t maxlen = 6;
	char s[maxlen];
	if (snprintf(s, maxlen, "%u.%u", majorVer, minorVer) >= maxlen) {
		log("warning: version truncated");
	}
	return string(s);
};
