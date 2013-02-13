#include <iostream>  // can remove when debug log impl
#include <vector>
#include "SmBios.h"
#include "SmReader.h"

using namespace std;

SmBios::SmBios(string p) 
{
	// find a strategy to deal with path
	// validate data in path has SMBIOS info
	
	//if path is devmem or dmidecode bindump
	reader = new SmBinaryReader(p);
	elements = reader->getAllElements();

	// if path is dmidecode text output1
	//reader = new SmTextReaderpath(path);
};

SmElement *SmBios::getElement(u8 t) 
{
	return reader->getElement(t);
};

const string SmBios::getVersion()
{
	return reader->getVersion();
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

// non-member functions
ostream &operator<<(ostream &os, const SmBios &smb)
{
	os << smb.reader->getVersion() << smb.reader->getTableInfo();
//	os << "v %u.%u Structures: %d  Max Size: %d bytes\nTable is %d bytes starting at 0x%08X\n", 
//	       majorVer, minorVer, nStructs, maxSize, tableLen, tablePtr);

};

